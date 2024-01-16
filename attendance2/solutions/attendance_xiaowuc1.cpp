#include <iostream>
#include <vector>

using namespace std;

int main() {
  vector<string> ret;
  int n;
  cin >> n;
  vector<string> all(n);
  for(auto& x: all) cin >> x;
  for(int i = 0; i < n;) {
    if(i+1 == n || all[i+1] != "Present!") {
      ret.push_back(all[i]);
      i++;
    }
    else i += 2;
  }
  if(ret.size() == 0) ret.push_back("No Absences");
  for(auto out: ret) cout << out << "\n";
}