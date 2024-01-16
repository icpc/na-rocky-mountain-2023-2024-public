#!/usr/bin/env python3
# Solution to the problem "A Complex Problem" in the
# Rocky Mountain Regional Contest 2023
# Author:  Dante Bencivenga
# Date:    2023-09-23

from sys import stdin, setrecursionlimit

setrecursionlimit(110000)

input = stdin.readline

m, n = [int(i) for i in input().split()]

name_to_int = {}
adj = [[], []]
count = 0

for i in range(m + n):
  inputs = input().split()
  for j in range(2):
    if inputs[j] not in name_to_int:
      name_to_int[inputs[j]] = count
      adj[0].append([])
      adj[1].append([])
      count += 1
    inputs[j] = name_to_int[inputs[j]]
  adj[0 if i < m else 1][inputs[0]].append(inputs[1])

dist = [0 for _ in range(count)]
idx = [-1 for _ in range(count)]
low = [-1 for _ in range(count)]
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

for i in range(count):
  if idx[i] == -1: dfs(i)

print(max(dist) + 1, sccs)
