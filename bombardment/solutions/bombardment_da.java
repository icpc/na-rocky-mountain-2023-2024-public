import java.io.BufferedInputStream;
import java.util.*;

public class bombardment_da {
    static class Bomb implements Comparable<Bomb> {
        int x, count;

        Bomb(int x, int count) {
            this.x = x;
            this.count = count;
        }

        @Override
        public int compareTo(Bomb o) {
            if (count == o.count) return x - o.x;
            return o.count - count;
        }
    }

    private int r, k;
    private int[] a;
    private Bomb[] ans;

    private void work() {
        Scanner sc = new Scanner(new BufferedInputStream(System.in));
        int n = sc.nextInt();
        r = sc.nextInt();
        a = new int[n];
        for (int i = 0; i < n; i++) a[i] = sc.nextInt();
        Arrays.sort(a);

        ans = new Bomb[n];
        k = 0;
        solve(0, n);
        Arrays.sort(ans, 0, k);

        System.out.println(k);
        for (int i = 0; i < k; i++) System.out.printf("%d ", ans[i].x);
        System.out.println();
    }

    private void solve(int s, int e) {
        if (s >= e) return;

        int bestX = 0;
        int bestCount = 0;
        int left = s;
        int right = s;
        int ll = 0, rr = 0;
        while (right < e) {
            while (a[left] + 2 * r < a[right]) left++;
            if (right - left + 1 > bestCount) {
                bestCount = right - left + 1;
                ll = left;
                rr = right;
                bestX = a[right] - r;
            }

            right++;
        }

        ans[k++] = new Bomb(bestX, bestCount);

        solve(s, ll);
        solve(rr + 1, e);
    }


    public static void main(String[] args) {
        new bombardment_da().work();
    }
}
