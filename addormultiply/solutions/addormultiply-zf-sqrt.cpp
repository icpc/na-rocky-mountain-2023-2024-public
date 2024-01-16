#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int B = 450;

const ll MOD = 1'000'000'007;

struct range {
  bool add; // does this range span an addition?

  // add == true: l, r are products of leftmost and rightmost
  // multiplication runs and v is sum of "interior" runs
  // add == false: v is the product of the range and l, r are 0
  ll l, v, r;
};

// given two consecutive ranges joined by operator "op", return the range
// obtained by merging them
range merge(range l, range r, char op) {
  assert (op == '*' || op == '+');
  if (op == '*') {
    if (l.add && r.add) return { true, l.l, (l.v + l.r*r.l + r.v) % MOD, r.r };
    else if (l.add) return { true, l.l, l.v, (l.r * r.v) % MOD };
    else if (r.add) return { true, (l.v * r.l) % MOD, r.v, r.r };
    else return { false, 0, (l.v * r.v) % MOD, 0};
  }
  else {
    if (l.add && r.add) return { true, l.l, (l.v + l.r + r.l + r.v) % MOD, r.r };
    else if (l.add) return { true, l.l, (l.v + l.r) % MOD, r.v };
    else if (r.add) return { true, l.v, (r.l + r.v) % MOD, r.r };
    else return { true, l.v, 0, r.v };
  }
}

range blocks[2][B];
string expr[2];

void update_blocks(int b) {
  for (int i = 0; i < 2; ++i) {
    range& r = blocks[i][b];

    r = {false, 0, 0, 0};
    ll cur = expr[i][2*b*B]-'0';
    for (int j = 1; j < B; ++j) {
      char op = expr[i][2*(b*B+j)-1];
      ll val = expr[i][2*(b*B+j)]-'0';
      if (op == '+') {
        if (!r.add) r = {true, cur, 0, 0};
        else r.v = (r.v + cur)%MOD;
        cur = val;
      }
      else cur = (cur * val) % MOD;
    }
    if (r.add) r.r = cur;
    else r.v = cur;
  }
}

ll evaluate(int i) {
  range val = blocks[i][0];
  for (int b = 1; b < B; ++b) val = merge(val, blocks[i][b], expr[i][2*b*B-1]);
  return (val.l + val.v + val.r)%MOD;
}

int main() {

  int N, M;
  cin >> N >> M;

  assert(B*B > N);

  cin >> expr[0];
  expr[1] = expr[0];
  for (int i = 1; i < expr[0].size(); i += 2) expr[1][i] ^= ('+'^'*');

  for (int i = 0; i < 2; ++i)
    while (expr[i].size() < 2*B*B)
      expr[i].append("+0");

  for (int b = 0; b < B; ++b) update_blocks(b);

  int curr_i = 0;
  cout << evaluate(curr_i) << endl;

  while (M--) {
    char t;
    cin >> t;
    if (t == 's') {
      int i, j;
      cin >> i >> j;
      swap(expr[0][2*i-2], expr[0][2*j-2]);
      swap(expr[1][2*i-2], expr[1][2*j-2]);
      update_blocks((i-1)/B);
      update_blocks((j-1)/B);
    }
    else if (t == 'a') curr_i ^= 1;
    else {
      int i;
      cin >> i;
      swap(expr[0][2*i-1], expr[1][2*i-1]);
      update_blocks((i-1)/B);
    }
    cout << evaluate(curr_i) << endl;
  }

  return 0;
}