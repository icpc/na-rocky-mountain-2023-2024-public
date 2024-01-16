import java.util.Scanner;
import java.util.Vector;

public class ribbonroad_cz_scanner {

    static boolean linePointIntersection(long[] a, long[] b, long[] point) {
        long cross = (point[1] - a[1]) * (b[0] - a[0]) - (point[0] - a[0]) * (b[1] - a[1]);
        if (cross != 0) {
            return false;
        }
        long dot = (point[0] - a[0]) * (b[0] - a[0]) + (point[1] - a[1]) * (b[1] - a[1]);
        if (dot < 0) {
            return false;
        }
        long squaredlength = (b[0] - a[0]) * (b[0] - a[0]) + (b[1] - a[1]) * (b[1] - a[1]);
        if (dot > squaredlength) {
            return false;
        }
        return true;
    }

    static boolean isPolygonCW(long[][] points) {
        int smallest = 0;
        long smallestx = points[smallest][0];
        long smallesty = points[smallest][1];
        for (int i = 0; i < points.length; i++) {
            if (points[i][0] < smallestx || (points[i][0] == smallestx && points[i][1] < smallesty)) {
                smallest = i;
                smallestx = points[i][0];
                smallesty = points[i][1];
            }
        }
        int prev = (smallest + points.length - 1) % points.length;
        int next = (smallest + points.length + 1) % points.length;

        long cross = (points[prev][0] - points[smallest][0]) * (points[next][1] - points[smallest][1])
                - (points[prev][1] - points[smallest][1]) * (points[next][0] - points[smallest][0]);

        return cross > 0;
    }

    public static void main(String[] args) {
        long start = System.currentTimeMillis();
        Scanner scan = new Scanner(System.in);
        int n = scan.nextInt();
        long[][] polyPoints = new long[n][2];
        for (int i = 0; i < n; i++) {
            polyPoints[i][0] = scan.nextInt();
            polyPoints[i][1] = scan.nextInt();
        }

        long[] christie = { scan.nextInt(), scan.nextInt() };
        long[] sig = { scan.nextInt(), scan.nextInt() };
        // System.err.println("Time to read: " + (System.currentTimeMillis() - start));
        boolean CW = isPolygonCW(polyPoints);
        // System.err.println("Christe " + christie[0] + " " + christie[1]);
        Vector<Character> ans = new Vector<Character>();
        // find the direction of the line that christie is on
        for (int i = 0; i < n; i++) {
            if (linePointIntersection(polyPoints[i], polyPoints[(i + 1) % n], christie)) {
                // Christie is parallel to this line

                double angSig = Math.atan2(sig[1] - christie[1], sig[0] - christie[0]);
                double angline = Math.atan2(polyPoints[(i + 1) % n][1] - polyPoints[i][1],
                        polyPoints[(i + 1) % n][0] - polyPoints[i][0]);
                if ((angSig - angline + 6 * Math.PI) % Math.PI < 1e-8) {
                    ans.add('?');
                    continue;
                }
                // System.err.println((angSig - angline + 2 * Math.PI) % (2 * Math.PI));

                if (CW) {
                    // if the polygon is clockwise and the signal is between 0-180 CW of the line,
                    // christie is inside
                    if ((angline - angSig + 2 * Math.PI) % (2 * Math.PI) < Math.PI) {
                        ans.add('I');
                    } else {
                        ans.add('O');
                    }
                } else {
                    // if the polygon is couterclockwise and the signal is between 0-180 CW of the
                    // line, christie is outside
                    if ((angline - angSig + 2 * Math.PI) % (2 * Math.PI) < Math.PI) {
                        ans.add('O');
                    } else {
                        ans.add('I');
                    }

                }
            }
        }
        char fin = ans.elementAt(0);
        for (int j = 0; j < ans.size(); j++) {
            if (fin != ans.elementAt(j)) {
                fin = '?';
                break;
            }
        }
        if (fin == '?') {
            System.out.println(fin);
        } else if (fin == 'I') {
            System.out.println("inside");
        } else {
            System.out.println("outside");
        }

        // System.err.println("Total time: " + (System.currentTimeMillis() - start));
    }
}