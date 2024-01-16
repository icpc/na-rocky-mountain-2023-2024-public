#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

const int MOD = int(1e9)+7;

int64_t get(int64_t x) {
  x %= MOD;
  if(x < 0) x += MOD;
  return x;
}

struct state {
  int64_t totalsum;
  int64_t leftsum;
  int64_t rightsum;
  bool product;
  int nterms;
  state() {}
  state(int64_t a, int64_t b, int64_t c, bool d, int e) {
    totalsum = a;
    leftsum = b;
    rightsum = c;
    product = d;
    nterms = e;
  }
};
state merge(const state& lhs, const state& rhs) {
  state ret;
  ret.product = rhs.product;
  ret.nterms = lhs.nterms + rhs.nterms - lhs.product;
  if(ret.nterms == 1) {
    ret.leftsum = ret.rightsum = ret.totalsum = get(lhs.totalsum * rhs.totalsum);
  }
  else if(lhs.product) {
    if(lhs.nterms == 1) {
      ret.leftsum = get(lhs.totalsum * rhs.leftsum);
      ret.rightsum = rhs.rightsum;
      ret.totalsum = get(lhs.totalsum * rhs.leftsum + rhs.totalsum - rhs.leftsum);
    }
    else if(rhs.nterms == 1) {
      ret.leftsum = lhs.leftsum;
      ret.rightsum = get(lhs.rightsum * rhs.totalsum);
      ret.totalsum = get(rhs.totalsum * lhs.rightsum + lhs.totalsum - lhs.rightsum);
    }
    else {
      ret.leftsum = lhs.leftsum;
      ret.rightsum = rhs.rightsum;
      ret.totalsum = get(lhs.totalsum - lhs.rightsum + rhs.totalsum - rhs.leftsum + lhs.rightsum * rhs.leftsum);
    }
  }
  else {
    ret.leftsum = lhs.leftsum;
    ret.rightsum = rhs.rightsum;
    ret.totalsum = get(lhs.totalsum + rhs.totalsum);
  }
  return ret;
}
struct seg {
  vector<state> tree;
  int SZ;
  seg(int n) {
    SZ = 1;
    while(SZ < n) SZ *= 2;
    tree.resize(2*SZ);
    for(int i = SZ; i < 2*SZ; i++) tree[i] = state(0, 0, 0, false, 1);
    for(int i = SZ-1; i > 0; i--) tree[i] = merge(tree[2*i], tree[2*i+1]);
  }
  void setprod(int idx, bool p) {
    idx += SZ;
    tree[idx].product = p;
    while(idx > 1) {
      idx /= 2;
      tree[idx] = merge(tree[2*idx], tree[2*idx+1]);
    }
  }
  void setval(int idx, int v) {
    idx += SZ;
    tree[idx].totalsum = tree[idx].leftsum = tree[idx].rightsum = v;
    while(idx > 1) {
      idx /= 2;
      tree[idx] = merge(tree[2*idx], tree[2*idx+1]);
    }
  }
};

int main() {
  ios_base::sync_with_stdio(false); cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  string s;
  cin >> s;
  vector<seg> segs(2, seg(n));
  int curr = 0;
  for(int i = 0; i < n; i++) {
    segs[0].setval(i, s[2*i] - '0');
    segs[1].setval(i, s[2*i] - '0');
    if(i < n-1) {
      segs[0].setprod(i, s[2*i+1] == '*');
      segs[1].setprod(i, s[2*i+1] != '*');
    }
  }
  cout << segs[curr].tree[1].totalsum << "\n";
  while(q--) {
    char op;
    cin >> op;
    if(op == 's') {
      int lhs, rhs;
      cin >> lhs >> rhs;
      lhs--; rhs--;
      int lv = segs[0].tree[segs[0].SZ + lhs].totalsum;
      int rv = segs[0].tree[segs[1].SZ + rhs].totalsum;
      segs[0].setval(lhs, rv); segs[0].setval(rhs, lv);
      segs[1].setval(rhs, lv); segs[1].setval(lhs, rv);
    }
    else if(op == 'f') {
      int lhs;
      cin >> lhs; lhs--;
      bool p = segs[0].tree[segs[0].SZ + lhs].product;
      segs[0].setprod(lhs, !p);
      segs[1].setprod(lhs, p);
    }
    else {
      assert(op == 'a');
      curr ^= 1;
    }
    cout << segs[curr].tree[1].totalsum << "\n";
  }
}