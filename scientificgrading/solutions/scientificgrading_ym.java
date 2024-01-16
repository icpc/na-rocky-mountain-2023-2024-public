import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;

class Term implements Comparable<Term> {
    int scale;
    BigDecimal unscaled;

    Term(int scale, BigDecimal unscaled) {
        this.scale = scale;
        this.unscaled = unscaled;
    }

    @Override
    public int compareTo(Term other) {
        if (this.scale < other.scale) return -1;
        if (this.scale > other.scale) return 1;
        return unscaled.compareTo(other.unscaled);
    }
}

class Value implements Comparable<Value> {
    private static final int MARGIN = 40;
    static final Value NEG = new Value("-1.000000000e+0");
    static final Value ZERO = new Value("+0.000000000e+0");

    ArrayList<Term> terms = new ArrayList<>();

    Value(String s) {
        int scale = Integer.parseInt(s.substring(13)) - 9;
        BigDecimal unscaled = new BigDecimal(s.substring(0, 12).replace(".", ""));
        terms.add(new Term(scale, unscaled));
    }

    Value(ArrayList<Term> terms) {
        this.terms = terms;
    }

    Value add(Value other) {
        ArrayList<Term> terms = new ArrayList<Term>(this.terms);
        terms.addAll(other.terms);
        Collections.sort(terms);

        ArrayList<Term> ret = new ArrayList<>();

        for (Term t : terms) {
            if (!ret.isEmpty() && t.scale < ret.get(ret.size() - 1).scale + MARGIN) {
                int s = ret.get(ret.size() - 1).scale;
                ret.set(ret.size() - 1, new Term(
                    s,
                    ret.get(ret.size() - 1).unscaled.add(t.unscaled.multiply(BigDecimal.TEN.pow(t.scale - s)))
                ));
            } else {
                ret.add(t);
            }
        }
        Collections.reverse(ret);
        return new Value(ret);
    }

    Value mul(Value other) {
        ArrayList<Term> terms = new ArrayList<>();

        for (Term t : this.terms) {
            for (Term s : other.terms) {
                terms.add(new Term(t.scale + s.scale, t.unscaled.multiply(s.unscaled)));
            }
        }
        Collections.sort(terms, Collections.reverseOrder());
        return new Value(terms);
    }

    Value sub(Value other) { return add(NEG.mul(other)); }
    Value abs() { return compareTo(ZERO) < 0 ? mul(NEG) : this; }
    Value min(Value other) { return compareTo(other) < 0 ? this : other; }

    int sign() {
        for (Term t: this.terms) {
            int r = t.unscaled.compareTo(BigDecimal.ZERO);
            if (r != 0) return r;
        }
        return 0;
    }

    @Override
    public int compareTo(Value other) { return sub(other).sign(); }

    @Override
    public String toString() {
        boolean first = false;
        StringBuilder sb = new StringBuilder();
        sb.append("[");
        for (Term t: terms) {
            if (first) sb.append(", ");
            sb.append(t.unscaled.toString());
            sb.append(" * 10^");
            sb.append(Integer.toString(t.scale));
            first = true;
        }
        sb.append("]");
        return sb.toString();
    }
}

public class scientificgrading_ym {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        Value eps = new Value("+1.000000000e-9");
        Value one = new Value("+1.000000000e+0");

        Value x = new Value(sc.nextLine());
        Value y = new Value(sc.nextLine());
        Value a = new Value(sc.nextLine());
        Value b = new Value(sc.nextLine());
        Value c = new Value(sc.nextLine());
        Value d = new Value(sc.nextLine());
        sc.close();

        Value p = x.add(y).sub(a).abs();
        Value q = x.sub(y).sub(b).abs();

        boolean result[] = {false, false, false, false};

        result[0] |= p.compareTo(Value.ZERO) == 0;
        result[0] |= p.compareTo(eps.mul(one.min(x.add(y).abs()))) < 0;

        result[1] |= q.compareTo(Value.ZERO) == 0;
        result[1] |= q.compareTo(eps.mul(one.min(x.sub(y).abs()))) < 0;

        result[2] = x.mul(y).sub(c).abs().compareTo(eps.mul(one.min(x.mul(y).abs()))) < 0;
        result[3] = x.sub(y.mul(d)).abs().compareTo(eps.mul(x.abs().min(y.abs()))) < 0;

        for (int i = 0; i < 4; ++i) System.out.println(result[i] ? "Correct" : "Incorrect");
    }
}
