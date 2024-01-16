#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using point = complex<ll>;

ll dot(point p, point q) { return real(conj(p)*q); }
ll cross(point p, point q) { return imag(conj(p)*q); }
ll sgn(ll z) { return z ? (z > 0 ? 1 : -1) : 0; }

ll area2(const vector<point>& p) {
  ll a = 0;
  for (int i = 0, j = p.size()-1; i < p.size(); j = i++) a += cross(p[j], p[i]);
  return a;
}

point readpt() {
  ll x, y;
  cin >> x >> y;
  return {x, y};
}

int main() {
  int N;
  cin >> N;

  vector<point> p(N);
  for (point& q : p) q = readpt();
  point ch = readpt(), ray = readpt();

  if (area2(p) < 0) reverse(p.begin(), p.end());

  vector<ll> ro;
  for (int i = 0, j = N-1; i < N; j = i++)
    if (!cross(p[j]-ch, p[i]-ch) && dot(p[j]-ch, p[i]-ch) <= 0)
      ro.push_back(sgn(cross(p[j]-ray, p[i]-ray)));
    
  cout << (ro[0]*ro.back() <= 0 ? "?" : (ro[0] > 0 ? "inside" : "outside")) << endl;

  return 0;
}
