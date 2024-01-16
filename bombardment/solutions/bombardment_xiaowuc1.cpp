#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  int n, r;
  cin >> n >> r;
  r *= 2;
  vector<int> v(n);
  for(auto& x: v) cin >> x;
  sort(v.begin(), v.end());
  vector<int> ret;
  while(v.size()) {
    vector<int> nv;
    int lhs = 0;
    array<int, 2> best = {0, 0};
    for(int i = 1; i < v.size(); i++) {
      while(v[i] - v[lhs] > r) lhs++;
      if(i-lhs > best[1]-best[0]) {
        best[0] = lhs;
        best[1] = i;
      }
    }
    ret.push_back(v[best[1]] - r/2);
    for(int a = 0; a < v.size(); a++) if(a < best[0] || a > best[1]) nv.push_back(v[a]);
    v.swap(nv);
  }
  cout << ret.size() << "\n";
  for(int i = 0; i < ret.size(); i++) cout << ret[i] << " \n"[i+1 == ret.size()];
}