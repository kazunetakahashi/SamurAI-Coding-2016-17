#ifndef FIELD_H
#define FIELD_H

#include "point.h"

class Field {
  int** _value;

public:
  Field();
  ~Field();
  
  void input();
  int value(int i, int j) { return _value[i][j]; }
  int value(Point p) { return _value[p.x()][p.y()]; }
  
};

#endif /* FIELD_H */
