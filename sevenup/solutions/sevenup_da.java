import java.io.BufferedInputStream;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

public class sevenup_da {

    static class State {
        int up, totalRem;
        int[] down, rem;

        State(int up, int[] down, int[] rem) {
            this.up = up;
            this.down = new int[7];
            this.rem = new int[8];
            System.arraycopy(down, 0, this.down, 0, 7);
            for (int i = 0; i < 8; i++) {
                this.rem[i] = rem[i];
                totalRem += rem[i];
            }
        }

        @Override
        public boolean equals(Object o) {
            State state = (State) o;
            if (up != state.up) return false;
            return Arrays.equals(down, state.down) && Arrays.equals(rem, state.rem);
        }

        @Override
        public int hashCode() {
            return 31 * (31 * up + Arrays.hashCode(down)) + Arrays.hashCode(rem);
        }
    }

    private void work() {
        String seven = "A234567";

        Scanner sc = new Scanner(new BufferedInputStream(System.in));
        String start = sc.next();
        int[] rem = new int[8];
        int[] down = new int[7];
        for (int i = 0; i < 7; i++) rem[i] = 4;
        rem[7] = 24;
        for (int i = 0; i < 7; i++) {
            int k = seven.indexOf(start.charAt(i));
            if (k < 0) {
                k = 7;
            }
            down[i] = k;
            rem[k]--;
        }

        State s = new State(0, down, rem);
        memo = new HashMap<>();
        System.out.println(go(s));
    }

    private static final int DONE = 127;
    private Map<State, Double> memo;

    private double go(State s) {
        if (s.up == DONE) return 0;
        if (memo.containsKey(s)) return memo.get(s);
        double ret = 1;
        for (int i = 0; i < 8; i++) {
            if (s.rem[i] == 0) continue;
            State next = new State(s.up, s.down, s.rem);
            next.rem[i]--;
            next.totalRem--;
            int t = i;
            while (t < 7 && ((1 << t) & next.up) == 0) {
                next.up |= 1 << t;
                t = s.down[t];
            }

            ret += (1.0 * s.rem[i] / s.totalRem) * go(next);
        }

        memo.put(s, ret);
        return ret;
    }

    public static void main(String[] args) {
        new sevenup_da().work();
    }
}
