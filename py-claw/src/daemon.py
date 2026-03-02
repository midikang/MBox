"""
daemon.py - 多线程后台服务
子线程跑 HTTP 网关，主线程做 cron 轮询
"""
import sys
import time
import threading


def start(cfg) -> int:
    """
    启动后台服务：
      - 子线程：HTTP 网关
      - 主线程：cron 定时任务轮询（1 秒间隔）
    """
    from .gateway import start as gw_start, stop as gw_stop
    from .cron import tick

    # 子线程启动网关
    gw_thread = threading.Thread(
        target=gw_start,
        args=(cfg.gateway_port, cfg.provider),
        daemon=True,
        name="gateway",
    )
    gw_thread.start()
    print(f"网关已启动，监听端口 {cfg.gateway_port}")

    # 主线程做 cron 轮询
    try:
        while True:
            tick(cfg.provider)
            time.sleep(1)
    except KeyboardInterrupt:
        print("\n正在停止服务...")
    finally:
        gw_stop()
        gw_thread.join(timeout=3)

    return 0
