#include <bits/stdc++.h>

using namespace std;

using ll = long long;

unordered_map<ll, double> memo;

string dealt;

int val(char c) {
  if (c == 'A') return 0;
  else if ('2' <= c && c <= '7') return c-'1';
  else return 7;
}

ll encode(int up[7], int rem[8]) {
  ll enc = 0;
  for (int i = 0; i < 7; ++i) enc = (enc<<1)|up[i];
  for (int i = 0; i < 7; ++i) enc = (enc<<3)|rem[i];
  enc = (enc<<5)|rem[7];
  
  return enc;
}

void decode(ll s, int up[7], int rem[8]) {
  rem[7] = s&0x1f;
  s >>= 5;
  for (int i = 6; i >= 0; --i) {
    rem[i] = s&0x7;
    s >>= 3;
  }
  for (int i = 6; i >= 0; --i) {
    up[i] = s&0x1;
    s >>= 1;
  }
}

double solve(ll s) {
  if (memo.find(s) == memo.end()) {
    double& ans = memo[s];

    int up[7], rem[8];
    decode(s, up, rem);

    bool done = true;
    for (int i = 0; i < 7; ++i) if (!up[i]) done = false;
    
    if (done) return ans = 0.0;

    int tot = 0;
    for (int i = 0; i < 8; ++i)
      if (rem[i]) {
        tot += rem[i];
        
        int t_up[7], t_rem[8];
        memcpy(t_up, up, sizeof(up));
        memcpy(t_rem, rem, sizeof(rem));

        --t_rem[i];
        int card = i;
        while (card < 7 && !t_up[card]) {
          t_up[card] = 1;
          card = val(dealt[card]);
        }
        ans += rem[i]*solve(encode(t_up, t_rem));
      }
    
    ans = 1.0 + ans/tot;
  }

  return memo[s];
}

int main() {
  cin >> dealt;

  int init_up[] = {0, 0, 0, 0, 0, 0, 0};
  int init_rem[] = {4, 4, 4, 4, 4, 4, 4, 24};

  for (char c : dealt) --init_rem[val(c)];

  cout << fixed << setprecision(10) << solve(encode(init_up, init_rem)) << endl;

  return 0;
}