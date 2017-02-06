#ifndef STATE_H
#define STATE_H

#include <vector>
#include "point.h"

class State {
  std::vector<Point> _route;
  std::vector<Point> _paint;
  int _initial_cost;

public:
  State() {};
  // Game::init() でのみセッタとして使う。後はゲッタとして使用する。
  std::vector<Point>& route() { return _route; }
  std::vector<Point>& paint() { return _paint; }
  int& initial_cost() { return _initial_cost; }
  
  Point start() const { return *_route.begin(); }
  Point goal() const { return *(_route.end()--); }
  bool does_paint() const { return !(_paint.empty()); }

  friend bool operator<(const State& left, const State& right) {
    return (left._initial_cost < right._initial_cost);
  }

  friend bool operator>(const State& left, const State& right) {
    return (left._initial_cost > right._initial_cost);
  }
};



#endif /* STATE_H */
