// clang-format off
#include <bits/stdc++.h>
using namespace std;
#define all(a) (a).begin(), (a).end()
#define len(a) int((a).size())
#define nxt(type) ({type x;cin>>x;x;})
#define nxtline() ({std::string s;while(s.empty())std::getline(std::cin,s);s;})
#define contains(c,x) ((c).find(x)!=(c).end())
#define Min(x,a) (x)=min((x),(a))
#define Max(x,a) (x)=max((x),(a))
#define forn(i,n) for (int (i)=0;(i)<(n);++(i))
#define fori(i,s,t) for (int (i)=s;(i)<=(t);++(i))
typedef long long ll;
// clang-format on

/**
 * Segment tree implementation for monoids.
 */
template <typename Monoid>
class SegmentTree {
 public:
  size_t n;  // number of elements
  size_t m;  // number of leaves of the perfect binary tree
  Monoid monoid;
  typedef decltype(Monoid().ident) T;
  std::vector<T> data_;

 public:
  /**
   * Initializes the segmentree of the given size for a specific monoid.
   */
  SegmentTree(size_t n) : n(n) {  //}, T identity_element, Func operation)
    // : n(n), identity_element(identity_element), operation(operation) {
    if (n == 0) throw std::invalid_argument("n cannot be zero");
    if (n > 1000000000U) throw std::invalid_argument("n cannot be too large");
    m = n == 1 ? 1 : (1U << (32 - __builtin_clz(n - 1)));  // 2-base ceiling
    data_.resize(2 * m, monoid.ident);
  }

  /**
   * @brief Updates one element at the given index. Time complexity: O(log n).
   *
   * @param index target index
   * @param value new value
   */
  void update(size_t index, T value) {
    auto k = m + index;
    data_[k] = value;
    while (k > 1) {
      k /= 2;
      data_[k] = monoid(data_[k * 2], data_[k * 2 + 1]);
    }
  }

  T query(size_t a, size_t b) const { return _query(a, b, 1, 0, m); }  // root index: 1

 private:
  T _query(size_t a, size_t b, size_t k, size_t l, size_t r) const {
    if (r <= a || b <= l) return data_[0];             // outside query range; return identity element
    if (a <= l && r <= b) return data_[k];             // inside query range
    auto u = _query(a, b, k * 2, l, (l + r) / 2);      // left child
    auto v = _query(a, b, k * 2 + 1, (l + r) / 2, r);  // right child
    return monoid(u, v);
  }
};

ll const M = 1000000007;

// define monoids
template <typename T>
struct Max {
  T ident = -1;
  T operator()(T a, T b) const { return std::max(a, b); }
};

template <typename T>
struct Min {
  T ident = 2000000000;
  T operator()(T a, T b) const { return std::min(a, b); }
};

template <typename T>
struct Mul {
  T ident = 1;
  T operator()(T a, T b) const { return a * b % M; }
};

int n, m, nop;
vector<ll> xs;
vector<char> ops;

int find_left_boundary(SegmentTree<Max<ll>> &tree, int index, char target) {
  if (index == 0 || ops[index - 1] != target) return index;

  int pos = (int)tree.query(0, index);
  return pos < 0 ? 0: pos + 1;
}

int find_right_boundary(SegmentTree<Min<ll>> &tree, int index, char target) {
  if (index == nop || ops[index] != target) return index;

  int pos = (int) tree.query(index, nop);
  return pos >= nop ? nop : pos;
}

std::pair<ll, ll> compute_total(vector<ll> const& xs, vector<char> const& ops) {
  auto n = xs.size();
  ll total[2] = {};
  ll sofar[] = {xs[0], xs[0]};

  for (size_t i = 0; i < xs.size(); ++i) {
    for (int phase = 0; phase < 2; ++phase) {
      if (i == n - 1) {
        total[phase] = (total[phase] + sofar[phase]) % M;
        continue;
      }
      if ((ops[i] == '+') == (phase == 0)) {
        total[phase] = (total[phase] + sofar[phase]) % M;
        sofar[phase] = 1;
      }
      sofar[phase] = (sofar[phase] * xs[i + 1]) % M;
    }
  }
  return {total[0], total[1]};
}

int main() {
  // Parse input.
  n = nxt(int);
  m = nxt(int);
  nop = n - 1;  // number of operators

  auto initial_condition = nxt(string);
  for (size_t i = 0; i < initial_condition.size(); ++i) {
    if (i % 2 == 0) {
      xs.push_back(initial_condition[i] - '0');
    } else {
      ops.push_back(initial_condition[i]);
    }
  }

  // Create segment trees.
  auto maxplustree = SegmentTree<Max<ll>>(nop);
  auto minplustree = SegmentTree<Min<ll>>(nop);
  auto maxmultree = SegmentTree<Max<ll>>(nop);
  auto minmultree = SegmentTree<Min<ll>>(nop);
  auto multree = SegmentTree<Mul<ll>>(n);

  for (int i = 0; i < nop; ++i) {
    maxplustree.update(i, ops[i] == '+' ? i : -1);
    minplustree.update(i, ops[i] == '+' ? i : nop);
    maxmultree.update(i, ops[i] == '+' ? -1 : i);
    minmultree.update(i, ops[i] == '+' ? nop : i);
  }
  for (int i = 0; i < n; ++i) multree.update(i, xs[i]);

  // Compute initial totals.
  auto initial_total = compute_total(xs, ops);

  int current_phase = 0;
  ll total[2] = {initial_total.first, initial_total.second};

  printf("%lld\n", total[current_phase]);

  for (int q = 0; q < m; ++q) {
    auto op = nxt(char);
    switch (op) {
      case 'a': { // All Flip
        current_phase ^= 1;
        break;
      }
      case 'f': {  // Flip
        int op_idx = nxt(int) - 1;

        for (int phase = 0; phase < 2; ++phase) {
          auto mul_start = find_left_boundary(phase == 0 ? maxplustree : maxmultree, op_idx, phase == 0 ? '*' : '+');
          auto mul_end = find_right_boundary(phase == 0 ? minplustree : minmultree, op_idx + 1, phase == 0 ? '*' : '+');

          auto left_mul = multree.query(mul_start, op_idx + 1);
          auto right_mul = multree.query(op_idx + 1, mul_end + 1);

          // Update total values.
          auto sign = ((ops[op_idx] == '+') == (phase == 0)) ? 1 : -1;
          auto diff = (sign * ((left_mul * right_mul - left_mul - right_mul) % M) + M) % M;
          total[phase] = (total[phase] + diff) % M;
        }

        // Update segment trees;
        if (ops[op_idx] == '+') {
          minmultree.update(op_idx, op_idx);
          maxmultree.update(op_idx, op_idx);
          minplustree.update(op_idx, nop);
          maxplustree.update(op_idx, -1);
        } else {
          minmultree.update(op_idx, nop);
          maxmultree.update(op_idx, -1);
          minplustree.update(op_idx, op_idx);
          maxplustree.update(op_idx, op_idx);
        }

        // Update vectors.
        ops[op_idx] = ops[op_idx] == '+' ? '*' : '+';
        break;
      }
      case 's': { // Swap
        int i = nxt(int) - 1;
        int j = nxt(int) - 1;
        if (xs[i] == xs[j]) break;

        if (i > j) std::swap(i, j);
        ll t[8] = {};

        for (int phase = 0; phase < 2; ++phase) {
          for (int k = 0; k < 2; ++k) {
            int num_idx = k == 0 ? i : j;
            t[phase * 4 + k * 2] = find_left_boundary(phase == 0 ? maxplustree : maxmultree, num_idx, phase == 0 ? '*' : '+');
            t[phase * 4 + k * 2 + 1] = find_right_boundary(phase == 0 ? minplustree : minmultree, num_idx, phase == 0 ? '*' : '+');

            // Decrease total values;
            total[phase] = (total[phase] - multree.query(t[phase * 4 + k * 2], t[phase * 4 + k * 2 + 1] + 1) + M) % M;
          }
        }

        // Update segment trees;
        multree.update(i, xs[j]);
        multree.update(j, xs[i]);

        // Increase total values;
        for (int phase = 0; phase < 2; ++phase) {
          for (int k = 0; k < 2; ++k) {
            total[phase] = (total[phase] + multree.query(t[phase * 4 + k * 2], t[phase * 4 + k * 2 + 1] + 1)) % M;
          }
        }

        // Update vectors.
        std::swap(xs[i], xs[j]);
        break;
      }
      default:;
    }
    printf("%lld\n", total[current_phase]);
  }

}
