#!/bin/python3

N, R = map(int, input().split())
pts = sorted(map(int, input().split()))

ans = []

while pts:
  best, bx, il = -1, -1, 0

  for ir in range(len(pts)):
    while pts[il] + 2*R < pts[ir]: il += 1
    if ir-il > best: best, bx = ir-il, pts[ir]-R
  ans.append(bx)
  pts = [x for x in pts if abs(x-bx) > R]

print(len(ans))
print(*ans)