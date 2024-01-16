#include <array>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  int n;
  cin >> n;
  vector<vector<array<int, 2>>> avail(n);
  for(int i = 0; i < n; i++) {
    int k;
    cin >> k;
    avail[i].resize(k);
    for(auto& [a, b]: avail[i]) cin >> a >> b;
  }
  int best = 0;
  int amt = 0;
  for(int t = 0; t < 86400; t++) {
    int numavail = 0;
    for(int i = 0; i < n; i++) {
      bool isavail = false;
      for(auto [a, b]: avail[i]) isavail = isavail || (a <= t && t <= b);
      numavail += isavail;
    }
    if(numavail > best) {
      best = numavail;
      amt = 0;
    }
    amt += best == numavail;
  }
  cout << best << "\n";
  cout << amt << "\n";
}