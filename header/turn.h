#ifndef TURN_H
#define TURN_H

#include <iostream>
#include <vector>
#include "point.h"
#include "field.h"

class Turn {
  // 入力
  int _turn_num;
  Point* _point_samurai;
  bool* _has_done;
  bool* _is_hidden;
  int* _treat_num;
  Field _field;

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
  Point* point_samurai() { return _point_samurai; }
  bool* has_done() { return _has_done; }
  bool* is_hidden() { return _is_hidden; }
  int* treat_num() { return _treat_num; };
  Field& field() { return _field; }
  int& actor() { return _actor; }
  std::vector<int>& act() { return _act; }
  
};

#endif /* TURN_H */
