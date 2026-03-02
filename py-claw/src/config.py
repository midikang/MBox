"""
config.py - INI 格式配置文件读写
支持 [provider] / [gateway] / [general] 三个段
"""
import os
from typing import Optional


def config_path() -> str:
    """返回配置文件的绝对路径 ~/.cclaw/config.ini"""
    return os.path.join(os.path.expanduser("~"), ".cclaw", "config.ini")


def _ensure_dir(path: str) -> None:
    """确保目录存在"""
    d = os.path.dirname(path)
    if d and not os.path.isdir(d):
        os.makedirs(d, exist_ok=True)


# ── 极简 INI 解析 ─────────────────────────────────────────────

def _parse_ini(text: str) -> dict:
    """将 INI 文本解析为 {section: {key: value}} 字典"""
    result: dict = {}
    section = ""
    for raw in text.splitlines():
        line = raw.strip()
        if not line or line.startswith("#") or line.startswith(";"):
            continue
        if line.startswith("[") and line.endswith("]"):
            section = line[1:-1].strip()
            result.setdefault(section, {})
            continue
        if "=" in line:
            key, _, val = line.partition("=")
            result.setdefault(section, {})[key.strip()] = val.strip()
    return result


def _write_ini(data: dict) -> str:
    """将 {section: {key: value}} 字典序列化为 INI 文本"""
    lines = []
    for section, kv in data.items():
        lines.append(f"[{section}]")
        for k, v in kv.items():
            lines.append(f"{k} = {v}")
        lines.append("")
    return "\n".join(lines)


# ── 公共 API ──────────────────────────────────────────────────

def load() -> Optional["Config"]:  # noqa: F821
    """从磁盘加载配置；失败返回 None"""
    from .claw import Config, Provider

    path = config_path()
    if not os.path.isfile(path):
        return None
    try:
        with open(path, "r", encoding="utf-8") as f:
            data = _parse_ini(f.read())
    except OSError:
        return None

    p = data.get("provider", {})
    g = data.get("gateway", {})
    n = data.get("general", {})

    cfg = Config(
        provider=Provider(
            base_url=p.get("base_url", "https://api.openai.com/v1"),
            api_key=p.get("api_key", ""),
            model=p.get("model", "gpt-3.5-turbo"),
            temperature=float(p.get("temperature", "0.7")),
        ),
        gateway_port=int(g.get("port", "8080")),
        max_history=int(n.get("max_history", "20")),
    )
    return cfg


def save(cfg: "Config") -> bool:  # noqa: F821
    """将配置写入磁盘；成功返回 True"""
    path = config_path()
    try:
        _ensure_dir(path)
        data = {
            "provider": {
                "base_url":    cfg.provider.base_url,
                "api_key":     cfg.provider.api_key,
                "model":       cfg.provider.model,
                "temperature": f"{cfg.provider.temperature:.1f}",
            },
            "gateway": {
                "port": str(cfg.gateway_port),
            },
            "general": {
                "max_history": str(cfg.max_history),
            },
        }
        with open(path, "w", encoding="utf-8") as f:
            f.write(_write_ini(data))
        return True
    except OSError:
        return False
