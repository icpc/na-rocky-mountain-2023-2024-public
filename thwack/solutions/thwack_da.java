import java.io.BufferedInputStream;
import java.util.*;

public class thwack_da {

    static class Game {
        int i, j;
        String s0, s1;

        Game(int i, int j, String s0, String s1) {
            this.i = i;
            this.j = j;
            this.s0 = s0;
            this.s1 = s1;
        }
    }

    private List<Game> play(String s) {
        List<Game> games = new ArrayList<>();
        for (int i = 0; i < s.length(); i++) {
            if (i > 0 && s.charAt(i) != s.charAt(i - 1)) {
                games.add(new Game(i, i - 1, s.substring(0, i - 1) + s.charAt(i), s.substring(i + 1)));
            }
            if (i < s.length() - 1 && s.charAt(i) != s.charAt(i + 1)) {
                games.add(new Game(i, i + 1, s.substring(0, i), s.charAt(i) + s.substring(i + 2)));
            }
        }

        return games;
    }

    private int solve(String s) {
        if(memo.containsKey(s)) return memo.get(s);
        Set<Integer> r = new HashSet<>();
        for (Game g : play(s)) r.add(solve(g.s0) ^ solve(g.s1));
        int i = 0;
        while (r.contains(i)) i++;
        memo.put(s, i);
        return i;
    }

    private Map<String, Integer> memo = new HashMap<>();

    private void work() {
        Scanner sc = new Scanner(new BufferedInputStream(System.in));
        int n = sc.nextInt();
        String[] ss = sc.next().split("\\.", -1);
        int tot = 0;
        for (String s : ss) {
            tot ^= solve(s);
        }

        List<Integer> ans = new ArrayList<>();
        int len = 1;
        for (String s : ss) {
            for (Game g : play(s)) {
                if ((tot ^ solve(s) ^ solve(g.s0) ^ solve(g.s1)) == 0) {
                    ans.add(g.i + len);
                    ans.add(g.j + len);
                }
            }
            len += s.length() + 1;
        }

        System.out.println(ans.size() / 2);
        for (int i = 0; i < ans.size(); i += 2) {
            System.out.printf("%d %d\n", ans.get(i), ans.get(i + 1));
        }
    }

    public static void main(String[] args) {
        new thwack_da().work();
    }
}
