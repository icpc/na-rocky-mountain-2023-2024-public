import java.io.BufferedInputStream;
import java.util.*;
import java.lang.Math;

public class buildingroads_hc {
  static class Point {
    int x, y;

    Point(int x, int y) {
      this.x = x;
      this.y = y;
    }
  }

  private int N;
  private Point[] p;
  
  private int sqdist(int i, int j) {
    int dx = p[i].x - p[j].x;
    int dy = p[i].y - p[j].y;
    return dx*dx + dy*dy;
  }

  private double solve1() {
    double best = Double.MAX_VALUE;
    if (N < 3) {
      return best;
    }

    for (int i = 0; i < N; i++) {
      int max1 = -1, max2 = -1;

      for (int j = 0; j < N; j++) {
	if (i == j) continue;

	int d = sqdist(i, j);
	if (d > max1) {
	  max2 = max1;
	  max1 = d;
	} else if (d > max2) {
	  max2 = d;
	}
      }
      best = Math.min(best, Math.sqrt(max1) + Math.sqrt(max2));
    }
    return best;
  }

  static class Pair implements Comparable<Pair> {
    int d, i;
    
    Pair(int d, int i) {
      this.d = d;
      this.i = i;
    }

    public int getD() {
      return d;
    }

    public int getI() {
      return i;
    }
    
    @Override
    public int compareTo(Pair p) {
      if (this.d != p.d) {
	return this.d - p.d;
      }
      return this.i - p.i;
    }

    @Override
    public String toString() {
        return "(" + this.d + ", " + this.i + ")";
    } 
  }
  
  private double solve2() {
    if (N < 4) {
      return Double.MAX_VALUE;
    }
    Pair[][] L = new Pair[N][N];
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
	L[i][j] = new Pair(sqdist(i, j), j);
      }
      Arrays.sort(L[i]);
    }

    double best_dist = Double.MAX_VALUE;
    for (int i = 0; i < N; i++) {
      for (int j = i+1; j < N; j++) {
	boolean[] coveredi = new boolean[N];
	boolean[] coveredj = new boolean[N];
	for (int k = 0; k < N; k++) {
	  coveredi[k] = false;
	  coveredj[k] = true;
	}

	int jindex = N-1;
	for (int k = 0; k < N; k++) {
	  coveredi[L[i][k].getI()] = true;
	  while (jindex > 0 && coveredi[L[j][jindex].getI()]) {
	    coveredj[L[j][jindex].getI()] = false;
	    jindex--;
	  }
	  if (L[i][k].getD() == 0) continue;
	  if (coveredj[L[i][k].getI()]) continue;
	  if (L[j][jindex].getD() == 0) break;
	  double dist = Math.sqrt(L[i][k].getD()) +
	    Math.sqrt(L[j][jindex].getD()) +
	    Math.sqrt(sqdist(i, j));
	  best_dist = Math.min(best_dist, dist);
	}
      }
    }
    
    return best_dist;
  }

  private double solve() {
    if (N == 2) {
      return Math.sqrt(sqdist(0, 1));
    }
    double ans1 = solve1();
    double ans2 = solve2();
    return Math.min(ans1, ans2);
  }
  
  private void work() {
    Scanner sc = new Scanner(new BufferedInputStream(System.in));
    N = sc.nextInt();
    p = new Point[N];
    for (int i = 0; i < N; i++) {
      int x = sc.nextInt();
      int y = sc.nextInt();
      p[i] = new Point(x, y);
    }
    double ans = solve();

    System.out.printf("%.10f", ans);
    System.out.println();
  }

  public static void main(String[] args) {
    new buildingroads_hc().work();
  }
}
