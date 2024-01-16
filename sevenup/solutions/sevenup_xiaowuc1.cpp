#include <array>
#include <cassert>
#include <functional>
#include <iomanip>
#include <iostream>
#include <unordered_map>
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
  vector<int> to(7, 7);
  array<int, 8> SCALE = {5, 5, 5, 5, 5, 5, 5, 25};
  array<int, 8> start = {4, 4, 4, 4, 4, 4, 4, 24};
  {
    string s;
    cin >> s;
    for(int i = 0; i < 7; i++) {
      if(s[i] == 'A') to[i] = 0;
      else if(s[i] >= '2' && s[i] <= '7') to[i] = s[i]-'1';
      start[to[i]]--;
      SCALE[to[i]]--;
    }
  }
  vector<unordered_map<int, double>> dp(127);
  auto dfs = y_combinator([&](auto self, int curr, int have) -> double {
    if(curr == 127) return 0;
    if(dp[curr].count(have)) return dp[curr][have];
    array<int, 8> materialize;
    int tot = 0;
    {
      int now = have;
      for(int i = 7; i >= 0; i--) {
        materialize[i] = now % SCALE[i];
        now /= SCALE[i];
        tot += materialize[i];
      }
    }
    {
      int orig = 0;
      for(int i = 0; i < 8; i++) {
        orig *= SCALE[i];
        orig += materialize[i];
      }
      assert(orig == have);
    }
    dp[curr][have] = 0;
    for(int i = 0; i < 8; i++) {
      if(materialize[i] == 0) continue;
      int ncurr = curr;
      materialize[i]--;
      {
        int x = i;
        while(x >= 0 && x < 7 && (ncurr&(1<<x)) == 0) {
          ncurr |= 1 << x;
          x = to[x];
        }
      }
      int nhave = 0;
      for(int j = 0; j < 8; j++) {
        nhave *= SCALE[j];
        nhave += materialize[j];
      }
      dp[curr][have] += self(ncurr, nhave) * (materialize[i] + 1.) / tot;
      materialize[i]++;
    }
    return ++dp[curr][have];
  });
  int src = 0;
  for(int i = 0; i < 8; i++) {
    src *= SCALE[i];
    src += start[i];
  }
  cout << fixed << setprecision(39) << dfs(0, src) << "\n";
}