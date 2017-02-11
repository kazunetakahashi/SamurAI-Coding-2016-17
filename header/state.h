#ifndef STATE_H
#define STATE_H

#include <vector>
#include <string>
#include <set>
#include "point.h"

class State {
  // 行動の内容
  std::vector<Point> _route;
  std::vector<Point> _paint;
  std::set<Point> _set_paint;
  int _initial_cost;
  std::vector<int> _act;

  // 判断の枠組み evaluate.cpp
  double _paint_score;
  bool _need_hidden_to_revealed;
  bool _can_revealed_to_hidden;
  int _need_cost;
  std::set<int> _killed_enemy;
  double _estimated_kill_score;
  double _death_prob;

public:
  // 定数 evaluate.cpp
  static const double SCORE_PAINT_EMPTY;
  static const double SCORE_PAINT_ENEMY;
  
  State() {};
  State(Point p, int cost) {
    _route.push_back(p);
    _initial_cost = cost;
  };
  State(const State& state) {
    _route = state._route;
    _paint = state._paint;
    _set_paint = state._set_paint;
    _initial_cost = state._initial_cost;
    _act = state._act;
  }
  // Game::init() でのみセッタとして使う。後はゲッタとして使用する。
  // const でないと怒られるやつは friend で処理する。
  // 本当はよくない。名前分けたくないからこうする。
  std::vector<Point>& route() { return _route; }
  std::vector<Point>& paint() { return _paint; }
  std::set<Point>& set_paint() { return _set_paint; }
  int& initial_cost() { return _initial_cost; }
  std::vector<int>& act() { return _act; }

  double& paint_score() { return _paint_score; }
  bool& need_hidden_to_revealed() { return _need_hidden_to_revealed; }
  bool& can_revealed_to_hidden() { return _can_revealed_to_hidden; }
  int& need_cost() { return _need_cost; }
  std::set<int>& killed_enemy() { return _killed_enemy; }
  double& estimated_kill_score() { return _estimated_kill_score; }
  double& death_prob() { return _death_prob; }

  // よく使うやつ
  Point start() const { return *_route.begin(); }
  Point goal() const { return *(_route.end()-1); }
  bool does_paint() const { return !(_paint.empty()); }
  int confirmed_kill_score() const {
    return _killed_enemy.size();
  }
  // turn.cpp
  bool route_has_key(const std::set<Point>& S);

  friend bool operator<(const State& left, const State& right) {
    return (left._initial_cost < right._initial_cost);
  }

  friend bool operator>(const State& left, const State& right) {
    return (left._initial_cost > right._initial_cost);
  }

  friend std::ostream& operator<<(std::ostream& s, const State& state) {
    s << "Route: ";
    for (auto it = state._route.begin(); it != state._route.end(); ++it) {
      s << *it;
      if (it+1 != state._route.end()) {
        s << " -> ";
      }
    }
    s << std::endl;
    s << "Paint: ";
    for (auto it = state._paint.begin(); it != state._paint.end(); ++it) {
      s << *it;
      if (it+1 != state._paint.end()) {
        s << ", ";
      }
    }
    s << std::endl;
    s << "Cost: " << state._initial_cost << std::endl;
    s << "Act: ";
    for (auto it = state._act.begin(); it != state._act.end(); ++it) {
      s << *it;
      if (it+1 != state._act.end()) {
        s << " ";
      }
    }
    return s;
  }
};

#endif /* STATE_H */
