#include <bits/stdc++.h>

using namespace std;

int main() {
  int N, R;
  cin >> N >> R;
  vector<int> pts(N), ans;
  for (int& x : pts) cin >> x;

  sort(pts.begin(), pts.end());

  while (pts.size()) {
    int best = -1, bx;

    for (int il = 0, ir = 0; ir < pts.size(); ++ir) {
      while (pts[il] + 2*R < pts[ir]) ++il;
      if (ir-il > best) {
        best = ir-il;
        bx = pts[ir]-R;
      }
    }

    ans.push_back(bx);
    vector<int> nxt;

    for (int x : pts) if (abs(x-bx) > R) nxt.push_back(x);
    pts = nxt;
  }

  cout << ans.size() << endl;
  for (int i = 0; i < ans.size(); ++i) cout << ans[i] << (i+1 == ans.size() ? '\n' : ' ');

  return 0;
}