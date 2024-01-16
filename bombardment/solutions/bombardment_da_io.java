import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.Scanner;

public class bombardment_da_io {
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

    private void work() throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String[] line = br.readLine().trim().split("\\s+");
        int n = Integer.parseInt(line[0]);
        r = Integer.parseInt(line[1]);
        a = new int[n];
        line = br.readLine().trim().split("\\s+");
        for (int i = 0; i < n; i++) a[i] = Integer.parseInt(line[i]);
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


    public static void main(String[] args) throws IOException {
        new bombardment_da_io().work();
    }
}
