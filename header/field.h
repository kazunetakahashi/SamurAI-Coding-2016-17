#ifndef FIELD_H
#define FIELD_H

#include "point.h"

class Field {
  int** _value;

public:
  Field();
  ~Field();
  
  void input();
  int value(int i, int j) const { return _value[i][j]; }
  int value(Point p) const { return value(p.x(), p.y()); }

  // 面倒だからここに定数も書く
  bool is_perceived(int i, int j) const {
    return !(_value[i][j] == 9);
  }
  bool is_perceived(Point p) const {
    return is_perceived(p.x(), p.y());
  }
  bool is_occupied_by_player(int i, int j) {
    return (0 <= _value[i][j] && _value[i][j] <= 2);
  }
  bool is_occupied_by_player(Point p) {
    return is_occupied_by_player(p.x(), p.y());
  }
  bool is_occupied_by_enemy(int i, int j) {
    return (3 <= _value[i][j] && _value[i][j] <= 5);
  }
  bool is_occupied_by_enemy(Point p) {
    return is_occupied_by_enemy(p.x(), p.y());
  }
  bool is_enemy_color(int player, int i, int j) {
    return (_value[i][j] == player+3);
  }
  bool is_enemy_color(int player, Point p) {
    return is_enemy_color(player, p.x(), p.y());
  }
};

#endif /* FIELD_H */
