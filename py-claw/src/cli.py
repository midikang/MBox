"""
cli.py - 命令行解析与路由
实现 onboard / chat / ask / gateway / cron add / daemon 六条命令
"""
import sys
from typing import List


# ── 帮助文本 ──────────────────────────────────────────────────

def _usage(prog: str = "pyclaw") -> None:
    print(f"Usage: {prog} <command> [options]\n")
    print("Commands:")
    print("  onboard                        初始化配置")
    print("  chat <prompt>                  带历史的对话")
    print("  ask  <prompt>                  单次提问（无历史）")
    print("  gateway [port]                 启动 HTTP 网关（默认 8080）")
    print("  cron add <interval> <prompt>   添加定时任务（秒）")
    print("  daemon                         后台服务（网关 + cron）")
    print("\nExamples:")
    print(f"  {prog} onboard")
    print(f"  {prog} chat \"你好，介绍一下自己\"")
    print(f"  {prog} ask  \"1+1等于几\"")
    print(f"  {prog} gateway 9000")
    print(f"  {prog} cron add 3600 \"每小时摘要\"")
    print(f"  {prog} daemon")


# ── 路由入口 ──────────────────────────────────────────────────

def run(argv: List[str]) -> int:
    """解析 argv 并分发到对应命令函数"""
    prog = "pyclaw"
    if not argv:
        _usage(prog)
        return 1

    cmd = argv[0]

    if cmd == "onboard":
        return do_onboard()

    if cmd == "chat":
        if len(argv) < 2:
            print("Error: chat 需要一个 prompt 参数", file=sys.stderr)
            return 1
        return do_chat(argv[1])

    if cmd == "ask":
        if len(argv) < 2:
            print("Error: ask 需要一个 prompt 参数", file=sys.stderr)
            return 1
        return do_ask(argv[1])

    if cmd == "gateway":
        port = 8080
        if len(argv) >= 2:
            try:
                port = int(argv[1])
                if not (1 <= port <= 65535):
                    raise ValueError
            except ValueError:
                print("Error: 无效端口号", file=sys.stderr)
                return 1
        return do_gateway(port)

    if cmd == "cron":
        if len(argv) < 2:
            print("Error: cron 需要子命令", file=sys.stderr)
            return 1
        if argv[1] == "add":
            if len(argv) < 4:
                print("Error: cron add 需要 <interval> <prompt>", file=sys.stderr)
                return 1
            try:
                interval = int(argv[2])
                if interval <= 0:
                    raise ValueError
            except ValueError:
                print("Error: 无效的间隔时间", file=sys.stderr)
                return 1
            return do_cron_add(interval, argv[3])
        print(f"Error: 未知的 cron 子命令 '{argv[1]}'", file=sys.stderr)
        return 1

    if cmd == "daemon":
        return do_daemon()

    print(f"Error: 未知命令 '{cmd}'", file=sys.stderr)
    _usage(prog)
    return 1


# ── 命令实现 ──────────────────────────────────────────────────

def do_onboard() -> int:
    """交互式初始化，生成 ~/.cclaw/config.ini"""
    from .claw import Config, Provider
    from .config import save

    print("=== py-claw Onboarding ===\n")

    base_url = input("Enter API base URL (e.g., https://api.openai.com/v1): ").strip()
    api_key  = input("Enter API key: ").strip()
    model    = input("Enter model name (e.g., gpt-3.5-turbo): ").strip()

    cfg = Config(
        provider=Provider(
            base_url=base_url or "https://api.openai.com/v1",
            api_key=api_key,
            model=model or "gpt-3.5-turbo",
            temperature=0.7,
        ),
        gateway_port=8080,
        max_history=20,
    )

    if not save(cfg):
        print("Error: 保存配置失败", file=sys.stderr)
        return 1

    from .config import config_path
    print("\n配置已保存！")
    print(f"配置文件：{config_path()}")
    return 0


def do_chat(prompt: str) -> int:
    """带历史记忆的对话"""
    from .config import load as cfg_load
    from .memory import load as mem_load, append as mem_append
    from .provider import chat

    cfg = cfg_load()
    if cfg is None:
        print("Error: 未找到配置，请先运行 'onboard'", file=sys.stderr)
        return 1

    # 加载历史消息，构造 Message 列表
    from .claw import Message
    history = mem_load(cfg.max_history * 2)
    send_msgs = [Message(role=m.role, content=m.content) for m in history]
    send_msgs.append(Message(role="user", content=prompt))

    response = chat(send_msgs, cfg.provider)
    if response is None:
        print("Error: 获取 AI 响应失败", file=sys.stderr)
        return 1

    print(response)
    mem_append("user", prompt)
    mem_append("assistant", response)
    return 0


def do_ask(prompt: str) -> int:
    """单次提问，不带历史"""
    from .config import load as cfg_load
    from .provider import chat
    from .claw import Message

    cfg = cfg_load()
    if cfg is None:
        print("Error: 未找到配置，请先运行 'onboard'", file=sys.stderr)
        return 1

    response = chat([Message(role="user", content=prompt)], cfg.provider)
    if response is None:
        print("Error: 获取 AI 响应失败", file=sys.stderr)
        return 1

    print(response)
    return 0


def do_gateway(port: int) -> int:
    """启动 HTTP 网关"""
    from .config import load as cfg_load
    from .gateway import start

    cfg = cfg_load()
    if cfg is None:
        print("Error: 未找到配置，请先运行 'onboard'", file=sys.stderr)
        return 1

    print(f"HTTP 网关启动，端口 {port}，按 Ctrl+C 退出")
    return start(port, cfg.provider)


def do_cron_add(interval: int, prompt: str) -> int:
    """添加一条定时任务"""
    from .cron import add

    if not add(interval, prompt):
        print("Error: 添加定时任务失败", file=sys.stderr)
        return 1

    print(f"定时任务已添加：每 {interval} 秒执行一次")
    print(f"Prompt: {prompt}")
    return 0


def do_daemon() -> int:
    """启动后台服务（网关 + cron 轮询）"""
    from .config import load as cfg_load
    from .daemon import start

    cfg = cfg_load()
    if cfg is None:
        print("Error: 未找到配置，请先运行 'onboard'", file=sys.stderr)
        return 1

    print(f"后台服务启动，网关端口 {cfg.gateway_port}，按 Ctrl+C 退出")
    return start(cfg)
