#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

template<class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x=0, T y=0) : x(x), y(y) {}
	bool operator<(P p) const { return tie(x,y) < tie(p.x,p.y); }
	bool operator==(P p) const { return tie(x,y)==tie(p.x,p.y); }
	P operator+(P p) const { return P(x+p.x, y+p.y); }
	P operator-(P p) const { return P(x-p.x, y-p.y); }
	P operator*(T d) const { return P(x*d, y*d); }
	P operator/(T d) const { return P(x/d, y/d); }
	T dot(P p) const { return x*p.x + y*p.y; }
	T cross(P p) const { return x*p.y - y*p.x; }
	T cross(P a, P b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	double dist() const { return sqrt((double)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	double angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(double a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
};

typedef Point<int64_t> P;

template<class P>
bool onSegment(const P& s, const P& e, const P& p) {
	P ds = p-s, de = p-e;
	return ds.cross(de) == 0 && ds.dot(de) <= 0;
}
template<class P>
int sideOf(const P& s, const P& e, const P& p) {
	auto a = (e-s).cross(p-s);
	return (a > 0) - (a < 0);
}
void done() {
  cout << "?\n";
  exit(0);
}

int main() {
  int n;
  cin >> n;
  vector<P> poly(n);
  for(auto& p: poly) cin >> p.x >> p.y;
  P src, dst;
  cin >> src.x >> src.y >> dst.x >> dst.y;
  {
    int64_t area = 0;
    for(int i = 0; i < n; i++) {
      int j = (i+1)%n;
      area += poly[i].cross(poly[j]);
    }
    if(area < 0) reverse(poly.begin(), poly.end());
  }
  bool inside = false, outside = false;
  for(int i = 0; i < n; i++) {
    int j = (i+1)%n;
    if(!onSegment(poly[i], poly[j], src)) continue;
    int s = sideOf(poly[i], poly[j], dst);
    if(s == 0) done();
    if(s > 0) inside = true;
    else outside = true;
  }
  if(inside && outside) done();
  assert(inside || outside);
  if(inside) cout << "inside\n";
  else cout << "outside\n";
}