#include <set>
#include <iomanip>
#include <iostream>
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

void State::debug() {
  cerr << *this << endl;
  cerr << "_total_score : " << fixed << setprecision(3)
       << _total_score << endl;
  cerr << "_paint_score : " << _paint_score << endl;
  cerr << "_pre_paint_score : " << _pre_paint_score << endl;
  cerr << "_need_hidden_to_revealed : " << _need_hidden_to_revealed << endl;
  cerr << "_normal_hidden : " << _normal_hidden << endl;
  cerr << "_can_revealed_to_hidden : " << _can_revealed_to_hidden << endl;
  cerr << "_decided_revealed_to_hidden : "
       << _decided_revealed_to_hidden << endl;
  cerr << "_final_hidden : " << _final_hidden << endl;
  cerr << "_need_cost : " << _need_cost << endl;
  cerr << "_killed_enemy : ";
  for (auto x : _killed_enemy) {
    cerr << x << " ";
  }
  cerr << endl;
  cerr << "_estimated_kill_score : " << _estimated_kill_score << endl;
  cerr << "_total_death_prob_revealed : "
       << _total_death_prob_revealed << endl;
  cerr << "_total_death_prob_hidden : " << _total_death_prob_hidden << endl;
  cerr << "_final_death_prob : " << _final_death_prob << endl;
}
