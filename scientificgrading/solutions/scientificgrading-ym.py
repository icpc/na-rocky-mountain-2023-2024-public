#!/usr/bin/env python3
import sys


# ===============================================================================
#    Data Structure
# ===============================================================================

class Value:
    """
    Data structure to support precise computations.
    A value is represented as the sum of fragments (exponent-significand pairs).
    """

    def __init__(self, fragments) -> None:
        """Initializes an instance from the given fragments."""

        self.fragments = {i: d for i, d in fragments.items() if d != 0}

    @staticmethod
    def parse(s: str):
        """Constructs a Value instance from a string."""

        sign = 1 if s[0] == '+' else -1
        m, n = s.replace('.', '').split('e', 1)
        fragments = {}
        for i, d in enumerate(m[1:]):
            if d != '0':
                fragments[int(n) - i] = int(d) * sign
        return Value(fragments)

    def __repr__(self) -> str:
        """Returns a string representation for debugging."""

        sorted_data = [(i, self.fragments[i]) for i in sorted(self.fragments.keys())]
        return f'Value(fragments={sorted_data})'

    def __add__(self, other):
        """Adds another Value to this instance."""

        if not isinstance(other, Value):
            raise ValueError()

        fragments = self.fragments.copy()
        for i, d in other.fragments.items():
            fragments[i] = self.fragments.get(i, 0) + d
        return Value(fragments)

    def __sub__(self, other):
        """Subtracts another Value from this instance."""

        if not isinstance(other, Value):
            raise ValueError()

        return self + (-other)

    def __neg__(self):
        """Flips the sign of this instance."""

        return Value({i: -d for i, d in self.fragments.items()})

    def __mul__(self, other):
        """Multiplies this instance by another Value."""

        if not isinstance(other, Value):
            raise ValueError()

        fragments = {}
        for i, a in self.fragments.items():
            for j, b in other.fragments.items():
                fragments[i + j] = fragments.get(i + j, 0) + a * b
        return Value(fragments)

    def __lt__(self, other):
        """Returns True if this instance is less than another Value."""

        if not isinstance(other, Value):
            raise ValueError()

        return (self - other).sign() < 0

    def sign(self) -> int:
        """Returns 1 if the value is positive, -1 if negative, and 0 if zero."""

        # Normalize internal data.
        self._normalize()

        # Check from the most significant fragment.
        for i in sorted(self.fragments.keys(), reverse=True):
            if self.fragments[i] > 0:
                return 1
            elif self.fragments[i] < 0:
                return -1
        return 0

    def abs(self):
        """Returns the absolute value of this instance."""

        return -self if self.sign() < 0 else self

    def is_zero(self) -> bool:
        """Returns True if the value is zero."""

        return self.sign() == 0

    def _normalize(self):
        """Reorganizes internal data so that every fragment has a value between -9 and +9."""

        keys = sorted(self.fragments.keys(), reverse=False)

        for i in range(len(keys)):
            d = self.fragments[keys[i]]
            if d == 0:
                continue
            sign = 1 if d > 0 else -1
            dval = abs(d)
            offset = 0
            while dval >= 10:
                offset += 1
                j = keys[i] + offset
                dval //= 10
                self.fragments[j] = self.fragments.get(j, 0) + sign * (dval % 10)
            self.fragments[keys[i]] = sign * (abs(d) % 10)


# ===============================================================================
#    Constants
# ===============================================================================

# Epsilon value for this problem.
EPS = Value.parse('+1.000000000e-9')

# String constants.
CORRECT = 'Correct'
INCORRECT = 'Incorrect'


# ===============================================================================
#    Functions
# ===============================================================================

def check_addition(x: Value, y: Value, z: Value) -> bool:
    """Returns True if value z is considered correct for x + y."""

    if (x + y).is_zero():
        return z.is_zero()
    a = (x + y - z).abs()
    return a < EPS and a < EPS * (x + y).abs()


def check_multiplication(x: Value, y: Value, z: Value) -> bool:
    """Returns True if value z is considered correct for x * y."""

    # Compare values without divisions.
    # |x*y-z| / |x*y| < EPS   <=>   |x*y-z| < EPS * |x*y|
    a = (x * y - z).abs()
    return a < EPS and a < EPS * (x * y).abs()


def check_division(x: Value, y: Value, z: Value) -> bool:
    """Returns True if value z is considered correct for x / y."""

    # Compare values without divisions.
    # |(x/y)-z| / |x/y| < EPS   <=>   |x-z*y| < EPS * |x|
    # |(x/y)-z| < EPS           <=>   |x-z*y| < EPS * |y|
    a = (x - z * y).abs()
    return a < EPS * x.abs() and a < EPS * y.abs()


# ===============================================================================
#    Main Logic
# ===============================================================================

x = Value.parse(sys.stdin.readline())
y = Value.parse(sys.stdin.readline())
a = Value.parse(sys.stdin.readline())
b = Value.parse(sys.stdin.readline())
c = Value.parse(sys.stdin.readline())
d = Value.parse(sys.stdin.readline())

print(CORRECT if check_addition(x, y, a) else INCORRECT)
print(CORRECT if check_addition(x, -y, b) else INCORRECT)
print(CORRECT if check_multiplication(x, y, c) else INCORRECT)
print(CORRECT if check_division(x, y, d) else INCORRECT)
