import java.io.BufferedInputStream;
import java.util.*;
import java.lang.Math;



public class acomplexproblem_hc {
  static final int MAX_NODES = 100005;

  static class Graph {
    int numNodes;
    ArrayList<TreeMap<Integer, Integer>> adj;

    public Graph() {
      adj = new ArrayList<TreeMap<Integer, Integer>>();
      for (int i = 0; i < MAX_NODES; i++) {
	adj.add(new TreeMap<Integer, Integer>());
      }
      clear();
    }
    
    public void clear() {
      numNodes = 0;
      for (int i = 0; i < MAX_NODES; i++) {
	adj.get(i).clear();
      }
    }

    public void add_edge(int u, int v, int strict) {
      adj.get(u).put(v, strict);
    }
  }

  private int[] po, comp;

  private void DFS(int v, Graph G, Graph G_scc, int[] C,
	   Stack<Integer> P, Stack<Integer> S) {
    po[v] = C[0];
    C[0]++;

    S.push(v);  P.push(v);
    for (Map.Entry<Integer,Integer> entry : G.adj.get(v).entrySet()) {
      int w = entry.getKey();
      int s = entry.getValue();
      if (po[w] == -1) {
	DFS(w, G, G_scc, C, P, S);
      } else if (comp[w] == -1) {
	while (!P.empty() && (po[P.peek()] > po[w])) {
	  P.pop();
	}
      }
    }

    if (!P.empty() && P.peek() == v) {
      while (!S.empty()) {
	int t = S.pop();
	comp[t] = G_scc.numNodes;
	if (t == v) {
	  break;
	}
      }
      G_scc.numNodes++;
      P.pop();
    }
  }
  
  private int SCC(Graph G, Graph G_scc) {
    G_scc.clear();
    int[] C = { 1 };
    Stack<Integer> P = new Stack<Integer>();
    Stack<Integer> S = new Stack<Integer>();
    for (int i = 0; i < G.numNodes; i++) {
      po[i] = -1;
      comp[i] = -1;
    }

    for (int i = 0; i < G.numNodes; i++) {
      if (po[i] == -1) {
	DFS(i, G, G_scc, C, P, S);
      }
    }

    for (int i = 0; i < G.numNodes; i++) {
      for (Map.Entry<Integer, Integer> entry : G.adj.get(i).entrySet()) {
	int w = entry.getKey();
	int s = entry.getValue();
	if (comp[i] != comp[w]) {
	  G_scc.add_edge(comp[i], comp[w], s);
	}
      }
    }

    return G_scc.numNodes;
  }

  private TreeMap<String, Integer> prob;

  private int prob_id(String s) {
    if (prob.get(s) == null) {
      int next = prob.size();
      prob.put(s, next);
    }
    return prob.get(s);
  }

  static class Pair {
    int a, b;

    public Pair(int a, int b) {
      this.a = a;
      this.b = b;
    }

    public int getA() {
      return a;
    }

    public int getB() {
      return b;
    }
  }
  
  private int M, N;
  private ArrayList<Pair> subseteq, subset;

  private void read_edge(Scanner sc, ArrayList<Pair> S, int N) {
    S.clear();
    S.ensureCapacity(N);
    for (int i = 0; i < N; i++) {
      String a, b;
      a = sc.next();
      b = sc.next();
      S.add(new Pair(prob_id(a), prob_id(b)));
    }
  }

  private Graph G, G_scc;
  ArrayList<Integer> pathlen;

  private int DFS(int v) {
    if (pathlen.get(v) >= 0) {
      return pathlen.get(v);
    }
    pathlen.set(v, 1);
    for (Map.Entry<Integer, Integer> entry : G_scc.adj.get(v).entrySet()) {
      int w = entry.getKey();
      int s = entry.getValue();
      pathlen.set(v, Math.max(pathlen.get(v), s + DFS(w)));
    }

    return pathlen.get(v);
  }
      
  private void work() {
    po = new int[MAX_NODES];
    comp = new int[MAX_NODES];
    subseteq = new ArrayList<Pair>();
    subset = new ArrayList<Pair>();
    G = new Graph();
    G_scc = new Graph();
    prob = new TreeMap<String, Integer>();

    Scanner sc = new Scanner(new BufferedInputStream(System.in));
    M = sc.nextInt();
    N = sc.nextInt();
    
    read_edge(sc, subseteq, M);
    read_edge(sc, subset, N);

    G.numNodes = prob.size();
    for (int i = 0; i < subseteq.size(); i++) {
      Pair p = subseteq.get(i);
      G.add_edge(p.getA(), p.getB(), 0);
    }
    int num_scc = SCC(G, G_scc);

    for (int i = 0; i < subset.size(); i++) {
      Pair p = subset.get(i);
      G_scc.add_edge(comp[p.getA()], comp[p.getB()], 1);
    }

    pathlen = new ArrayList<Integer>(num_scc);
    for (int i = 0; i < num_scc; i++) {
      pathlen.add(-1);
    }
    for (int i = 0; i < num_scc; i++) {
      DFS(i);
    }

    System.out.print(Collections.max(pathlen));
    System.out.println(" " + num_scc);
  }

  public static void main(String[] args) {
    new acomplexproblem_hc().work();
  }
}
