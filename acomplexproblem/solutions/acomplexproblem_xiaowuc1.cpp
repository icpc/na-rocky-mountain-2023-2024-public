#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef vector<int> vi;

vi val, comp, z, cont;
int Time, ncomps;
template<class G, class F> int dfs(int j, G& g, F f) {
	int low = val[j] = ++Time, x; z.push_back(j);
	trav(e,g[j]) if (comp[e] < 0)
		low = min(low, val[e] ?: dfs(e,g,f));

	if (low == val[j]) {
		do {
			x = z.back(); z.pop_back();
			comp[x] = ncomps;
			cont.push_back(x);
		} while (x != j);
		f(cont); cont.clear();
		ncomps++;
	}
	return val[j] = low;
}
template<class G, class F> void scc(G& g, F f) {
	int n = sz(g);
	val.assign(n, 0); comp.assign(n, -1);
	Time = ncomps = 0;
	rep(i,0,n) if (comp[i] < 0) dfs(i, g, f);
}

vector<vi> adj;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int m, n;
  cin >> m >> n;
  map<string, int> mapping;
  vector<array<string, 2>> edges(m+n);
  int nv = 0;
  for(auto& x: edges) {
    cin >> x[0] >> x[1];
    if(!mapping.count(x[0])) mapping[x[0]] = nv++;
    if(!mapping.count(x[1])) mapping[x[1]] = nv++;
  }
  adj.resize(nv);
  for(int i = 0; i < m+n; i++) {
    int a = mapping[edges[i][0]];
    int b = mapping[edges[i][1]];
    adj[a].push_back(b);
  }
  vi compsize;
  compsize.resize(nv);
  scc(adj, [&](vi& v) {
    compsize[ncomps] = sz(v);
  });
  int rhs = ncomps;
  adj.clear();
  adj.resize(nv);
  for(int i = 0; i < m+n; i++) {
    int a = mapping[edges[i][0]];
    int b = mapping[edges[i][1]];
    adj[a].push_back(b);
    if(i < m) adj[b].push_back(a);
  }
  scc(adj, [&](vi& v) {
    compsize[ncomps] = sz(v);
  });
  vector<vector<array<int, 2>>> graph(nv);
  vector<vector<int>> comptov(ncomps);
  vector<int> par(nv);
  for(int i = 0; i < nv; i++) comptov[comp[i]].push_back(i);
  for(int i = 0; i < m+n; i++) {
    int a = mapping[edges[i][0]];
    int b = mapping[edges[i][1]];
    graph[a].push_back({b, i >= m});
    if(comp[a] != comp[b]) par[comp[b]]++;
  }
  vector<int> dp(nv, 1);
  priority_queue<array<int, 2>, vector<array<int, 2>>, greater<array<int, 2>>> q;
  for(int i = 0; i < nv; i++) if(par[comp[i]] == 0) q.push({dp[i], i});
  while(q.size()) {
    auto [w, v] = q.top(); q.pop();
    if(dp[v] != w) continue;
    for(auto [u, nw] : graph[v]) {
      if(nw == 0) {
        if(dp[v] > dp[u]) {
          dp[u] = dp[v];
          q.push({dp[u], u});
        }
        continue;
      }
      if(dp[v] + 1 > dp[u]) {
        dp[u] = dp[v] + 1;
        q.push({dp[u], u});
      }
      if(--par[comp[v]] == 0) {
        for(int out: comptov[comp[u]]) q.push({dp[out], out});
      }
    }
  }
  cout << *max_element(dp.begin(), dp.end()) << " " << rhs << "\n";
}
