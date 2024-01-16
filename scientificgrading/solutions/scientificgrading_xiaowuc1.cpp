#include <cassert>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct num {
  map<int, int> dp;
  void reduce() {
    int rhs = 2.1e9;
    while(dp.size()) {
      auto it = dp.lower_bound(rhs);
      if(it == dp.begin()) break;
      it--;
      int k = it->first;
      int v = it->second;
      if(abs(v) >= 10) {
        dp[k+1] += v/10;
        dp[k] %= 10;
        if(dp[k] == 0) dp.erase(k);
        rhs = k+2;
      }
      else {
        if(v == 0) dp.erase(k);
        rhs = k;
      }
    }
    rhs = 2.1e9;
    while(dp.size()) {
      auto it = dp.lower_bound(rhs);
      if(it == dp.begin()) break;
      it--;
      int k = it->first;
      int v = it->second;
      if(v == 1 && dp.count(k-1) && dp[k-1] < 0) {
        dp.erase(k);
        dp[k-1] += 10;
      }
      else if(v == -1 && dp.count(k+1) && dp[k+1] > 0) {
        dp.erase(k);
        dp[k-1] -= 10;
      }
      rhs = k;
    }
  }
  num() {}
  num(map<int, int> ndp) {
    dp = ndp;
    reduce();
  }
  num add(const num& other) {
    map<int, int> ret;
    for(auto [k, v]: dp) ret[k] += v;
    for(auto [k, v]: other.dp) ret[k] += v;
    return num(ret);
  }
  num sub(const num& other) {
    map<int, int> ret;
    for(auto [k, v]: dp) ret[k] += v;
    for(auto [k, v]: other.dp) ret[k] -= v;
    return num(ret);
  }
  num mul(const num& other) {
    map<int, int> ret;
    for(auto [k, v]: dp) {
      for(auto [kk, vv]: other.dp) {
        ret[k+kk] += v * vv;
      }
    }
    return num(ret);
  }
  num absv() {
    if(dp.size() == 0) return num(dp);
    map<int, int> ret = dp;
    int x = dp.rbegin()->first;
    assert(dp[x] != 0);
    bool flip = dp[x] < 0;
    if(flip) {
      for(auto& [k, v]: ret) v *= -1;
    }
    return num(ret);
  }
  bool iszero() {
    return dp.size() == 0;
  }
  bool ispos() {
    return dp.size() && dp.rbegin()->second > 0;
  }
};

num getnum(string s) {
  bool neg = s[0] == '-'; s.erase(s.begin());
  vector<int> v;
  while(s[0] != 'e') {
    if(s[0] >= '0' && s[0] <= '9') v.push_back((neg ? -1 : 1) * (s[0] - '0'));
    s.erase(s.begin());
  }
  s.erase(s.begin());
  int p = stoi(s);
  map<int, int> dp;
  for(int i = 0; i < (int)v.size(); i++) {
    dp[p - i] = v[i];
  }
  return num(dp);
}

num getnum() {
  string s;
  cin >> s;
  return getnum(s);
}

num EPS = getnum("+1e-9");

bool check(num expect, num have) {
  if(expect.iszero()) return have.iszero();
  num abserror = (expect.sub(have)).absv();
  if(abserror.iszero()) return true;
  num cand = EPS.sub(abserror);
  if(!cand.ispos()) return false;
  num rhs = expect.absv().mul(EPS);
  num fin = rhs.sub(abserror);
  return fin.ispos();
}

const string AC = "Correct\n";
const string WA = "Incorrect\n";

int main() {
  num a = getnum();
  num b = getnum();
  num c = getnum();
  num d = getnum();
  num e = getnum();
  num f = getnum();
  if(check(a.add(b), c)) cout << AC; else cout << WA;
  if(check(a.sub(b), d)) cout << AC; else cout << WA;
  if(check(a.mul(b), e)) cout << AC; else cout << WA;
  {
    num cand = (a.sub(f.mul(b))).absv();
    if(EPS.mul(a.absv()).sub(cand).ispos() && EPS.mul(b.absv()).sub(cand).ispos()) cout << AC; else cout << WA;
  }
}