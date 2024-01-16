#!/usr/bin/env python3
import sys

# Parse input.
lines = [line.strip() for line in sys.stdin.readlines()]
n = int(lines[0])
points = [tuple(map(int, lines[i].split())) for i in range(1, 1 + n)]
cx, cy, x, y = map(int, lines[1 + n].split())


# Determine orientation (-1, 0, 1) by computing the area of the polygon.
def orientation(ps):
    ret = sum((ps[(i + 1) % len(ps)][0] + ps[i][0]) * (ps[(i + 1) % len(ps)][1] - ps[i][1]) for i in range(len(ps)))
    return 1 if ret > 0 else -1 if ret < 0 else 0


# Check if the position p is on the segment (p1, p2).
def on_segment(p1, p2, p):
    if orientation([p1, p2, p]) != 0:
        return False
    return min(p1[0], p2[0]) <= p[0] <= max(p1[0], p2[0]) and min(p1[1], p2[1]) <= p[1] <= max(p1[1], p2[1])


polygon_orientation = orientation(points)
ret = 0

for i in range(n):
    segment = points[i], points[(i + 1) % n]

    if on_segment(points[i], points[(i + 1) % n], (cx, cy)):
        r = orientation([segment[0], segment[1], (x, y)])
        if r == 0:
            ret |= 3  # cannot determine
        elif r == polygon_orientation:
            ret |= 1  # in possible
        else:
            ret |= 2  # out possible

# assert 1 <= ret <= 3
if ret == 1:
	print('inside')
elif ret == 2:
	print('outside')
else:
	print('?')
