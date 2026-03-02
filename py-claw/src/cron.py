"""
cron.py - 定时任务引擎
纯文本存储，格式：id|interval_sec|next_run_epoch|enabled|prompt
"""
import os
import time
import sys
from typing import List


def _cron_path() -> str:
    return os.path.join(os.path.expanduser("~"), ".cclaw", "cron.txt")


def _ensure_dir(path: str) -> None:
    d = os.path.dirname(path)
    if d and not os.path.isdir(d):
        os.makedirs(d, exist_ok=True)


# ── 读写 ──────────────────────────────────────────────────────

def _load() -> List["CronTask"]:  # noqa: F821
    """从文件加载任务列表"""
    from .claw import CronTask

    path = _cron_path()
    if not os.path.isfile(path):
        return []

    tasks: List[CronTask] = []
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.rstrip("\n")
            if not line:
                continue
            parts = line.split("|", 4)
            if len(parts) < 5:
                continue
            try:
                t = CronTask(
                    id=int(parts[0]),
                    interval_sec=int(parts[1]),
                    next_run=int(parts[2]),
                    enabled=int(parts[3]),
                    prompt=parts[4],
                )
                tasks.append(t)
            except ValueError:
                continue  # 跳过损坏行
    return tasks


def _save(tasks: List["CronTask"]) -> bool:  # noqa: F821
    """将任务列表写回文件"""
    path = _cron_path()
    try:
        _ensure_dir(path)
        with open(path, "w", encoding="utf-8") as f:
            for t in tasks:
                f.write(f"{t.id}|{t.interval_sec}|{t.next_run}|{t.enabled}|{t.prompt}\n")
        return True
    except OSError:
        return False


# ── 公共 API ──────────────────────────────────────────────────

def add(interval: int, prompt: str) -> bool:
    """添加一条定时任务；返回 True 表示成功"""
    from .claw import CronTask

    tasks = _load()
    new_id = max((t.id for t in tasks), default=0) + 1
    task = CronTask(
        id=new_id,
        interval_sec=interval,
        next_run=int(time.time()) + interval,
        enabled=1,
        prompt=prompt,
    )
    tasks.append(task)
    return _save(tasks)


def tick(provider_cfg) -> None:
    """
    执行一次定时检查。
    对所有到期且已启用的任务：调用 provider_chat，记录结果，更新 next_run。
    """
    from .provider import chat
    from .claw import Message
    from .memory import append as mem_append

    tasks = _load()
    if not tasks:
        return

    now = int(time.time())
    changed = False
    for t in tasks:
        if t.enabled and now >= t.next_run:
            print(f"[cron] 任务 #{t.id} 触发：{t.prompt[:50]}", file=sys.stderr)
            response = chat([Message(role="user", content=t.prompt)], provider_cfg)
            if response:
                mem_append("assistant", f"[cron#{t.id}] {response}")
            t.next_run = now + t.interval_sec
            changed = True

    if changed:
        _save(tasks)
