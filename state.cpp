#include <set>
#include "header/game.h"

using namespace std;

#define DEBUG 0

bool State::route_has_key(const set<Point>& S) {
  // _route が empty ということはない。
  // 速度のためチェックもしない。
#if DEBUG == 1
  cerr << "_route : ";
  for (auto x : _route) {
    cerr << x;
  }
  cerr << endl;
#endif
  for (auto it = _route.begin()+1; it != _route.end(); ++it) {
#if DEBUG == 1
    cerr << *it << " ";    
#endif
    if (S.find(*it) != S.end()) {
#if DEBUG == 1
      cerr << "found" << endl;    
#endif
      return true;
    }
  }
#if DEBUG == 1
  cerr << "not founded" << endl;
#endif
  return false;
}
