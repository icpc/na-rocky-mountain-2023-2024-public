#!/usr/bin/env python3

import sys
from heapq import heappush, heappop
import bisect

lines = [line.strip() for line in sys.stdin.readlines()]
n, r = map(int, lines[0].split())
xs = sorted(map(int, lines[1].split()))

heap = []
j = 0
for i in range(n):
    if i < n - 1 and xs[i] == xs[i + 1]:
        continue
    while j < i and xs[j] < xs[i] - 2 * r:
        j += 1
    heappush(heap, (-(i - j + 1), xs[i] - r, j, i))

avail = [1 for _ in range(n)]
ret = []

while heap:
    c, x, left, right = heappop(heap)
    c = -c

    if not avail[right]:
        continue  # removed

    # count remaining points
    cnt = right - bisect.bisect_left(avail, 1, left, right + 1) + 1

    if c != cnt:
        heappush(heap, (-cnt, x, left, right))
        continue  # update

    i = right
    while i >= left and avail[i]:
        avail[i] = 0
        i -= 1

    ret += [x]

print(len(ret))
print(' '.join(map(str, ret)))
