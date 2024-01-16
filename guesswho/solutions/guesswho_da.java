import java.io.BufferedInputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

public class guesswho_da {
    private void work() {
        Scanner sc = new Scanner(new BufferedInputStream(System.in));
        int n = sc.nextInt();
        int m = sc.nextInt();
        int q = sc.nextInt();
        char[][] cs = new char[n][m];
        for (int i = 0; i < n; i++) {
            cs[i] = sc.next().toCharArray();
        }

        char[] a = new char[m];
        while (q-- > 0) {
            int k = sc.nextInt() - 1;
            a[k] = sc.next().charAt(0);
        }

        List<Integer> ans = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            if (canDo(m, a, cs[i])) ans.add(i + 1);
        }

        if (ans.size() == 1) {
            System.out.println("unique");
            System.out.println(ans.get(0));
        } else {
            System.out.println("ambiguous");
            System.out.println(ans.size());
        }
    }

    private boolean canDo(int m, char[] a, char[] c) {
        for (int i = 0; i < m; i++) {
            if (a[i] == '\0') continue;
            if (a[i] != c[i]) return false;
        }

        return true;
    }

    public static void main(String[] args) {
        new guesswho_da().work();
    }
}
