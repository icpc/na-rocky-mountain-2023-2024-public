import java.io.BufferedInputStream;
import java.util.*;
import java.lang.Math;


public class gasstation_hc {
  static class Car {
    int arr, fill;
    int side;
    int left;

    public void read(Scanner sc) {
      arr = sc.nextInt();
      fill = sc.nextInt();
      String s = sc.next();
      side = (s.equals("L")) ? 1 : 0;
    }

    @Override
    public String toString() {
      return "(" + arr + ", " + fill + ", " + side + ")";
    }
  }

  static class Event implements Comparable<Event> {
    int time;
    int c_index;
    boolean arrive;
    int pump_index, pumpAB;

    public Event(int t, int index) {
      time = t;
      c_index = index;
      arrive = true;
      pump_index = -1;
      pumpAB = -1;
    }

    public Event(int t, int index, int p_index, int pAB) {
      time = t;
      c_index = index;
      arrive = false;
      pump_index = p_index;
      pumpAB = pAB;
    }

    @Override
    public int compareTo(Event e) {
      if (time != e.time) {
	return time - e.time;
      }

      if (arrive == e.arrive) return 0;
      if (!arrive) return -1;
      else return 1;
    }
  }

  private int P, N;
  private Car[] car;
  private boolean[][][] pump;
  private ArrayList<ArrayList<Queue<Integer>>> lineup;
  private PriorityQueue<Event> event;

  private boolean open_column(int pi, int side) {
    return !pump[pi][0][side];
  }

  void serve(int cindex, int pi, int time) {
    int side = car[cindex].side;
    int pumpAB = 0;
    if (!pump[pi][1][side]) {
      pumpAB = 1;
    }
    pump[pi][pumpAB][side] = true;
    event.add(new Event(time + car[cindex].fill, cindex, pi, pumpAB));
  }

  void process_arrive(Event e) {
    int index = e.c_index;
    int side = car[index].side;

    for (int i = 0; i < P; i++) {
      if (open_column(i, side)) {
	serve(index, i, car[index].arr);
	return;
      }
    }

    int best_q = 0;
    for (int i = 0; i < P; i++) {
      if (lineup.get(i).get(side).size() <
	  lineup.get(best_q).get(side).size()) {
	best_q = i;
      }
    }
    lineup.get(best_q).get(side).add(index);
  }

  void process_leave(Event e) {
    int pi = e.pump_index;
    int pAB = e.pumpAB;
    int side = car[e.c_index].side;

    car[e.c_index].left = e.time;
    pump[pi][pAB][side] = false;
  }

  void check_queues(int time) {
    for (int pi = 0; pi < P; pi++) {
      for (int side = 0; side < 2; side++) {
	while (lineup.get(pi).get(side).size() > 0 &&
	       open_column(pi, side)) {
	  serve(lineup.get(pi).get(side).poll(), pi, time);
	}
      }
    }
  }
  
  private void work() {
    Scanner sc = new Scanner(new BufferedInputStream(System.in));
    P = sc.nextInt();
    N = sc.nextInt();
    car = new Car[N];
    for (int i = 0; i < N; i++) {
      car[i] = new Car();
      car[i].read(sc);
    }

    pump = new boolean[P][2][2];
    lineup = new ArrayList<ArrayList<Queue<Integer>>>();
    for (int i = 0; i < P; i++) {
      lineup.add(new ArrayList<Queue<Integer>>());
      for (int j = 0; j < 2; j++) {
	for (int k = 0; k < 2; k++) {
	  pump[i][j][k] = false;
	}
	lineup.get(i).add(new LinkedList<Integer>());
      }
    }
    event = new PriorityQueue<Event>();
    
    int time = 0;
    for (int i = 0; i < N; i++) {
      event.add(new Event(car[i].arr, i));
    }

    while (event.size() > 0) {
      Event e = event.poll();
      if (e.arrive) {
	process_arrive(e);
      } else {
	int time2 = e.time;
	process_leave(e);
	while (event.size() > 0 && !(event.peek().arrive) &&
	       event.peek().time == time2) {
	  e = event.poll();
	  process_leave(e);
	}
	check_queues(time2);
      }
	  
    }

    for (int i = 0; i < N; i++) {
      System.out.print(car[i].left);
      System.out.println();
    }
  }

  public static void main(String[] args) {
    new gasstation_hc().work();
  }
}
