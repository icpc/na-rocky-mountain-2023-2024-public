#!/usr/bin/python3
import sys
import math

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y    

    def read(self):
        line = input()
        self.x, self.y = map(int, line.split())

MAX_N = 200;
p = [Point(0,0)] * MAX_N
N = 0

def sqdist(i, j):
    dx = p[i].x - p[j].x
    dy = p[i].y - p[j].y
    return dx*dx + dy*dy;

def solve1():
    best = sys.float_info.max
    if N < 3:
        return best

    for i in range(N):
        max1, max2 = -1, -1
        
        for j in range(N):
            if i == j:
                continue
            d = sqdist(i, j)
            if d > max1:
                max2 = max1
                max1 = d
            elif d > max2:
                max2 = d

        best = min(best, math.sqrt(max1) + math.sqrt(max2))

    return best;

def solve2():
    best_dist = sys.float_info.max
    if N < 4:
        return best_dist
    
    L = [[(sqdist(i,j), j) for j in range(N)] for i in range(N)]
    for i in range(N):
        L[i].sort()
        

    for i in range(N):
        for j in range(i+1, N):
            coveredi = [False] * MAX_N
            coveredj = [True] * MAX_N

            jindex = N-1
            for k in range(N):
                coveredi[L[i][k][1]] = True
                while jindex > 0 and coveredi[L[j][jindex][1]]:
                    coveredj[L[j][jindex][1]] = False
                    jindex -= 1
                if L[i][k][0] == 0:
                    continue
                if coveredj[L[i][k][1]]:
                    continue
                if L[j][jindex][0] == 0:
                    break
                dist = math.sqrt(L[i][k][0]) + math.sqrt(L[j][jindex][0]) + math.sqrt(sqdist(i, j))
                best_dist = min(best_dist, dist);

    return best_dist;

def solve():
    if N == 2:
        return math.sqrt(sqdist(0, 1))

    ans1 = solve1()
    ans2 = solve2()

    return min(ans1, ans2)

N = int(input())
for i in range(N):
    tp = Point(0,0)
    tp.read()
    p[i] = tp

ans = solve()
print("{:.10f}".format(ans))
