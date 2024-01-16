#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
bool dEqual(ll x, ll y) { return x == y; }

struct Point {
  ll x, y;
  bool operator==(const Point &p) const { return dEqual(x, p.x) && dEqual(y, p.y); }
  bool operator<(const Point &p) const { return y < p.y || (y == p.y && x < p.x); }
};

Point operator-(Point p,Point q){ p.x -= q.x; p.y -= q.y; return p; }
Point operator+(Point p,Point q){ p.x += q.x; p.y += q.y; return p; }
Point operator*(ll r,Point p){ p.x *= r; p.y *= r; return p; }
Point operator/(Point p, ll r){ p.x /= r; p.y /= r; return p; }
ll operator*(Point p,Point q){ return p.x*q.x + p.y*q.y; }
ll len2(Point p){ return p*p; }
ll cross(Point p,Point q){ return p.x*q.y - q.x*p.y; }
Point inv(Point p){ Point q = {-p.y,p.x}; return q; }

enum Orientation {CCW, CW, CNEITHER};

//------------------------------------------------------------------------------
// Colinearity test
bool colinear(Point a, Point b, Point c) { return dEqual(cross(b-a,c-b),0); }

bool on_segment(Point p, Point a, Point b)
{
  return colinear(a, p, b) && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x)
    && min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

//------------------------------------------------------------------------------
// Orientation test  (When pts are colinear: ccw: a-b-c  cw: c-a-b  neither: a-c-b)
Orientation ccw(Point a, Point b, Point c) { // 
  Point d1 = b - a, d2 = c - b;
  if (dEqual(cross(d1,d2),0))
    if (d1.x * d2.x < 0 || d1.y * d2.y < 0) 
      return (d1 * d1 >= d2*d2) ? CNEITHER : CW;
    else return CCW;
  else return (cross(d1,d2) > 0) ? CCW : CW;
}

//------------------------------------------------------------------------------
// Signed Area of Polygon
ll area_polygon2(Point p[], int n) {
  ll sum = 0;
  for (int i = 0; i < n; i++)  sum += cross(p[i],p[(i+1)%n]);
  return sum;
}

const int MAX_N = 100000;
Point polygon[MAX_N];

int main()
{
  int N;
  cin >> N;

  for (int i = 0; i < N; i++) {
    cin >> polygon[i].x >> polygon[i].y;
    polygon[i] = 2 * polygon[i];
  }

  // make CCW
  double area = area_polygon2(polygon, N);
  if (area < 0) {
    reverse(polygon, polygon+N);
  }
  
  Point bug, signal;
  cin >> bug.x >> bug.y;
  cin >> signal.x >> signal.y;

  bug = 2*bug;
  signal = 2*signal;
  
  bool inside = false, outside = false;
  for (int i = 0; i < N; i++) {
    Point p = polygon[i], q = polygon[(i+1)%N];
    Point bug2 = bug;

    if (bug2 == p || bug2 == q) {
      bug2 = (p + q) / 2;
    }
    if (!on_segment(bug2, p, q)) continue;

    auto orient = ccw(p, bug2, signal);
    inside |= (orient == CCW || colinear(p, bug2, signal));
    outside |= (orient == CW || colinear(p, bug2, signal));
  }

  if (inside == outside) {
    cout << "?" << endl;
  } else if (inside) {
    cout << "inside" << endl;
  } else {
    cout << "outside" << endl;
  }
  
  return 0;
}
