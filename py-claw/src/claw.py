"""
claw.py - py-claw 核心类型定义与API声明
数据结构 + 对外接口，不含业务实现
"""
from dataclasses import dataclass, field
from typing import List, Optional


# ── 核心数据结构 ──────────────────────────────────────────────

@dataclass
class Message:
    role: str        # user / assistant
    content: str


@dataclass
class Provider:
    base_url: str = "https://api.openai.com/v1"
    api_key: str = ""
    model: str = "gpt-3.5-turbo"
    temperature: float = 0.7


@dataclass
class CronTask:
    id: int = 0
    interval_sec: int = 3600
    next_run: int = 0   # epoch 秒
    enabled: int = 1
    prompt: str = ""


@dataclass
class Config:
    provider: Provider = field(default_factory=Provider)
    gateway_port: int = 8080
    max_history: int = 20


# ── CLI 命令接口 ──────────────────────────────────────────────

def cli_main(argv: List[str]) -> int:
    from .cli import run; return run(argv)  # noqa: E702

def cmd_onboard() -> int:
    from .cli import do_onboard; return do_onboard()  # noqa: E702

def cmd_chat(prompt: str) -> int:
    from .cli import do_chat; return do_chat(prompt)  # noqa: E702

def cmd_ask(prompt: str) -> int:
    from .cli import do_ask; return do_ask(prompt)  # noqa: E702

def cmd_gateway(port: int) -> int:
    from .cli import do_gateway; return do_gateway(port)  # noqa: E702

def cmd_cron_add(interval: int, prompt: str) -> int:
    from .cli import do_cron_add; return do_cron_add(interval, prompt)  # noqa: E702

def cmd_daemon() -> int:
    from .cli import do_daemon; return do_daemon()  # noqa: E702


# ── Provider API ──────────────────────────────────────────────

def provider_chat(msgs: List[Message], cfg: Provider) -> Optional[str]:
    from .provider import chat; return chat(msgs, cfg)  # noqa: E702


# ── 配置 API ──────────────────────────────────────────────────

def config_load() -> Optional[Config]:
    from .config import load; return load()  # noqa: E702

def config_save(cfg: Config) -> bool:
    from .config import save; return save(cfg)  # noqa: E702


# ── 记忆 API ──────────────────────────────────────────────────

def memory_append(role: str, content: str) -> None:
    from .memory import append; append(role, content)  # noqa: E702

def memory_load(max_count: int = 40) -> List[Message]:
    from .memory import load; return load(max_count)  # noqa: E702


# ── Cron API ──────────────────────────────────────────────────

def cron_add(interval: int, prompt: str) -> bool:
    from .cron import add; return add(interval, prompt)  # noqa: E702

def cron_tick(cfg: Provider) -> None:
    from .cron import tick; tick(cfg)  # noqa: E702
