#!/usr/bin/env python3
import sys

EPS = [(-9, 1)]  # 1e-9
ONE = [(0, 1)]  # 1
NEG = [(0, -1)]  # -1

def read_value(s):  # (s: str) -> list[tuple[int, int]]:
    return [(int(s[13:]) - 9, int(s[:12].replace('.', '')))]  # (scale, unscaled-value) pair

def sign(xs):  # (xs: list[tuple[int, int]]) -> int:
    for _, x in sorted(xs, reverse=True):
        if x < 0:
            return -1  # negative
        elif x > 0:
            return 1  # positive
    return 0  # zero

def add(xs, ys):  # (xs: list[tuple[int, int]], ys: list[tuple[int, int]]) -> list[tuple[int, int]]:
    ret = []
    for s, t in sorted(xs + ys):
        if ret and s < ret[-1][0] + len(str(abs(ret[-1][1]))):  # merge overlapping digits
            ret[-1] = (ret[-1][0], ret[-1][1] + t * (10 ** (s - ret[-1][0])))
        else:
            ret += [(s, t)]
    return ret

def mul(xs, ys):  # (xs: list[tuple[int, int]], ys: list[tuple[int, int]]) -> list[tuple[int, int]]:
    ret = []
    for s1, t1 in xs:
        for s2, t2 in ys:
            ret += [(s1 + s2, t1 * t2)]
    return ret

def sub(xs, ys):  # (xs: list[tuple[int, int]], ys: list[tuple[int, int]]) -> list[tuple[int, int]]:
    return add(xs, mul(ys, NEG))

def absolute(xs):  # (xs: list[tuple[int, int]]) -> list[tuple[int, int]]:
    return mul(xs, NEG) if sign(xs) < 0 else xs

def less_than(xs, ys):  # (xs: list[tuple[int, int]], ys: list[tuple[int, int]]) -> bool:
    return sign(sub(xs, ys)) < 0

def minimum(xs, ys):  # (xs: list[tuple[int, int]], ys: list[tuple[int, int]]) -> list[tuple[int, int]]:
    return xs if less_than(xs, ys) else ys

# Parse input
x = read_value(sys.stdin.readline())
y = read_value(sys.stdin.readline())
a = read_value(sys.stdin.readline())
b = read_value(sys.stdin.readline())
c = read_value(sys.stdin.readline())
d = read_value(sys.stdin.readline())

# Compute
abs_err_add = absolute(sub(add(x, y), a))
abs_err_sub = absolute(sub(sub(x, y), b))
ret = [
    sign(abs_err_add) == 0 or less_than(abs_err_add, mul(EPS, minimum(ONE, absolute(add(x, y))))),
    sign(abs_err_sub) == 0 or less_than(abs_err_sub, mul(EPS, minimum(ONE, absolute(sub(x, y))))),
    less_than(absolute(sub(mul(x, y), c)), mul(EPS, minimum(ONE, absolute(mul(x, y))))),
    less_than(absolute(sub(x, mul(y, d))), mul(EPS, minimum(absolute(x), absolute(y)))),
]

# Output result
for r in ret:
    print('Correct' if r else 'Incorrect')
