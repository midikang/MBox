"""
memory.py - 对话记忆持久化
追加写入 ~/.cclaw/memory.log，格式：[epoch] role: content
不做索引，用 grep 搜索
"""
import os, time
from typing import List

_PATH = os.path.join(os.path.expanduser("~"), ".cclaw", "memory.log")


def _ensure():
    os.makedirs(os.path.dirname(_PATH), exist_ok=True)


def append(role: str, content: str) -> None:
    """追加一条记录"""
    _ensure()
    safe = content.replace("\\", "\\\\").replace("\n", "\\n").replace("\r", "\\r")
    with open(_PATH, "a", encoding="utf-8") as f:
        f.write(f"[{int(time.time())}] {role}: {safe}\n")


def load(max_count: int = 40) -> List["Message"]:  # noqa: F821
    """加载最近 max_count 条消息"""
    from .claw import Message
    if not os.path.isfile(_PATH):
        return []
    with open(_PATH, "r", encoding="utf-8") as f:
        lines = f.readlines()
    msgs = []
    for line in lines[-max_count:]:
        line = line.rstrip("\n")
        if not line.startswith("["):
            continue
        i = line.find("] ")
        if i == -1:
            continue
        rest = line[i + 2:]
        j = rest.find(": ")
        if j == -1:
            continue
        role, safe = rest[:j], rest[j + 2:]
        content = safe.replace("\\\\", "\\").replace("\\n", "\n").replace("\\r", "\r")
        msgs.append(Message(role=role, content=content))
    return msgs
