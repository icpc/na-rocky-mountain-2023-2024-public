#!/bin/python3

N = int(input())

calls = [input() for _ in range(N)]

absent = []
for i in range(N):
  if calls[i] != "Present!" and (i == N-1 or calls[i+1] != "Present!"):
    absent.append(calls[i])

if absent: print(*absent, sep="\n")
else: print("No Absences")
