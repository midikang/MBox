#!/bin/bash
# c-claw 测试脚本 / Test Script

echo "=========================================="
echo "c-claw 编译测试 / Compilation Test"
echo "=========================================="
echo ""

# 显示二进制文件信息
echo "二进制文件信息 / Binary Information:"
ls -lh cclaw
file cclaw
echo ""

# 显示依赖关系
echo "依赖库 / Dependencies:"
ldd cclaw | grep -E "(libcurl|pthread|libc)"
echo ""

# 测试帮助信息
echo "=========================================="
echo "帮助信息测试 / Help Test"
echo "=========================================="
./cclaw
echo ""

# 显示项目结构
echo "=========================================="
echo "项目结构 / Project Structure"
echo "=========================================="
tree -L 2 -I 'obj' || find . -type f \( -name "*.c" -o -name "*.h" -o -name "Makefile" \) | sort
echo ""

# 代码行数统计
echo "=========================================="
echo "代码统计 / Code Statistics"
echo "=========================================="
echo "模块 / Module                      行数 / Lines"
echo "--------------------------------------------"
for file in include/claw.h src/*.c Makefile; do
    printf "%-35s %4d\n" "$file" "$(wc -l < $file)"
done
echo "--------------------------------------------"
printf "%-35s %4d\n" "总计 / Total" "$(find . -name '*.c' -o -name '*.h' | xargs wc -l | tail -1 | awk '{print $1}')"
echo ""

# 运行验证脚本
echo "=========================================="
echo "完整验证 / Full Verification"
echo "=========================================="
./verify.sh
