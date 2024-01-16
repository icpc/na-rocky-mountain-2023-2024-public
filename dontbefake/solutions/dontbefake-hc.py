#!/usr/bin/python3

import collections

MAX_TIME = 86400
freq = [0] * MAX_TIME

n = int(input())

for i in range(n):
    line = [ int(x) for x in input().split() ]
    m = line[0]
    for j in range(m):
        left = line[2*j+1]
        right = line[2*j+2]
        for k in range(left, right+1):
            freq[k] += 1
            
pics = max(freq)
counter = collections.Counter(freq)
secs = counter[pics]

print(str(pics))
print(str(secs))
