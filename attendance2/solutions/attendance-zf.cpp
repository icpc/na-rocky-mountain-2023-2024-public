#include <bits/stdc++.h>

using namespace std;

int main() {
  int N;
  cin >> N;

  string prev = "";
  int absent = 0;

  for (int i = 0; i < N; ++i) {
    string call;
    cin >> call;
    if (call == "Present!") prev = "";
    else {
      if (prev != "") {
        cout << prev << endl;
        ++absent;
      }
      prev = call;
    }
  }
  if (prev != "") {
    cout << prev << endl;
    ++absent;
  }
  
  if (absent == 0) cout << "No Absences" << endl;

  return 0;
}