#!/usr/bin/env python3
import sys
from typing import Sequence


M = 1000000007

# Precompute modular inverses: i * modinv[i] == 1 (mod M) for 1 <= i <= 9
modinv = [0, 1, 500000004, 333333336, 250000002, 400000003, 166666668, 142857144, 125000001, 111111112]

# ===============================================================================
#    Data Structures
# ===============================================================================

class MulTree:
    """Modified Binary Indexed Tree."""

    def __init__(self, xs: Sequence[int]) -> None:
        n = len(xs)
        self.n = n
        self.data = [1 for _ in range(n + 1)]
        self.datainv = [1 for _ in range(n + 1)]
        self.dataraw = xs.copy()

        # populate initial elements
        for i in range(n):
            self._mul(i + 1, xs[i], modinv[xs[i]])

    def update(self, i: int, x: int) -> None:
        prev = self.dataraw[i]
        self.dataraw[i] = x
        self._mul(i + 1, modinv[prev] * x % M, modinv[x] * prev % M)

    def query(self, a: int, b: int) -> int:
        return self._prod(b) * self._prodinv(a) % M

    def _prod(self, i: int) -> int:
        ret = 1
        while i > 0:
            ret = ret * self.data[i] % M
            i -= i & -i
        return ret

    def _prodinv(self, i: int) -> int:
        ret = 1
        while i > 0:
            ret = ret * self.datainv[i] % M
            i -= i & -i
        return ret

    def _mul(self, i: int, x: int, xinv: int) -> None:
        while i <= self.n:
            self.data[i] = self.data[i] * x % M
            self.datainv[i] = self.datainv[i] * xinv % M
            i += i & -i

class OneSidedMaxTree:
    def __init__(self, xs: Sequence[int]) -> None:
        n = len(xs)
        N = 1 << (n.bit_length() + 1)  # 2 ^ (floor(lg n) + 1)

        self.n = n
        self.N = N
        self.data = [-1] * (2 * N)  # self.data[0] is always default

        # populate initial elements
        for i in range(N + n - 1, 0, -1):
            self.data[i] = xs[i - N] if i >= N else max(self.data[i * 2], self.data[i * 2 + 1])

    def update(self, index: int, value: int) -> None:
        # preconditions:
        #   - value in [index, -1]
        #   - self.data[index + self.N] != value

        k = index + self.N
        self.data[k] = value
        while k > 1:
            k //= 2
            self.data[k] = max(self.data[k * 2], self.data[k * 2 + 1])

    def query(self, index: int) -> int:
        """Returns the maximum value in the index range [0, index)."""
        ret = -1

        t = self.N
        while index > 0:
            if index & 1 == 1:
                ## for any j such that xs[j] >= 0, we have xs[i] <= xs[j] for any i <= j
                ret = self.data[t + index - 1]
                if ret >= 0:
                    break
            t >>= 1
            index >>= 1
        return ret


# ===============================================================================
#    Main Logic
# ===============================================================================

# Parse input.
n, m = map(int, sys.stdin.readline().split())
nop = n - 1  # number of operators

initial_condition = sys.stdin.readline()
xs = [int(initial_condition[i]) for i in range(0, 2 * n, 2)]
ops = [int(initial_condition[i] == '*') for i in range(1, 2 * nop, 2)]

queries = [sys.stdin.readline() for _ in range(m)]

# Create segment trees.
multree = MulTree(xs)
maxplustree = OneSidedMaxTree([i if not ops[i] else -1 for i in range(nop)])
maxmultree = OneSidedMaxTree([i if ops[i] else -1 for i in range(nop)])
minplustree = OneSidedMaxTree([i if not ops[nop - i - 1] else -1 for i in range(nop)])
minmultree = OneSidedMaxTree([i if ops[nop - i - 1] else -1 for i in range(nop)])

current_phase = 0
total = [0, 0]  # keep track of two values for both phases

# Compute current total values.
sofar = [xs[0], xs[0]]  # partial result of multiplication
for i in range(n):
    for phase in range(2):
        if i == n - 1:
            total[phase] = (total[phase] + sofar[phase]) % M
            continue
        if ops[i] == phase:
            total[phase] = (total[phase] + sofar[phase]) % M
            sofar[phase] = 1
        sofar[phase] = sofar[phase] * xs[i + 1] % M

sys.stdout.write(str(total[current_phase]) + '\n')

# Process each query.
output = []
for t in range(m):
    tokens = queries[t].split()
    if tokens[0] == 'a':
        # -------------------------------------------------------------------------------
        #    All Flip
        # -------------------------------------------------------------------------------
        current_phase ^= 1
    elif tokens[0] == 'f':
        # -------------------------------------------------------------------------------
        #    Flip
        # -------------------------------------------------------------------------------
        op_idx = int(tokens[1]) - 1

        for phase in range(2):
            # Find continuous multiplications.
            maxtree = [maxplustree, maxmultree][phase]
            mintree = [minplustree, minmultree][phase]

            mul_start = op_idx if (op_idx == 0 or ops[op_idx - 1] == phase) else maxtree.query(op_idx) + 1
            mul_end = (op_idx + 2) if (op_idx == nop - 1 or ops[op_idx + 1] == phase) else nop - mintree.query(nop - op_idx - 1)
 
            left_mul = xs[op_idx] if mul_start == op_idx else multree.query(mul_start, op_idx + 1)
            right_mul = xs[op_idx + 1] if mul_end == op_idx + 2 else multree.query(op_idx + 1, mul_end)

            # Update total values.
            diff = left_mul * right_mul - left_mul - right_mul
            sign = 1 if ops[op_idx] == phase else -1
            total[phase] = (total[phase] + diff * sign) % M

        # # Update segment trees.
        if not ops[op_idx]:
            maxmultree.update(op_idx, op_idx)
            maxplustree.update(op_idx, -1)
            minmultree.update(nop - op_idx - 1, nop - op_idx - 1)
            minplustree.update(nop - op_idx - 1, -1)
        else:
            maxmultree.update(op_idx, -1)
            maxplustree.update(op_idx, op_idx)
            minmultree.update(nop - op_idx - 1, -1)
            minplustree.update(nop - op_idx - 1, nop - op_idx - 1)

        # Update arrays.
        ops[op_idx] = 1 - ops[op_idx]
    else:
        # -------------------------------------------------------------------------------
        #    Swap
        # -------------------------------------------------------------------------------
        i, j = int(tokens[1]) - 1, int(tokens[2]) - 1
        i, j = min(i, j), max(i, j)

        if xs[i] != xs[j]:  # no change when swapping same values (possibly same indices)
            # Find components including i, j.
            t = [[[0, n], [0, n]], [[0, n], [0, n]]]
            for k in range(2):
                num_idx = [i, j][k]
                if num_idx == 0:
                    # left-most index
                    t[0][k][0] = 0
                    t[1][k][0] = 0
                elif not ops[num_idx - 1]:
                    t[0][k][0] = num_idx
                    t[1][k][0] = maxmultree.query(num_idx) + 1
                else:
                    t[0][k][0] = maxplustree.query(num_idx) + 1
                    t[1][k][0] = num_idx

                if num_idx == n - 1:
                    # right-most index
                    t[0][k][1] = n
                    t[1][k][1] = n
                elif not ops[num_idx]:
                    t[0][k][1] = num_idx + 1
                    t[1][k][1] = nop - minmultree.query(nop - num_idx)
                else:
                    t[0][k][1] = nop - minplustree.query(nop - num_idx)
                    t[1][k][1] = num_idx + 1

            # Update total values.
            for phase in range(2):
                if t[phase][0][0] != t[phase][1][0]:
                    x = multree.query(t[phase][0][0], t[phase][0][1]) * modinv[xs[i]] % M
                    y = multree.query(t[phase][1][0], t[phase][1][1]) * modinv[xs[j]] % M
                    total[phase] = (total[phase] - (xs[i] - xs[j]) * (x - y)) % M

            # Update segment trees.
            multree.update(i, xs[j])
            multree.update(j, xs[i])

            # Update arrays.
            xs[i], xs[j] = xs[j], xs[i]

    output += [str(total[current_phase])]

sys.stdout.write('\n'.join(output) + '\n')
