import java.io.BufferedInputStream;
import java.util.Scanner;

public class dontbefake_da {

    private void work() {
        int[] a = new int[86400];
        Scanner sc = new Scanner(new BufferedInputStream(System.in));
        int n = sc.nextInt();
        while(n-- > 0){
            int m = sc.nextInt();
            while(m -- > 0){
                int l = sc.nextInt();
                int r = sc.nextInt();
                for(int i = l; i <= r; i++) a[i]++;
            }
        }

        int best = 0;
        int bestCount = 0;
        for(int i = 0; i < 86400; i++){
            if(a[i] > best){
                best = a[i];
                bestCount = 0;
            }

            if(a[i] == best) bestCount++;
        }

        System.out.printf("%d\n%d", best, bestCount);
        System.out.println();
    }

    public static void main(String[] args) {
        new dontbefake_da().work();
    }
}
