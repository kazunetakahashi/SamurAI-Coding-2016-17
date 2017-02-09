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
  int _turn_num;
  Point* _point_samurai;
  bool* _has_done;
  bool* _is_hidden;
  int* _treat_num;
  Field _field;

  // 入力から作るもの
  std::set<Point> _set_point_samurai;

  // 思考用
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
  std::vector<State>& states(int player) { return _states[player]; }
  int& actor() { return _actor; }
  bool& hidden_to_revealed() { return _hidden_to_revealed; }
  bool& revealed_to_hidden() { return _revealed_to_hidden; }
  std::vector<int>& act() { return _act; }

  // 思考用メンバ関数
  void remove_prohibited_states();
  
};

#endif /* TURN_H */
