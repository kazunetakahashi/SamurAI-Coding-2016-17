#ifndef STATE_H
#define STATE_H

#include <vector>
#include <string>
#include <set>
#include "point.h"

class State {
  std::vector<Point> _route;
  std::vector<Point> _paint;
  int _initial_cost;
  std::vector<int> _act;

public:
  State() {};
  State(Point p, int cost) {
    _route.push_back(p);
    _initial_cost = cost;
  };
  State(const State& state) {
    _route = state._route;
    _paint = state._paint;
    _initial_cost = state._initial_cost;
    _act = state._act;
  }
  // Game::init() でのみセッタとして使う。後はゲッタとして使用する。
  // const でないと怒られるやつは friend で処理する。
  // 本当はよくない。名前分けたくないからこうする。
  std::vector<Point>& route() { return _route; }
  std::vector<Point>& paint() { return _paint; }
  int& initial_cost() { return _initial_cost; }
  std::vector<int>& act() { return _act; }
  
  Point start() const { return *_route.begin(); }
  Point goal() const { return *(_route.end()-1); }
  bool does_paint() const { return !(_paint.empty()); }
  
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
