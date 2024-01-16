#include <array>
#include <cassert>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

using namespace std;

const int64_t DEPART = -1;
const int64_t ARRIVE = 1;

int main() {
  int pumps, q;
  cin >> pumps >> q;
  vector<priority_queue<array<int, 2>, vector<array<int, 2>>, greater<array<int, 2>>>> pumpstatus(2);
  vector<queue<array<int64_t, 2>>> queued(2*pumps);
  vector<array<bool, 2>> inuse(2*pumps);
  // <time, type>, (DEPART -> location, innerindex), (ARRIVE -> parity, fuelup), true index
  vector<int64_t> departtimes(q, -1);
  priority_queue<array<int64_t, 5>, vector<array<int64_t, 5>>, greater<array<int64_t, 5>>> eventq;
  for(int i = 0; i < 2; i++) for(int j = 0; j < pumps; j++) pumpstatus[i].push({-1, 2*j+i});
  for(int i = 0; i < q; i++) {
    int arrive, fuel;
    char ch;
    cin >> arrive >> fuel >> ch;
    eventq.push({arrive, ARRIVE, ch == 'R', fuel, i});
  }
  auto forceinsert = [&](int64_t currt, array<int64_t, 2> fuelt, int64_t loc) -> void {
    assert(!inuse[loc][0]);
    if(!inuse[loc][1]) {
      inuse[loc][1] = true;
      eventq.push({currt+fuelt[0], DEPART, loc, int64_t(1), fuelt[1]});
    }
    else {
      inuse[loc][0] = true;
      eventq.push({currt+fuelt[0], DEPART, loc, int64_t(0), fuelt[1]});
    }
  };
  while(eventq.size()) {
    auto [time, type, lhs, rhs, retidx] = eventq.top(); eventq.pop();
    if(type == ARRIVE) {
      int idx = lhs%2;
      int fuelt = rhs;
      while(true) {
        assert(pumpstatus[idx].size());
        auto [expected, loc] = pumpstatus[idx].top(); pumpstatus[idx].pop();
        if(expected == -1) {
          if(!inuse[loc][0]) {
            forceinsert(time, {fuelt, retidx}, loc);
            pumpstatus[idx].push({-1, loc});
            break;
          }
          else {
            pumpstatus[idx].push({int(queued[loc].size()), loc});
            continue;
          }
        }
        assert(inuse[loc][0]);
        if(expected != queued[loc].size()) continue;
        queued[loc].push({fuelt, retidx});
        pumpstatus[idx].push({int(queued[loc].size()), loc});
        break;
      }
    }
    else {
      assert(type == DEPART);
      int timeleave = time;
      set<int> locs;
      while(true) {
        departtimes[retidx] = timeleave;
        int loc = lhs;
        int pumpidx = rhs;
        locs.insert(loc);
        assert(inuse[loc][pumpidx]);
        inuse[loc][pumpidx] = false;
        if(eventq.size() && eventq.top()[0] == timeleave && eventq.top()[1] == DEPART) {
          lhs = eventq.top()[2];
          rhs = eventq.top()[3];
          retidx = eventq.top()[4];
          eventq.pop();
        }
        else break;
      }
      for(int locupd: locs) {
        while(queued[locupd].size() && !inuse[locupd][0]) {
          auto fuelt = queued[locupd].front(); queued[locupd].pop();
          forceinsert(time, fuelt, locupd);
        }
        int nexpected = int(queued[locupd].size());
        if(nexpected == 0) nexpected = -1;
        pumpstatus[locupd%2].push({nexpected, locupd});
      }
    }
  }
  for(int i = 0; i < q; i++) cout << departtimes[i] << "\n";
}