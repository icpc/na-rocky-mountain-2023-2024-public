// clang-format off
#include <bits/stdc++.h>
using namespace std;
#define _1 first
#define _2 second
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

ll M = 1000000000LL;
int sign(ll x) { return x == 0 ? 0 : x > 0 ? 1 : -1; }

struct Val {
  std::vector<std::pair<ll, ll>> ts;

  Val(std::string const& s) {
    auto a = std::stoll(s.substr(13));
    ts.push_back({a, std::stoll(s.substr(0, 2))});
    ts.push_back({a - 9, std::stoll(s.substr(0, 1) + s.substr(3, 9))});
  }

  Val(vector<pair<ll, ll>> const& ws): ts(ws) {}
};

int sign(Val const& v) {
  auto ws = v.ts;
  sort(ws.rbegin(), ws.rend());

  int i = 0;
  ll remainder = 0;
  for (int i = 0; i < ws.size(); ++i) {
    assert(abs(ws[i]._2) < M);

    int gap = i == 0 ? 0 : ws[i - 1]._1 - ws[i]._1;

    while (remainder != 0 && gap-- > 0) {
      remainder *= 10;
      if (abs(remainder) > 10 * M) return sign(remainder);
    }
    remainder += ws[i]._2;
  }
  return sign(remainder);
}

Val operator+(Val const& a, Val const& b) {
  vector<pair<ll, ll>> ws = a.ts;
  for (auto &p: b.ts) ws.push_back(p);
  return Val(ws);
}

Val operator*(Val const& a, Val const& b) {
  vector<pair<ll, ll>> ws;
  for (auto &p: a.ts) {
    for (auto &q: b.ts) {
      auto x = p._2 * q._2;
      if (abs(x) >= M) ws.push_back({p._1 + q._1 + 9, x / M});
      ws.push_back({p._1 + q._1, sign(x) * (abs(x) % M)});
    }
  }
  return Val(ws);
}

Val operator-(Val const& v) { return v * Val("-1.000000000e+0"); } // negation
Val operator-(Val const& a, Val const& b) { return a + (-b); }
bool operator<(Val const& a, Val const& b) { return sign(a - b) < 0; } 
Val abs(Val const& v) { return sign(v) < 0 ? -v : v; }
Val min(Val const& a, Val const& b) { return a < b ? a : b; }


int main() {
  auto eps = Val("+1.000000000e-9");
  auto one = Val("+1.000000000e+0");
  auto x = Val(nxt(string));
  auto y = Val(nxt(string));
  auto a = Val(nxt(string));
  auto b = Val(nxt(string));
  auto c = Val(nxt(string));
  auto d = Val(nxt(string));

  if (sign(x + y - a) == 0 || abs(x + y - a) < eps * min(one, abs(x + y))) {
    printf("Correct\n");
  } else {
    printf("Incorrect\n");
  }

  if (sign(x - y - b) == 0 || abs(x - y - b) < eps * min(one, abs(x - y))) {
    printf("Correct\n");
  } else {
    printf("Incorrect\n");
  }

  if (abs(x * y - c) < eps * min(one, abs(x * y))) {
    printf("Correct\n");
  } else {
    printf("Incorrect\n");
  }

  if (abs(x - y * d) < eps * min(abs(x), abs(y))) {
    printf("Correct\n");
  } else {
    printf("Incorrect\n");
  }
}
