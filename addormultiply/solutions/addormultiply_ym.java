import java.util.ArrayList;
import java.util.Scanner;

class Expression {
    long x, y, z;
    Expression(long z) { this.x = this.y = -1; this.z = z; }
    Expression(long x, long y, long z) { this.x = x; this.y = y; this.z = z; }
}

class CompTree {
    static final long M = 1000000007;
    int n, m;
    ArrayList<Expression> data = new ArrayList<>();

    static Expression IDENT = new Expression(1);

    CompTree(int n) {
        int m = 1;
        while (m < n) m *= 2;
        for (int i = 0; i < 2 * m; ++i) data.add(IDENT);
        this.n = n;
        this.m = m;
    }

    Expression merge(Expression a, Expression b) {
        switch ((a.x >= 0 ? 2 : 0) + (b.x >= 0 ? 1 : 0)) {
            case 0: return new Expression(a.z * b.z % M);
            case 1: return new Expression(a.z * b.x % M, b.y, b.z);
            case 2: return new Expression(a.x, a.y, a.z * b.z % M);
            case 3: return new Expression(a.x, (a.y + a.z * b.x + b.y) % M, b.z);
            default: break;
        }
        return IDENT;
    }

    void update(int index, long d) {
        int k = m + index;
        data.set(k, d > 0 ? new Expression(1, 0, d) : new Expression(-d));
        while (k > 1) {
            k /= 2;
            data.set(k, merge(data.get(k * 2), data.get(k * 2 + 1)));
        }
    }

    long total() { return (data.get(1).y + data.get(1).z) % M; }
}


public class addormultiply_ym {
    public static void main(String[] args) {
        
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        int m = sc.nextInt();

        String initial_condition = sc.next();

        long xs[] = new long[n];

        for (int i = 0; i < initial_condition.length(); i += 2) {
            long sign = (i == 0 || initial_condition.charAt(i - 1) == '+') ? 1 : -1;
            xs[i / 2] = sign * (initial_condition.charAt(i) - '0');
        }

        CompTree ts[] = {new CompTree(n), new CompTree(n)};

        for (int i = 0; i < n; ++i) {
            ts[0].update(i, xs[i]);
            ts[1].update(i, i == 0 ? xs[i] : -xs[i]);
        }

        int current_phase = 0;

        System.out.println(Long.toString(ts[current_phase].total()));

        for (int q = 0; q < m; ++q) {
            String op = sc.next();
            switch (op) {
                case "a":
                    current_phase ^= 1;
                    break;
                case "f":
                    int op_idx = sc.nextInt();
                    xs[op_idx] *= -1;
                    ts[0].update(op_idx, xs[op_idx]);
                    ts[1].update(op_idx, -xs[op_idx]);
                    break;
                default:
                    int i = sc.nextInt() - 1;
                    int j = sc.nextInt() - 1;
                    if (xs[i] == xs[j]) break;

                    long x = xs[i];
                    xs[i] = ((x > 0) == (xs[j] > 0)) ? xs[j] : -xs[j];
                    xs[j] = ((x > 0) == (xs[j] > 0)) ? x : -x;
                    ts[0].update(i, xs[i]);
                    ts[0].update(j, xs[j]);
                    ts[1].update(i, i == 0 ? xs[i] : -xs[i]);
                    ts[1].update(j, j == 0 ? xs[j] : -xs[j]);
                    break;
            }
            System.out.println(Long.toString(ts[current_phase].total()));
        }

        sc.close();
    }
}
