import java.util.Scanner;
import java.util.Vector;

import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

class Reader {
    final private int BUFFER_SIZE = 1 << 16;
    private DataInputStream din;
    private byte[] buffer;
    private int bufferPointer, bytesRead;

    public Reader() {
        din = new DataInputStream(System.in);
        buffer = new byte[BUFFER_SIZE];
        bufferPointer = bytesRead = 0;
    }

    public Reader(InputStream in) {
        din = new DataInputStream(in);
        buffer = new byte[BUFFER_SIZE];
        bufferPointer = bytesRead = 0;
    }

    public Reader(String file_name) throws IOException {
        din = new DataInputStream(new FileInputStream(file_name));
        buffer = new byte[BUFFER_SIZE];
        bufferPointer = bytesRead = 0;
    }

    private void fillBuffer() throws IOException {
        bytesRead = din.read(buffer, bufferPointer = 0, BUFFER_SIZE);
        if (bytesRead == -1)
            buffer[0] = -1;
    }

    private byte read() throws IOException {
        if (bufferPointer == bytesRead)
            fillBuffer();
        return buffer[bufferPointer++];
    }

    public void close() throws IOException {
        if (din == null)
            return;
        din.close();
    }

    public boolean hasNext() throws IOException {
        return din.available() > 0;
    }

    public String nextLine() throws IOException {
        byte[] buf = new byte[64]; // line length
        int cnt = 0, c;
        while ((c = read()) != -1) {
            if (c == '\n')
                break;
            buf[cnt++] = (byte) c;
        }
        return new String(buf, 0, cnt);
    }

    public String next() throws IOException {
        StringBuffer ret = new StringBuffer();
        byte c = read();
        while (Character.isWhitespace(c))
            c = read();

        do {
            ret.append((char) c);
        } while (!Character.isWhitespace(c = read()));

        return ret.toString();
    }

    public char nextChar() throws IOException {
        byte c = read();
        while (Character.isWhitespace(c))
            c = read();
        return (char) c;
    }

    public int nextInt() throws IOException {
        int ret = 0;
        byte c = read();
        while (c <= ' ')
            c = read();
        boolean neg = (c == '-');
        if (neg)
            c = read();
        do {
            ret = ret * 10 + c - '0';
        } while ((c = read()) >= '0' && c <= '9');

        if (neg)
            return -ret;
        return ret;
    }

    public long nextLong() throws IOException {
        long ret = 0;
        byte c = read();
        while (c <= ' ')
            c = read();
        boolean neg = (c == '-');
        if (neg)
            c = read();
        do {
            ret = ret * 10 + c - '0';
        } while ((c = read()) >= '0' && c <= '9');
        if (neg)
            return -ret;
        return ret;
    }

    public double nextDouble() throws IOException {
        double ret = 0, div = 1;
        byte c = read();
        while (c <= ' ')
            c = read();
        boolean neg = (c == '-');
        if (neg)
            c = read();

        do {
            ret = ret * 10 + c - '0';
        } while ((c = read()) >= '0' && c <= '9');

        if (c == '.') {
            while ((c = read()) >= '0' && c <= '9') {
                ret += (c - '0') / (div *= 10);
            }
        }

        if (neg)
            return -ret;
        return ret;
    }

}

public class ribbonroad_cz_optimized_io {

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

    public static void main(String[] args) throws IOException {
        long start = System.currentTimeMillis();
        Reader scan = new Reader(System.in);
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