"""
main.py - py-claw 程序入口
仅做参数转发，不含业务逻辑
"""
import sys
import os

# 将 src 目录加入模块搜索路径
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "src"))

from src.claw import cli_main

if __name__ == "__main__":
    sys.exit(cli_main(sys.argv[1:]))
