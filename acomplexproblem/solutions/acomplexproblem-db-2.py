#!/usr/bin/env python3
# Solution to the problem "A Complex Problem" in the
# Rocky Mountain Regional Contest 2023
# Author:  Dante Bencivenga
# Date:    2023-10-13

from sys import setrecursionlimit

setrecursionlimit(110000)

# this solution uses built-in I/O and does not convert strings to integers

m, n = [int(i) for i in input().split()]
names = set()

adj = [{}, {}]
count = 0

for i in range(m + n):
  inputs = input().split()
  for j in range(2):
    if inputs[j] not in names:
      names.add(inputs[j])
      adj[0][inputs[j]] = []
      adj[1][inputs[j]] = []
      count += 1
  adj[0 if i < m else 1][inputs[0]].append(inputs[1])

dist = {name:0 for name in names}
idx = {name:-1 for name in names}
low = {name:-1 for name in names}
stack = []
num = 0
sccs = 0

def dfs(i):
  global num, sccs
  stack.append(i)
  idx[i] = low[i] = num
  num += 1
  for k in range(2):
    for j in adj[k][i]:
      if idx[j] == -1:
        dfs(j)
        low[i] = min(low[i], low[j])
      else:
        low[i] = min(low[i], idx[j])
  if idx[i] == low[i]: # root of SCC
    sccs += 1
    scc = [i]
    while stack[-1] != i: scc.append(stack.pop())
    stack.pop()
    scc_dist = 0
    for j in scc:
      idx[j] = count
      for k in range(2):
        for l in adj[k][j]:
          scc_dist = max(scc_dist, dist[l] + k)
    for j in scc: dist[j] = scc_dist

for name in names:
  if idx[name] == -1: dfs(name)

print(max(dist.values()) + 1, sccs)
