#ifndef TURN_H
#define TURN_H

#include <iostream>
#include <vector>
#include <set>
#include "point.h"
#include "field.h"
#include "state.h"

class Turn {
  // 入力
  int _turn_num; // 使わない
  Point* _point_samurai;
  bool* _has_done;
  bool* _is_hidden;
  int* _treat_num;
  Field _field;

  // 入力から作るもの
  std::set<Point> _set_point_samurai;
  std::set<Point> _set_point_enemy;

  // 前回までの入力も踏まえて作るもの
  int _acted_enemy;
  bool**** _kappa;
  std::vector<Point>* _point_enemy;
  bool* _is_remained;

  // 思考用
  bool** _beta;
  std::vector<Point> _gamma;
  std::vector<Point> _delta;
  std::vector<State>* _states;

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
  Point point_samurai(int samurai) { return _point_samurai[samurai]; }
  bool is_visible(int samurai);
  bool has_done(int samurai) { return _has_done[samurai]; }
  bool is_hidden(int samurai) { return _is_hidden[samurai]; }
  int treat_num(int samurai) { return _treat_num[samurai]; };
  int field(Point p) { return _field.value(p); }
  
  std::set<Point>& set_point_samurai() { return _set_point_samurai; }
  std::set<Point>& set_point_enemy() { return _set_point_enemy; }

  int& acted_enemy() { return _acted_enemy; }
  bool& kappa(int type, int player, int x, int y) {
    return _kappa[type][player][x][y];
  }
  bool& kappa(int type, int player, Point p) {
    return kappa(type, player, p.x(), p.y());
  }

  std::vector<Point>& point_enemy(int player) {
    return _point_enemy[player];
  }
  bool& is_remained(int player) {
    return _is_remained[player];
  }

  bool& beta(int i, int j) { return _beta[i][j]; }
  bool& beta(Point p) { return beta(p.x(), p.y()); }
  std::vector<Point>& gamma() { return _gamma; }
  std::vector<Point>& delta() { return _delta; }
  std::vector<State>& states(int player) { return _states[player]; }
  int& actor() { return _actor; }
  bool& hidden_to_revealed() { return _hidden_to_revealed; }
  bool& revealed_to_hidden() { return _revealed_to_hidden; }
  std::vector<int>& act() { return _act; }

  // 思考用メンバ関数
  void remove_prohibited_states();
  void calc_kill_enemy();

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


  // メンバ関数で表現されるギリシア文字
  bool lambda(int i, int j) {
    return is_perceived(i, j) && !is_occupied_by_enemy(i, j);
  }
  bool lambda(Point p) {
    return lambda(p.x(), p.y());
  }

};

#endif /* TURN_H */
