/*
  Using (slightly modified) strongly connected components from KACTL
  https://github.com/kth-competitive-programming/kactl/blob/main/content/graph/SCC.h
*/

#include <bits/stdc++.h>

using namespace std;

vector<int> val, comp, z, cont;
int Time, ncomps;
template<class G> int dfs(int j, G& g) {
	int low = val[j] = ++Time, x; z.push_back(j);
	for (auto e : g[j]) if (comp[e] < 0)
		low = min(low, val[e] ?: dfs(e,g));

	if (low == val[j]) {
		do {
			x = z.back(); z.pop_back();
			comp[x] = ncomps;
			cont.push_back(x);
		} while (x != j);
		cont.clear();
    ++ncomps;
	}
	return val[j] = low;
}
template<class G> void scc(G& g) {
	int n = g.size();
	val.assign(n, 0); comp.assign(n, -1);
	Time = ncomps = 0;
	for(int i = 0; i < n; ++i) if (comp[i] < 0) dfs(i, g);
}

int lpath(const vector<vector<pair<int,int>>>& h, int v, vector<int>& memo) {
  int& ans = memo[v];
  if (ans == -1) {
    ans = 1;
    for (auto [u, w] : h[v]) ans = max(ans, w+lpath(h, u, memo));
  }
  return ans;
}

int main() {
  int M, N;
  cin >> M >> N;

  unordered_map<string, int> id;
  vector<pair<string, string>> e1(M), e2(N);

  auto touch = [&](const string& s) {
    if (id.find(s) == id.end()) {
      id[s] = id.size();
    }
  };

  for (auto& [a, b] : e1) {
    cin >> a >> b;
    touch(a);
    touch(b);
  }
  for (auto& [a, b] : e2) {
    cin >> a >> b;
    touch(a);
    touch(b);
  }

  vector<vector<int>> g(id.size());
  for (const auto& [a, b] : e1) g[id[a]].push_back(id[b]);

  scc(g);

  vector<vector<pair<int,int>>> h(ncomps);
  for (const auto& [a, b] : e1) {
    int u = comp[id[a]], v = comp[id[b]];
    if (u != v) h[comp[id[a]]].push_back({comp[id[b]], 0});
  }

  for (const auto& [a, b] : e2) {
    int u = comp[id[a]], v = comp[id[b]];
    if (u != v) h[comp[id[a]]].push_back({comp[id[b]], 1});
  }

  int ans = 1;
  vector<int> memo(ncomps, -1);
  for (int v = 0; v < ncomps; ++v)
    ans = max(ans, lpath(h, v, memo));

  cout << ans << ' ' << ncomps << endl;

  return 0;
}