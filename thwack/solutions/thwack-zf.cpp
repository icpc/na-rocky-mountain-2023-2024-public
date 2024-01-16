#include <iostream>
#include <cstring>
#include <unordered_set>

using namespace std;

const int MAXN = 210;

string game;
int memo[MAXN][MAXN][3][3];

char chr[3] = {'.', 'B', 'W'};
int ord[256];

// represents the subgame chr[pl] + game[l:r] + chr[pr]
struct state {
  int l, r, pl, pr;
};

const state NULL_GAME = {0, 0, 0, 0};

// compute the two subgames obtained by moving stone i (s.l <= i < s.r) to the left
// returns true iff this is a valid move
bool moveL(state s, int i, state& sL, state& sR) {
  if (i == s.l) {
    if (chr[s.pl] == '.' || chr[s.pl] == game[i]) return false;
    sL = NULL_GAME;
    sR = {i+1, s.r, ord['.'], s.pr};
  }
  else {
    if (game[i] == game[i-1]) return false;
    sL = {s.l, i-1, s.pl, ord[game[i]]};
    sR = {i+1, s.r, ord['.'], s.pr};
  }
  return true;
}

// compute the two subgames obtained by moving stone i (s.l <= i < s.r) to the right
// returns true iff this is a valid move
bool moveR(state s, int i, state& sL, state& sR) {
  if (i+1 == s.r) {
    if (chr[s.pr] == '.' || chr[s.pr] == game[i]) return false;
    sL = {s.l, i, s.pl, ord['.']};
    sR = NULL_GAME;
  }
  else {
    if (game[i] == game[i+1]) return false;
    sL = {s.l, i, s.pl, ord['.']};
    sR = {i+2, s.r, ord[game[i]], s.pr};
  }

  return true;
}

// compute the nimber of the subgame represented by s
int nimber(state s) {
  auto [l, r, pl, pr] = s;
  int& x = memo[l][r][pl][pr];

  if (x == -1) {
    unordered_set<int> moves;
    char cl = chr[pl], cr = chr[pr];
    // empty state
    if (l == r) {
      // either no move or exaclty one move
      x = (pl*pr == 2);
    }
    else {
      // try moving the stone represted by pl
      if (cl != '.' && cl != game[l]) moves.insert(nimber({l+1, r, pl, pr}));
      
      // try moving the stone represented by pr
      if (cr != '.' && cr != game[r-1]) moves.insert(nimber({l, r-1, pl, pr}));

      // try moving each stone in game[l:r] to the left and also to the right
      for (int i = l; i < r; ++i) {
        state sL, sR;

        if (moveL(s, i, sL, sR)) moves.insert(nimber(sL) ^ nimber(sR));
        if (moveR(s, i, sL, sR)) moves.insert(nimber(sL) ^ nimber(sR));
      }
      x = 0;
      while (moves.find(x) != moves.end()) ++x;
    }
  }


  return x;
}

int main() {
  // initialization
  memset(memo, -1, sizeof(memo));
  memo[NULL_GAME.l][NULL_GAME.r][NULL_GAME.pl][NULL_GAME.pr] = 0;
  for (int i = 0; i < 3; ++i) ord[chr[i]] = i;

  int n;
  cin >> n >> game;
  game += '.';

  // gl[i] and gr[i] will be the [l,r) interval of a consecutive sequence of B/W
  // ng will be the # of these subgames, n_tot will be the nim number of the initial game state
  state subgames[MAXN];
  int ng = 0, n_tot = 0;

  int l = 0;
  for (int r = 0; r <= n; ++r)
    if (game[r] == '.') {
      if (l != r) {
        subgames[ng] = {l, r, ord['.'], ord['.']};
        n_tot ^= nimber(subgames[ng]);
        ++ng;
      }
      l = r+1;
    }
  
  int a[MAXN], d[MAXN];
  int W = 0;

  // go through each connected subgame
  // try each valid move in the connected subgame and update the resulting game nimber
  // the move is a winning move iff the resulting nimber is 0
  for (int i = 0; i < ng; ++i) {
    for (int j = subgames[i].l; j < subgames[i].r; ++j) {
      state sL, sR;

      if (moveL(subgames[i], j, sL, sR) && (n_tot^nimber(subgames[i])^nimber(sL)^nimber(sR)) == 0) {
        a[W] = j;
        d[W] = j-1;
        ++W;
      }

      if (moveR(subgames[i], j, sL, sR) && (n_tot^nimber(subgames[i])^nimber(sL)^nimber(sR)) == 0) {
        a[W] = j;
        d[W] = j+1;
        ++W;
      }
    }
  }

  cout << W << endl;
  for (int i = 0; i < W; ++i) cout << a[i]+1 << ' ' << d[i]+1 << endl;

  return 0; 
}