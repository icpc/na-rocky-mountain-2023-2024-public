#!/bin/python3

import functools

NULL_GAME = (0,0,0,0)
  
def moveL(l, r, cl, cr, i):
  if i == l:
    if cl == '.' or cl == game[i]: return None, None
    else: return NULL_GAME, (i+1, r, '.', cr)
  else:
    if game[i] == game[i-1]: return None, None
    else: return (l, i-1, cl, game[i]), (i+1, r, '.', cr)

def moveR(l, r, cl, cr, i):
  if i+1 == r:
    if cr == '.' or cr == game[i]: return None, None
    else: return (l, i, cl, '.'), NULL_GAME
  else:
    if game[i] == game[i+1]: return None, None
    else: return (l, i, cl, '.'), (i+2, r, game[i], cr)

@functools.lru_cache(maxsize=None)
def nimber(l, r, cl, cr):
  moves = set()

  if l == r:
    return 1 if cl != '.' and cr != '.' and cl != cr else 0
  
  if cl != '.' and cl != game[l]:
    moves.add(nimber(l+1, r, cl, cr))
  if cr != '.' and cr != game[r-1]:
    moves.add(nimber(l, r-1, cl, cr))
  
  for i in range(l, r):
    sL, sR = moveL(l, r, cl, cr, i)
    if sL is not None: moves.add(nimber(*sL) ^ nimber(*sR))
    
    sL, sR = moveR(l, r, cl, cr, i)
    if sL is not None: moves.add(nimber(*sL) ^ nimber(*sR))

  x = 0
  while x in moves: x += 1
  return x  

input()
game = input()+'.'

# just make sure this works correctly
assert nimber(*NULL_GAME) == 0

subgames = []
n_tot = 0
l = 0
for r in range(len(game)):
  if game[r] == '.':
    if l != r:
      subgames.append((l, r, '.', '.'))
      n_tot ^= nimber(*subgames[-1])
    l = r+1

win_moves = []

for i in range(len(subgames)):
  l, r, cl, cr = subgames[i]
  for j in range(l, r):
    sL, sR = moveL(l, r, cl, cr, j)
    if sL is not None and n_tot^nimber(*subgames[i])^nimber(*sL)^nimber(*sR) == 0:
      win_moves.append((j+1, j))
    sL, sR = moveR(l, r, cl, cr, j)
    if sL is not None and n_tot^nimber(*subgames[i])^nimber(*sL)^nimber(*sR) == 0:
      win_moves.append((j+1, j+2))

print(len(win_moves))
for m in win_moves: print(*m)

