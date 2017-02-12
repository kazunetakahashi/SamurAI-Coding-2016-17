#ifndef STATE_H
#define STATE_H

#include <vector>
#include <string>
#include <set>
#include "point.h"

class State {
  // 行動の内容
  int _player;
  int _samurai;
  std::vector<Point> _route;
  std::vector<Point> _paint;
  std::set<Point> _set_paint;
  int _initial_cost;
  std::vector<int> _act;

  // 判断の枠組み evaluate.cpp
  double _total_score;
  double _paint_score;
  double _pre_paint_score;
  bool _need_hidden_to_revealed;
  bool _normal_hidden;
  bool _can_revealed_to_hidden;
  bool _decided_revealed_to_hidden;
  bool _final_hidden;
  int _need_cost;
  std::set<int> _killed_enemy;
  double _estimated_kill_score;
  double _total_death_prob_revealed;
  double _total_death_prob_hidden;
  double _final_death_prob;
  double _total_eye_prob;
  double _final_place_score;

public:
  // 定数 inform.cpp
  static const int MAX_PLACE_ENEMY;
  static const double MIN_BASE_PROB;
  // 定数 evaluate.cpp
  static const double SCORE_PAINT_EMPTY;
  static const double SCORE_PAINT_ENEMY;
  static const double SCORE_PRE_PAINT_UNKNOWN;
  static const double SCORE_PRE_PAINT_EMPTY;
  static const double SCORE_PRE_PAINT_ENEMY;
  static const double MAX_DEATH_PROB;
  static const double MAX_EYE_PROB;
  static const double RATE_ESTIMATED_KILL;
  static const double RATE_CONFIRMED_KILL;
  static const double RATE_DEATH;
  static const double RATE_PLACE;
  static const Point TARGET_POINT[];
  
  State() {};
  State(int player, int samurai, Point p, int cost)
    : _player(player), _samurai(samurai), _initial_cost(cost) {
    _route.push_back(p);
  };
  State(const State& state) {
    _player = state._player;
    _samurai = state._samurai;
    _route = state._route;
    _paint = state._paint;
    _set_paint = state._set_paint;
    _initial_cost = state._initial_cost;
    _act = state._act;
  }
  // Game::init() でのみセッタとして使う。後はゲッタとして使用する。
  // const でないと怒られるやつは friend で処理する。
  // 本当はよくない。名前分けたくないからこうする。
  int& player() { return _player; }
  int& samurai() { return _samurai; }
  std::vector<Point>& route() { return _route; }
  std::vector<Point>& paint() { return _paint; }
  std::set<Point>& set_paint() { return _set_paint; }
  int& initial_cost() { return _initial_cost; }
  std::vector<int>& act() { return _act; }

  double& total_score() { return _total_score; }
  double& paint_score() { return _paint_score; }
  double& pre_paint_score() { return _pre_paint_score; }
  bool& need_hidden_to_revealed() { return _need_hidden_to_revealed; }
  bool& normal_hidden() { return _normal_hidden; }
  bool& can_revealed_to_hidden() { return _can_revealed_to_hidden; }
  bool& decided_revealed_to_hidden() {
    return _decided_revealed_to_hidden;
  }
  bool& final_hidden() { return _final_hidden; }
  int& need_cost() { return _need_cost; }
  std::set<int>& killed_enemy() { return _killed_enemy; }
  double& estimated_kill_score() { return _estimated_kill_score; }
  double confirmed_kill_score() const {
    return (double)_killed_enemy.size();
  }
  double& total_death_prob_revealed() {
    return _total_death_prob_revealed;
  }
  double& total_death_prob_hidden() {
    return _total_death_prob_hidden;
  }
  double& final_death_prob() {
    return _final_death_prob;
  }
  double& total_eye_prob() {
    return _total_eye_prob;
  }
  double& final_place_score() { return _final_place_score; }
  // スコアの低い方から順に並べる choose.cpp
  // 高い方から並べるには reverse する。
  // 今のところ使っていない。
  friend bool sort_by_score(const State& left, const State& right) {
    return left._total_score < right._total_score;
  }

  // よく使うやつ
  Point start() const { return *_route.begin(); }
  Point goal() const { return *(_route.end()-1); }
  bool does_paint() const { return !(_paint.empty()); }
  // turn.cpp
  bool route_has_key(const std::set<Point>& S);

  friend bool operator<(const State& left, const State& right) {
    return (left._initial_cost < right._initial_cost);
  }

  friend bool operator>(const State& left, const State& right) {
    return (left._initial_cost > right._initial_cost);
  }

  friend std::ostream& operator<<(std::ostream& s,
                                  const State& state) {
    s << "Route: ";
    for (auto it = state._route.begin();
         it != state._route.end(); ++it) {
      s << *it;
      if (it+1 != state._route.end()) {
        s << " -> ";
      }
    }
    s << std::endl;
    s << "Paint: ";
    for (auto it = state._paint.begin();
         it != state._paint.end(); ++it) {
      s << *it;
      if (it+1 != state._paint.end()) {
        s << ", ";
      }
    }
    s << std::endl;
    s << "Cost: " << state._initial_cost << std::endl;
    s << "Act: ";
    for (auto it = state._act.begin();
         it != state._act.end(); ++it) {
      s << *it;
      if (it+1 != state._act.end()) {
        s << " ";
      }
    }
    return s;
  }

  void debug();
};

#endif /* STATE_H */
