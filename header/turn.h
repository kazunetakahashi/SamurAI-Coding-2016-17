#ifndef TURN_H
#define TURN_H

#include <iostream>
#include <vector>
#include "point.h"
#include "field.h"

class Turn {
  int _turn_num;
  Point* _point_samurai;
  bool* _has_done;
  bool* _is_hidden;
  int* _treat_num;
  Field _field;

  int actor;
  std::vector<int> act;

public:
  Turn();
  ~Turn();
  void input();
  void output();
  
};

#endif /* TURN_H */
