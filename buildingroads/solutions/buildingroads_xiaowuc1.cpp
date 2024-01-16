#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<array<int, 2>> v(n);
  for(auto& x: v) cin >> x[0] >> x[1];
  vector<vector<double>> dist(n);
  for(int i = 0; i < n; i++) {
    dist[i].resize(n);
    for(int j = 0; j < n; j++) dist[i][j] = hypot(v[i][0] - v[j][0], v[i][1] - v[j][1]);
  }
  vector<vector<double>> edgew = dist;
  for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) for(int k = 0; k < n; k++) dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
  for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) for(int k = 0; k < n; k++) dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
  for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) for(int k = 0; k < n; k++) dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
  double ret = 1e9;
  for(int i = 0; i < n; i++) ret = min(ret, *max_element(dist[i].begin(), dist[i].end()));
  for(int i = 0; i < n; i++) for(int j = i+1; j < n; j++) {
    auto eval = [&](double t) -> double {
      double ret = 0;
      for(int k = 0; k < n; k++) {
        ret = max(ret, min(t + dist[i][k], edgew[i][j] - t + dist[j][k]));
      }
      return ret;
    };
    double lhs = 0;
    double rhs = edgew[i][j];
    for(int q = 0; q < 30; q++) {
      double a = (2*lhs+rhs)/3;
      double b = (2*rhs+lhs)/3;
      double av = eval(a);
      double bv = eval(b);
      if(av < bv) {
        ret = min(ret, av);
        rhs = b;
      }
      else {
        ret = min(ret, bv);
        lhs = a;
      }
    }
  }
  cout << fixed << setprecision(39) << (2*ret) << "\n";
}