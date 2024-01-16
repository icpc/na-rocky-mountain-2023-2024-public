#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

template<class Fun>
class y_combinator_result {
  Fun fun_;
public:
  template<class T>
  explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

  template<class ...Args>
  decltype(auto) operator()(Args &&...args) {
    return fun_(std::ref(*this), std::forward<Args>(args)...);
  }
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
  return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  string s;
  cin >> n >> s;
  vector<vector<array<array<int, 2>, 2>>> dp(n);
  auto getcharouter = [&](int idx) -> char {
    if(idx < 0 || idx >= s.size()) return '.';
    return s[idx];
  };
  auto dfs = y_combinator([&](auto self, int start, int sz, int lhs, int rhs) -> int {
    if(sz <= 1) return 0;
    if(dp[start][sz][lhs][rhs] >= 0) return dp[start][sz][lhs][rhs];
    set<int> nimbers;
    auto getchar = [&](int idx) -> char {
      if(idx == start) return lhs ? 'B' : 'W';
      if(idx == start + sz - 1) return rhs ? 'B' : 'W';
      if(idx < 0 || idx >= s.size()) return '.';
      return s[idx];
    };
    for(int src = start; src < start+sz; src++) {
      char srcchar = getchar(src);
      if(srcchar == '.') continue;
      for(int dx = -1; dx <= 1; dx += 2) {
        int dst = src + dx;
        if(dst < start || dst >= start + sz) continue;
        char dstchar = getchar(dst);
        if(srcchar == dstchar || dstchar == '.') continue;
        int nimber = 0;
        if(dx == -1) {
          nimber ^= self(start, src - start, lhs, srcchar == 'B');
          nimber ^= self(src + 1, sz - (src - start) - 1, getchar(src+1) == 'B', rhs);
        }
        else {
          assert(dx == 1);
          nimber ^= self(start, src - start, lhs, getchar(src-1) == 'B');
          nimber ^= self(src+1, sz - (src - start) - 1, srcchar == 'B', rhs);
        }
        nimbers.insert(nimber);
      }
    }
    while(nimbers.count(++dp[start][sz][lhs][rhs]));
    return dp[start][sz][lhs][rhs];   
  });
  for(auto& x: dp) {
    x.resize(n+1);
    for(auto& y: x) {
      for(auto& z: y) {
        z[0] = -1;
        z[1] = -1;
      }
    }
  }
  vector<array<int, 2>> ret;
  vector<int> outernimbers;
  int fullouter = 0;
  for(int i = 0; i < n;) {
    if(s[i] == '.') {
      i++;
      continue;
    }
    int j = i+1;
    while(j < n && s[j] != '.') j++;
    outernimbers.push_back(dfs(i, j-i, s[i] == 'B', s[j-1] == 'B'));
    fullouter ^= outernimbers.back();
    i = j;
  }
  int nimberidx = 0;
  for(int i = 0; i < n;) {
    if(s[i] == '.') {
      i++;
      continue;
    }
    int j = i+1;
    while(j < n && s[j] != '.') j++;
    int goal = fullouter ^ outernimbers[nimberidx++];
    int sz = j-i;
    int start = i;
    for(int src = i; src < j; src++) {
      char srcchar = s[src];
      for(int dx = -1; dx <= 1; dx += 2) {
        int dst = src + dx;
        if(dst < i || dst >= j || s[src] == s[dst]) continue;
        int nimber = 0;
        if(dx == -1) {
          nimber ^= dfs(start, src - start, s[i] == 'B', srcchar == 'B');
          nimber ^= dfs(src + 1, sz - (src - start) - 1, getcharouter(src+1) == 'B', s[j-1] == 'B');
        }
        else {
          assert(dx == 1);
          nimber ^= dfs(start, src - start, s[i] == 'B', getcharouter(src-1) == 'B');
          nimber ^= dfs(src+1, sz - (src - start) - 1, srcchar == 'B', s[j-1] == 'B');
        }
        if(goal == nimber) ret.push_back({src+1, dst+1});
      }
    }
    i = j;
  }
  cout << ret.size() << "\n";
  for(auto [a, b]: ret) cout << a << " " << b << "\n";
}