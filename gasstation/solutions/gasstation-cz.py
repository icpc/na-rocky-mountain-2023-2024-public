#!/usr/bin/python3

from collections import deque
from heapq import *
p, n = map(int, input().split())

cars = []
for i in range(n):
    t, f, s = input().split()
    t = int(t)
    f = int(f)
    s = 0 if s == 'L' else 1
    cars.append((t, f, s))

cars.sort()
leave_time = ["-1" for _ in range(n)]
# pumps[pump col number][left or right][pump A or B]
pumps = [[[True for _ in range(2)] for _ in range(2)] for _ in range(p)]

# lines[pump col number][left or right]
lines = [[deque() for _ in range(2)] for _ in range(p)]

# we need to process whenever a car joins the line or when a car finishes getting gas
times_to_process = []

# we process cars leaving before a car joins
CAR_LEAVE = 0
CAR_JOIN = 1


for i in range(len(cars)):
    t, f, s = cars[i]
    heappush(times_to_process, (t, CAR_JOIN, 0, 0, 0, i))

prev = -1
while len(times_to_process) > 0:
    t, action, j, s, a, i = heappop(times_to_process)
    if action == CAR_LEAVE:
        pumps[j][s][a] = True
        # print(f"Car {car_id+1} left at time {t}")
        leave_time[i] = str(t)
        prev = CAR_LEAVE

    # move cars in queue after all cars leave
    if (action == CAR_JOIN and prev == CAR_LEAVE) or (len(times_to_process) == 0 or times_to_process[0][0] != t):
        # move cars from head of queue into the appropriate pump location
        for j in range(p):
            for s in range(2):
                if pumps[j][s][0] and pumps[j][s][1]:
                    # pump A and B are open, move car into pump B
                    if len(lines[j][s]) > 0:
                        car_id = lines[j][s].popleft()
                        _, f, _ = cars[car_id]
                        pumps[j][s][1] = False
                        # print(f"Car {car_id+1} fuels at {j+1}B at time {t}")
                        heappush(times_to_process,
                                 (t+f, CAR_LEAVE, j, s, 1, car_id))
                if pumps[j][s][0]:
                    # pump A is open (or still open)
                    if len(lines[j][s]) > 0:
                        car_id = lines[j][s].popleft()
                        _, f, _ = cars[car_id]
                        pumps[j][s][0] = False
                        # print(f"Car {car_id+1} fuels at {j+1}A at time {t}")
                        heappush(times_to_process,
                                 (t+f, CAR_LEAVE, j, s, 0, car_id))
    if action == CAR_JOIN:
        processed = False
        _, f, s = cars[i]
        # check if any lanes are open

        for j in range(p):
            if pumps[j][s][0]:
                processed = True
                # pump A is open
                if pumps[j][s][1]:
                    # pump A and B are open, move car into pump B
                    pumps[j][s][1] = False
                    heappush(times_to_process,
                             (t+f, CAR_LEAVE, j, s, 1, i))
                    break

                pumps[j][s][0] = False
                heappush(times_to_process,
                         (t+f, CAR_LEAVE, j, s, 0, i))
                break
        if not processed:  # we didn't find any open lanes
            shortest_val = n
            target_line = 0
            for j in range(p):
                if len(lines[j][s]) < shortest_val:
                    target_line = j
                    shortest_val = len(lines[j][s])
            lines[target_line][s].append(i)
            prev = CAR_JOIN

        prev = -1
print("\n".join(leave_time))
