import java.io.BufferedInputStream;
import java.util.Scanner;

public class attendance_da {

    private void work() {
        Scanner sc = new Scanner(new BufferedInputStream(System.in));
        int n = sc.nextInt();
        String[] callouts = new String[n + 1];
        for (int i = 0; i < n; i++) callouts[i] = sc.next();
        callouts[n] = "meh";
        int absent = 0;
        for (int i = 0; i < n; i++) {
            if (callouts[i].equals("Present!")) continue;
            if (!callouts[i + 1].equals("Present!")){
                absent++;
                System.out.println(callouts[i]);
            }
        }

        if(absent == 0) System.out.println("No Absences");
    }

    public static void main(String[] args) {
        new attendance_da().work();
    }
}
