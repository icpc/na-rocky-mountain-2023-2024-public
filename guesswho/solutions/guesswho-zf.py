#!/bin/python3

N, M, Q = map(int, input().split())

attr = [input() for _ in range(N)]

valid = list(range(N))

for _ in range(Q):
  a, b = input().split()
  a = int(a)

  valid = [i for i in valid if attr[i][a-1] == b]

if len(valid) > 1:
  print("ambiguous")
  print(len(valid))
elif len(valid) == 1:
  print("unique")
  print(valid[0]+1)
else:
  print("impossible")