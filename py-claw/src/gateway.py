"""
gateway.py - 基于标准库的极简 HTTP 服务器
只支持两个端点：GET /health  POST /chat
"""
import sys
import socket
import threading
from typing import Optional


# ── 请求分包接收 ──────────────────────────────────────────────

def _recv_request(conn: socket.socket) -> str:
    """循环读取直到获得完整 HTTP 请求"""
    buf = b""
    conn.settimeout(5.0)
    try:
        while True:
            chunk = conn.recv(4096)
            if not chunk:
                break
            buf += chunk
            # headers 结束标志
            if b"\r\n\r\n" not in buf:
                continue
            header_end = buf.index(b"\r\n\r\n") + 4
            headers_raw = buf[:header_end].decode("utf-8", errors="replace")
            # 查找 Content-Length
            body_received = buf[header_end:]
            content_length = 0
            for line in headers_raw.splitlines():
                if line.lower().startswith("content-length:"):
                    try:
                        content_length = int(line.split(":", 1)[1].strip())
                    except ValueError:
                        pass
            if len(body_received) >= content_length:
                break
    except (socket.timeout, OSError):
        pass
    return buf.decode("utf-8", errors="replace")


# ── 响应构建 ──────────────────────────────────────────────────

def _response(status: str, body: str, content_type: str = "application/json") -> bytes:
    encoded = body.encode("utf-8")
    header = (
        f"HTTP/1.1 {status}\r\n"
        f"Content-Type: {content_type}; charset=utf-8\r\n"
        f"Content-Length: {len(encoded)}\r\n"
        f"Connection: close\r\n"
        "\r\n"
    )
    return header.encode("utf-8") + encoded


# ── 极简路由 ──────────────────────────────────────────────────

def _handle(raw: str, provider_cfg) -> bytes:
    """直接字符串比对做路由，不用路由表"""
    first_line = raw.split("\r\n", 1)[0] if raw else ""

    # GET /health
    if first_line.startswith("GET /health"):
        return _response("200 OK", '{"status":"ok"}')

    # POST /chat
    if first_line.startswith("POST /chat"):
        # 提取 body（headers 之后的部分）
        body = ""
        if "\r\n\r\n" in raw:
            body = raw.split("\r\n\r\n", 1)[1]

        # 从 body 中提取 prompt 字段（手写，只找 "prompt":"..."）
        prompt = _extract_prompt(body)
        if not prompt:
            return _response("400 Bad Request", '{"error":"missing prompt"}')

        from .claw import Message
        from .provider import chat
        response = chat([Message(role="user", content=prompt)], provider_cfg)
        if response is None:
            return _response("500 Internal Server Error", '{"error":"provider error"}')

        # 手写 JSON 转义响应
        from .provider import _escape_json
        return _response("200 OK", f'{{"response":"{_escape_json(response)}"}}')

    return _response("404 Not Found", '{"error":"not found"}')


def _extract_prompt(body: str) -> Optional[str]:
    """从请求 body 里提取 prompt 字符串值"""
    key = '"prompt"'
    pos = body.find(key)
    if pos == -1:
        return None
    pos += len(key)
    while pos < len(body) and body[pos] in (' ', '\t', '\n', '\r', ':'):
        pos += 1
    if pos >= len(body) or body[pos] != '"':
        return None
    pos += 1
    out = []
    while pos < len(body):
        ch = body[pos]
        if ch == '\\':
            pos += 1
            if pos < len(body):
                esc = body[pos]
                if esc == 'n':
                    out.append('\n')
                elif esc == 'r':
                    out.append('\r')
                elif esc == 't':
                    out.append('\t')
                elif esc == '\\':
                    out.append('\\')
                elif esc == '"':
                    out.append('"')
                else:
                    out.append(esc)
        elif ch == '"':
            break
        else:
            out.append(ch)
        pos += 1
    return "".join(out) or None


# ── 服务器主循环 ──────────────────────────────────────────────

_stop_flag = threading.Event()


def start(port: int, provider_cfg) -> int:
    """启动 HTTP 服务，阻塞直到 Ctrl+C"""
    global _stop_flag
    _stop_flag.clear()

    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        srv.bind(("0.0.0.0", port))  # 绑定所有网络接口（网关服务器的预期行为）
    except OSError as e:
        print(f"Error: 无法绑定端口 {port} - {e}", file=sys.stderr)
        return 1
    srv.listen(16)
    srv.settimeout(1.0)

    try:
        while not _stop_flag.is_set():
            try:
                conn, _ = srv.accept()
            except socket.timeout:
                continue
            except OSError:
                break
            t = threading.Thread(target=_serve_conn, args=(conn, provider_cfg), daemon=True)
            t.start()
    except KeyboardInterrupt:
        pass
    finally:
        srv.close()
    return 0


def stop() -> None:
    """发出停止信号"""
    _stop_flag.set()


def _serve_conn(conn: socket.socket, provider_cfg) -> None:
    """在独立线程中处理单个连接"""
    try:
        raw = _recv_request(conn)
        resp = _handle(raw, provider_cfg)
        conn.sendall(resp)
    except OSError:
        pass
    finally:
        try:
            conn.close()
        except OSError:
            pass
