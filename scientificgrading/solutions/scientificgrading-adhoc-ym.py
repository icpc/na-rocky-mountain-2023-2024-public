#!/usr/bin/env python3
import sys
from math import log10

def read_value(s):
    return int(s[13:]) - 9, int(s[:12].replace('.', ''))

def less_than_eps(significand, exponent):
    return significand == 0 or log10(significand) < -exponent - 9

def check(ax, ay, bx, by):
    if abs(ax - bx) > 10:
        return False  # relative error must be large

    base = min(ax, bx)
    a = ay * (10 ** (ax - base))
    b = by * (10 ** (bx - base))

    # exact
    if a == b:
        return True

    # relative error
    if not less_than_eps(abs((a - b) / a), 0):
        return False
    
    # absolute error
    return less_than_eps(abs(a - b), base)

def check_add(xx, xy, yx, yy, zx, zy):
    if xx < yx:
        xx, xy, yx, yy = yx, yy, xx, xy

    if xx == yx and xy + yy == 0:
        return zy == 0

    expected_scale = max(xx, yx)

    if abs(zx - expected_scale) > 20:
        return False

    if abs(xx - yx) <= 20:  # all close
        base = min(xx, yx, zx)
        x = xy * (10 ** (xx - base))
        y = yy * (10 ** (yx - base))
        z = zy * (10 ** (zx - base))
        if x + y - z == 0:
            return True

        # relative error
        if abs(x + y - z) * (10 ** 9) >= abs(x + y):
            return False

        # absolute error
        return less_than_eps(abs(x + y - z), base)
    else:
        base = min(xx, zx)
        x = xy * (10 ** (xx - base))
        z = zy * (10 ** (zx - base))

        # relative error
        if abs(x - z) * (10 ** 9) > abs(x):
            return False
        elif abs(x - z) * (10 ** 9) == abs(x):
            if (z < x) != (yy < 0):
                return False

        # absolute error
        if x == z:
            return less_than_eps(abs(yy), yx)

        if abs(x - z) == 1 and base == -9:
            return (z < x) == (yy < 0)
        return less_than_eps(abs(x - z), base)

# Parse input
xx, xy = read_value(sys.stdin.readline())
yx, yy = read_value(sys.stdin.readline())
ax, ay = read_value(sys.stdin.readline())
bx, by = read_value(sys.stdin.readline())
cx, cy = read_value(sys.stdin.readline())
dx, dy = read_value(sys.stdin.readline())

ret = [
    check_add(xx, xy, yx, yy, ax, ay),
    check_add(xx, xy, yx, -yy, bx, by),
    check(xx + yx, xy * yy, cx, cy),
    check(xx - yx, xy / yy, dx, dy),
]

# Output result
for r in ret:
    print('Correct' if r else 'Incorrect')
