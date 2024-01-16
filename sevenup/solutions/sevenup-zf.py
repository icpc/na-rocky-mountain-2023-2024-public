#!/bin/python3

import functools

val = {
  'A':0, '2':1, '3':2, '4':3, '5':4, '6':5, '7':6,
  '8':7, '9':7, 'T':7, 'J':7, 'Q':7, 'K':7
}

@functools.lru_cache(maxsize = None)
def solve(up, rem):
  ans, tot = 0.0, 0

  if all(up): return ans

  for i in range(8):
    if rem[i]:
      tot += rem[i]
      t_up, t_rem = list(up), list(rem)
      
      t_rem[i] -= 1
      card = i
      
      while card < 7 and not t_up[card]:
        t_up[card] = 1
        card = val[dealt[card]]

      ans += rem[i]*solve(tuple(t_up), tuple(t_rem))

  return ans/tot + 1

up, rem = [False]*7, [4]*7+[24]

dealt = input()
for c in dealt: rem[val[c]] -= 1

print("{0:.010f}".format(solve(tuple(up), tuple(rem))))
