#!/bin/bash
# Verification script for c-claw project

echo "=== c-claw Project Verification ==="
echo ""

# 1. File structure
echo "1. File Structure Check"
echo "   ✓ Header files: $(find include -name '*.h' | wc -l) (expected: 1)"
echo "   ✓ Source files: $(find src -name '*.c' | wc -l) (expected: 9)"
echo "   ✓ Makefile: $(ls Makefile 2>/dev/null | wc -l) (expected: 1)"
echo ""

# 2. Line counts
echo "2. Line Count Check (must be < 1800)"
TOTAL_LINES=$(find . -name '*.c' -o -name '*.h' | xargs wc -l | tail -1 | awk '{print $1}')
echo "   Total C code lines: $TOTAL_LINES"
if [ $TOTAL_LINES -lt 1800 ]; then
    echo "   ✓ PASSED: Under 1800 line limit"
else
    echo "   ✗ FAILED: Exceeds 1800 line limit"
fi
echo ""

# 3. Build test
echo "3. Build Test"
make clean > /dev/null 2>&1
if make > /dev/null 2>&1; then
    echo "   ✓ Build successful"
    BINARY_SIZE=$(stat -c%s cclaw 2>/dev/null || echo "0")
    echo "   Binary size: $BINARY_SIZE bytes (~$((BINARY_SIZE/1024))KB)"
else
    echo "   ✗ Build failed"
fi
echo ""

# 4. Dependencies check
echo "4. Dependencies Check"
DEPS=$(ldd cclaw 2>/dev/null | grep -E "(libcurl|pthread)" | wc -l)
if [ $DEPS -ge 1 ]; then
    echo "   ✓ Required dependencies found:"
    ldd cclaw | grep -E "(libcurl|pthread)" | sed 's/^/     /'
else
    echo "   ✗ Missing required dependencies"
fi
echo ""

# 5. Basic functionality test
echo "5. Basic Functionality Test"
if ./cclaw 2>&1 | grep -q "Usage:"; then
    echo "   ✓ Binary runs and shows help"
else
    echo "   ✗ Binary execution failed"
fi
echo ""

# 6. Module breakdown
echo "6. Module Line Count Breakdown"
printf "   %-30s %s\n" "File" "Lines"
printf "   %-30s %s\n" "----" "-----"
for file in include/claw.h src/*.c Makefile; do
    printf "   %-30s %4d\n" "$file" "$(wc -l < $file)"
done
echo ""

echo "=== Verification Complete ==="
