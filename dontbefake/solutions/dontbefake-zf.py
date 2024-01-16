#!/bin/python3

freq = [0]*86400

for _ in range(int(input())):
  f = list(map(int, input().split()))
  for i in range(f[0]):
    for t in range(f[2*i+1], f[2*i+2]+1):
      freq[t] += 1

mx = max(freq)
print(mx)
print(len([t for t in freq if t == mx]))