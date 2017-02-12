#ifndef TURN_H
#define TURN_H

#include <iostream>
#include <vector>
#include <set>
#include <iomanip>
#include "point.h"
#include "field.h"
#include "state.h"

enum Kappatype { _Lying_, _Painted_, _Watched_, _Kept_,
                 _Kappatypesize_ };

class Turn {
  // 入力
  int _turn_num; // 使わない
  Point* _point_samurai;
  bool* _has_done;
  bool* _is_hidden;
  int* _treat_num;
  Field _field;

  // 入力から作るもの
  bool* _is_killed;
  std::set<Point> _set_point_samurai;
  std::set<int> _done_enemy;

  // 前回までの入力も踏まえて作るもの
  int _acted_enemy; // 0...2 player 型
  std::vector<Point>* _point_enemy;
  std::set<Point>* _set_point_enemy;
  double* _base_prob;
  bool* _is_remained;

  // 思考用
  bool** _beta;
  std::vector<Point> _gamma;
  std::vector<Point> _delta;
  bool**** _kappa;
  std::vector<State>* _states;
  double*** _table_pre_paint_score;
  double* _initial_death_prob;

  // 出力
  int _actor;
  bool _hidden_to_revealed;
  bool _revealed_to_hidden;
  std::vector<int> _act;

public:
  Turn();
  ~Turn();
  void input();
  void output();

  // ゲッタとセッタ
  Point& point_samurai(int samurai) { return _point_samurai[samurai]; }
  bool is_visible(int samurai);
  bool has_done(int samurai) { return _has_done[samurai]; }
  bool& is_hidden(int samurai) { return _is_hidden[samurai]; }
  int treat_num(int samurai) { return _treat_num[samurai]; }
  int field(Point p) { return _field.value(p); }
  
  bool is_killed(int samurai) { return _is_killed[samurai]; }
  std::set<Point>& set_point_samurai() { return _set_point_samurai; }
  std::set<int>& done_enemy() { return _done_enemy; }

  int& acted_enemy() { return _acted_enemy; }
  std::vector<Point>& point_enemy(int player) {
    return _point_enemy[player];
  }
  std::set<Point>& set_point_enemy(int player) {
    return _set_point_enemy[player];
  }
  double& base_prob(int player) {
    return _base_prob[player];
  }
  bool& is_remained(int player) {
    return _is_remained[player];
  }

  bool& beta(int i, int j) { return _beta[i][j]; }
  bool& beta(Point p) { return beta(p.x(), p.y()); }
  std::vector<Point>& gamma() { return _gamma; }
  std::vector<Point>& delta() { return _delta; }
  bool& kappa(Kappatype type, int player, int x, int y) {
    return _kappa[type][player][x][y];
  }
  bool& kappa(Kappatype type, int player, Point p) {
    return kappa(type, player, p.x(), p.y());
  }
  std::vector<State>& states(int player) { return _states[player]; }
  double& table_pre_paint_score(int player, int x, int y) {
    return _table_pre_paint_score[player][x][y];
  }
  double& table_pre_paint_score(int player, const Point p) {
    return table_pre_paint_score(player, p.x(), p.y());
  }
  double& initial_death_prob(int player) {
    return _initial_death_prob[player];
  }
  
  int& actor() { return _actor; }
  bool& hidden_to_revealed() { return _hidden_to_revealed; }
  bool& revealed_to_hidden() { return _revealed_to_hidden; }
  std::vector<int>& act() { return _act; }

  // eveluate.cpp
  void evaluate();
  void remove_prohibited_states();
  void calc_paint_score(State& state);
  double calc_pre_paint_score(State& state);
  void calc_hidden_booleans(State& state);
  void calc_kill_score(State& state);
  void calc_death_prob(State& state);
  void calc_eye_prob(State& state);
  void calc_place_score(State& state);
  void calc_total_score(State& state);
  // death_prob.cpp
  static double death_prob(int player,
                           Point p, Point q, bool is_hidden);
  static double eye_prob(Point p, Point q);
  static double target_dist(int samurai, Point p);
  // choose.cpp
  void choose();
  void set_act(int player, std::vector<int> act, bool h_r, bool r_h);
  void trivial_act(int player);
  void choose_actor_and_act();
  void calc_initial_death_prob();

  // Field からの輸入
  bool is_perceived(int i, int j) const {
    return _field.is_perceived(i, j);
  }
  bool is_perceived(Point p) const {
    return is_perceived(p.x(), p.y());
  }
  bool is_occupied_by_player(int i, int j) {
    return _field.is_occupied_by_player(i, j);
  }
  bool is_occupied_by_player(Point p) {
    return is_occupied_by_player(p.x(), p.y());
  }
  bool is_occupied_by_enemy(int i, int j) {
    return _field.is_occupied_by_enemy(i, j);
  }
  bool is_occupied_by_enemy(Point p) {
    return is_occupied_by_enemy(p.x(), p.y());
  }
  bool is_enemy_color(int player, int i, int j) {
    return _field.is_enemy_color(player, i, j);
  }
  bool is_enemy_color(int player, Point p) {
    return is_enemy_color(player, p.x(), p.y());
  }

  friend std::ostream& operator<<(std::ostream& s,
                                  const Turn& turn) {
    s << "_point_samurai : ";
    for (auto i = 0; i < 6; ++i) {
      s << turn._point_samurai[i] << " ";
    }
    s << std::endl;
    s << "_has_done : ";
    for (auto i = 0; i < 6; ++i) {
      s << turn._has_done[i] << " ";
    }
    s << std::endl;
    s << "_is_hidden : ";
    for (auto i = 0; i < 6; ++i) {
      s << turn._is_hidden[i] << " ";
    }
    s << std::endl;
    s << "_treat_num : ";
    for (auto i = 0; i < 6; ++i) {
      s << turn._treat_num[i] << " ";
    }
    s << std::endl;
    s << "_is_killed : ";
    for (auto i = 0; i < 6; ++i) {
      s << turn._is_killed[i] << " ";
    }
    s << std::endl;
    s << "_set_point_samurai : ";
    for (auto x : turn._set_point_samurai) {
      s << x << " ";
    }
    s << std::endl;
    s << "_acted_enemy : " << turn._acted_enemy << std::endl;
    for (auto i = 0; i < 3; ++i) {
      s << "_point_enemy[" << i << "] : ";
      for (auto x : turn._point_enemy[i]) {
        s << x << " ";
      }
      s << std::endl;
    }
    /*
    for (auto i = 0; i < 3; ++i) {
      s << "_set_point_enemy[" << i << "] : ";
      for (auto x : turn._set_point_enemy[i]) {
        s << x << " ";
      }
      s << std::endl;
    }
    */
    for (auto i = 0; i < 3; ++i) {
      s << "_base_prob[" << i << "] : "
        << std::fixed << std::setprecision(3)
        << turn._base_prob[i] << std::endl;
    }
    for (auto i = 0; i < 3; ++i) {
      s << "_is_remained[" << i << "] : "
        << turn._is_remained[i] << std::endl;
    }
    return s;
  }

};

#endif /* TURN_H */
