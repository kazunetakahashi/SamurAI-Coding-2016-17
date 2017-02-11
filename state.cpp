#include <set>
#include "header/game.h"

using namespace std;

#define DEBUG 0

bool State::route_has_key(const set<Point>& S) {
  // _route が empty ということはない。
  // 速度のためチェックもしない。
  for (auto it = _route.begin()+1; it != _route.end(); ++it) {
    if (S.find(*it) != S.end()) {
      return true;
    }
  }
  return false;
}
