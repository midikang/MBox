"""
provider.py - OpenAI 兼容 API 客户端
手写 JSON 序列化与反序列化，仅依赖 requests
"""
import sys
from typing import List, Optional


# ── JSON 工具函数（手写，仅处理5种特殊字符）─────────────────────

def _escape_json(s: str) -> str:
    """对字符串做 JSON 转义，处理 5 种特殊字符并过滤其余控制字符"""
    out = []
    for ch in s:
        if ch == "\\":
            out.append("\\\\")
        elif ch == '"':
            out.append('\\"')
        elif ch == "\n":
            out.append("\\n")
        elif ch == "\r":
            out.append("\\r")
        elif ch == "\t":
            out.append("\\t")
        elif ord(ch) < 0x20:
            pass  # 过滤其余控制字符，避免生成非法 JSON
        else:
            out.append(ch)
    return "".join(out)


def _extract_content(json_text: str) -> Optional[str]:
    """
    从 OpenAI API 响应中提取 content 字段值。
    只做字符串搜索，不做完整 JSON 解析。
    目标结构：choices[0].message.content
    """
    # 先定位 "content" 键
    key = '"content"'
    pos = json_text.find(key)
    if pos == -1:
        return None

    pos += len(key)
    # 跳过空白和冒号
    while pos < len(json_text) and json_text[pos] in (' ', '\t', '\n', '\r', ':'):
        pos += 1

    if pos >= len(json_text) or json_text[pos] != '"':
        return None

    pos += 1  # 跳过开头的 "
    out = []
    while pos < len(json_text):
        ch = json_text[pos]
        if ch == '\\':
            pos += 1
            if pos >= len(json_text):
                break
            esc = json_text[pos]
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
            break  # 字符串结束
        else:
            out.append(ch)
        pos += 1

    return "".join(out)


# ── 公共 API ──────────────────────────────────────────────────

def chat(msgs: List["Message"], cfg: "Provider") -> Optional[str]:  # noqa: F821
    """
    调用 /chat/completions 接口，返回 assistant 的回复文本。
    失败返回 None 并打印错误信息。
    """
    import requests  # 仅在调用时导入，保持顶层无副作用

    # 手工构建 JSON 请求体
    msg_parts = []
    for m in msgs:
        msg_parts.append(
            f'{{"role":"{_escape_json(m.role)}",'
            f'"content":"{_escape_json(m.content)}"}}'
        )
    messages_json = ",".join(msg_parts)
    body = (
        f'{{"model":"{_escape_json(cfg.model)}",'
        f'"temperature":{cfg.temperature:.1f},'
        f'"messages":[{messages_json}]}}'
    )

    url = cfg.base_url.rstrip("/") + "/chat/completions"
    headers = {
        "Content-Type": "application/json",
        "Authorization": f"Bearer {cfg.api_key}",
    }

    try:
        resp = requests.post(url, data=body.encode("utf-8"), headers=headers, timeout=60)
    except Exception as e:
        print(f"Error: 请求失败 - {e}", file=sys.stderr)
        return None

    if resp.status_code != 200:
        print(f"Error: API 返回 {resp.status_code} - {resp.text[:200]}", file=sys.stderr)
        return None

    content = _extract_content(resp.text)
    if content is None:
        print(f"Error: 无法提取响应内容 - {resp.text[:200]}", file=sys.stderr)
    return content
