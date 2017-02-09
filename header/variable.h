#ifndef VARIABLE_H
#define VARIABLE_H

#include <iostream>
#include <string>

class Variable {
  std::string _str;
  int _x, _y, _z;

public:

  Variable(std::string str, int x = -1, int y = -1, int z = -1)
    : _str(str), _x(x), _y(y), _z(z) {};

  std::string str() const { return _str; }
  int x() const { return _x; }
  int y() const { return _y; }
  int z() const { return _z; }

  bool operator==(const Variable& v) const {
    return (_str == v._str
            && _x == v._x
            && _y == v._y
            && _z == v._z);
  }

  bool operator!=(const Variable& v) const {
    return !(*this == v);
  }

  bool operator<(const Variable& v) const {
    if (_str != v._str) return _str < v._str;
    if (_x != v._x) return _x < v._x;
    if (_y != v._y) return _y < v._y;
    return _z < v._z;
  }
  
  bool operator>(const Variable& v) const {
    if (_str != v._str) return _str > v._str;
    if (_x != v._x) return _x > v._x;
    if (_y != v._y) return _y > v._y;
    return _z > v._z;
  }
  
};

inline std::ostream& operator<<(std::ostream& s, const Variable& v) {
  s << v.str() << "(";
  if (v.x() != -1) s << v.x();
  if (v.y() != -1) s << ", " << v.y();
  if (v.z() != -1) s << ", " << v.z();
  return s << ")";
}

#endif /* VARIABLE_H */
