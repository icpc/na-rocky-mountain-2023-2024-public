#!/bin/python3

import functools, operator

def plays(s):
  ret = []
  for i in range(len(s)):
    if i > 0 and s[i-1] != s[i]: ret.append((i, i-1, s[:i-1]+s[i], s[i+1:]))
    if i+1 < len(s) and s[i] != s[i+1]: ret.append((i, i+1, s[:i], s[i]+s[i+2:]))
  return ret

@functools.lru_cache(maxsize = None)
def solve(s):
  nimbers = set()
  for _, _, s1, s2 in plays(s): nimbers.add(solve(s1)^solve(s2))
  return min(x for x in range(len(nimbers)+1) if x not in nimbers)

_, g = input(), input().split(".")

nimtot = functools.reduce(operator.xor, [solve(s) for s in g])

wins = []
clen = 0
for s in g:
  for i, j, s1, s2 in plays(s):
    if not nimtot^solve(s)^solve(s1)^solve(s2):
      wins.append((i+1+clen, j+1+clen))
  clen += len(s)+1

print(len(wins))
for t in wins: print(*t)