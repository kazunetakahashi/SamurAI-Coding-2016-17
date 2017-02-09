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

int State::count_kill_enemy(const set<Point>& S) {
  _kill_enemy = 0;
  for (auto it = _paint.begin(); it != _paint.end(); ++it) {
    if (S.find(*it) != S.end()) {
      ++_kill_enemy;
    }
  }
  return _kill_enemy;
}
