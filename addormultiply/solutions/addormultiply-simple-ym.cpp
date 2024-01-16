// clang-format off
#include <bits/stdc++.h>
using namespace std;
#define nxt(type) ({type x;cin>>x;x;})
typedef long long ll;
// clang-format on

ll const M = 1000000007;

/**
 * Segment tree implementation for monoids.
 */
struct Expression {
  /*
  Two types of expressions:
  Type I : *z (z is nonzero) -> represented as {-1, -1, z}
  Type II: *x +y +z

  Merging expressions:
  -       *c ::       *z =                 *(cz)
  -       *c :: *x +y +z = *(cx) +y        +z
  - *a +b +c ::       *z = *a    +b        +(cz)
  - *a +b +c :: *x +y +z = *a    +(b+cx+y) +z
  */
  ll x, y, z;
  Expression(ll z): x(-1), y(-1), z(z) {}
  Expression(ll x, ll y, ll z): x(x), y(y), z(z) {}
};

class CompTree {
 public:
  size_t n;  // number of elements
  size_t m;  // number of leaves of the perfect binary tree
  Expression ident = Expression(1);
  std::vector<Expression> data_;

 public:
  CompTree(size_t n) : n(n) {
    if (n == 0) throw std::invalid_argument("n cannot be zero");
    if (n > 1000000000U) throw std::invalid_argument("n cannot be too large");
    m = n == 1 ? 1 : (1U << (32 - __builtin_clz(n - 1)));  // 2-base ceiling
    data_.resize(2 * m, ident);
  }

  Expression merge(Expression const& a, Expression const& b) const {
    switch ((a.x >= 0) * 2 + (b.x >= 0)) {
      case 0: return Expression(a.z * b.z % M);
      case 1: return Expression(a.z * b.x % M, b.y, b.z);
      case 2: return Expression(a.x, a.y, a.z * b.z % M);
      case 3: return Expression(a.x, (a.y + a.z * b.x + b.y) % M, b.z);
      default: assert(false); return ident;  // never happens
    }
  }

  void update(size_t index, int d) {
    auto k = m + index;
    data_[k] = d > 0 ? Expression(1, 0, d) : Expression(-d);
    while (k > 1) {
      k /= 2;
      data_[k] = merge(data_[k * 2], data_[k * 2 + 1]);
    }
  }

  // data_[1].x must be 1
  int total() { return (data_[1].y + data_[1].z) % M; }
};

int n, m;
vector<ll> xs;

int main() {
  // Parse input.
  n = nxt(int);
  m = nxt(int);

  auto initial_condition = nxt(string);
  for (size_t i = 0; i < initial_condition.size(); i += 2) {
    ll sign = (i == 0 || initial_condition[i - 1] == '+') ? 1 : -1;
    xs.push_back(sign * (initial_condition[i] - '0'));
  }

  // Create binary trees.
  CompTree ts[] = {CompTree(n), CompTree(n)};

  for (int i = 0; i < n; ++i) {
    ts[0].update(i, xs[i]);
    ts[1].update(i, i == 0 ? xs[i] : -xs[i]);
  }

  // Compute initial totals.
  int current_phase = 0;

  printf("%d\n", ts[current_phase].total());

  for (int q = 0; q < m; ++q) {
    auto op = nxt(char);
    switch (op) {
      case 'a': { // All Flip
        current_phase ^= 1;
        break;
      }
      case 'f': {  // Flip
        int op_idx = nxt(int);
        assert(op_idx > 0);
        xs[op_idx] *= -1;
        ts[0].update(op_idx, xs[op_idx]);
        ts[1].update(op_idx, -xs[op_idx]);
        break;
      }
      case 's': { // Swap
        int i = nxt(int) - 1;
        int j = nxt(int) - 1;
        if (xs[i] == xs[j]) break;

        auto x = xs[i];
        xs[i] = ((x > 0) == (xs[j] > 0)) ? xs[j] : -xs[j];
        xs[j] = ((x > 0) == (xs[j] > 0)) ? x : -x;
        ts[0].update(i, xs[i]);
        ts[0].update(j, xs[j]);
        ts[1].update(i, i == 0 ? xs[i] : -xs[i]);
        ts[1].update(j, j == 0 ? xs[j] : -xs[j]);
        break;
      }
      default:;
    }
    printf("%d\n", ts[current_phase].total());
  }
}
