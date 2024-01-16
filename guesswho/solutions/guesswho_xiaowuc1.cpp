#include <iostream>
#include <vector>

using namespace std;

int main() {
  int r, c, q;
  cin >> r >> c >> q;
  vector<string> g(r);
  for(auto& x: g) cin >> x;
  vector<pair<int, char>> info(q);
  for(auto& x: info) {
    cin >> x.first >> x.second;
    x.first--;
  }
  vector<int> ret;
  for(int i = 0; i < r; i++) {
    bool valid = true;
    for(auto x: info) {
      valid = valid && g[i][x.first] == x.second;
    }
    if(valid) ret.push_back(i+1);
  }
  if(ret.size() == 0) cout << "impossible\n";
  else if(ret.size() > 1) cout << "ambiguous\n" << ret.size() << "\n";
  else cout << "unique\n" << ret[0] << "\n";
}