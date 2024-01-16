#!/usr/bin/env python3

import sys

lines = [line.strip() for line in sys.stdin.readlines()]
initial_state = lines[0]
faces = 'A23456789TJQK'
values = {c: i for i, c in enumerate(faces)}

initial = [initial_state.count(faces[i]) for i in range(13)]
dp = [[0.0 for _ in range(128)] for _ in range(46)]
dp[0][0] = 1.0

for turn in range(45):
    for mask in range(128):
        q = 1.0
        for value in range(7):
            if mask & (1 << value) == 0:
                # new open
                p = (4 - initial[value]) / (45 - turn)
                q -= p
                cur = value
                new_mask = 0
                while new_mask & (1 << cur) == 0 and cur < 7:
                    new_mask |= 1 << cur
                    cur = values[initial_state[cur]]
                dp[turn + 1][mask | new_mask] += dp[turn][mask] * p

        # no change
        dp[turn + 1][mask] += dp[turn][mask] * q

ret = 0.0
for turn in range(45):
    ret += (turn + 1) * (dp[turn + 1][127] - dp[turn][127])
print(f'{ret:.10f}')
