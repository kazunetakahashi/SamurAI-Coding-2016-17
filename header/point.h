#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point {
  int _x, _y;
  
public:
  Point() {};
  Point(int x, int y) : _x(x), _y(y) {};
  
  int x() const { return _x; };
  int y() const { return _y; };

  Point operator+() const { return *this; }
  Point operator-() const { return Point(-_x, -_y); }

  Point& operator+=(const Point& p) {
    _x += p._x;
    _y += p._y;
    return *this;
  }

  Point& operator-=(const Point& p) {
    _x -= p._x;
    _y -= p._y;
    return *this;
  }

  Point operator+(const Point& p) const {
    return Point(_x + p._x, _y + p._y);
  }
  
  Point operator-(const Point& p) const {
    return Point(_x - p._x, _y - p._y);
  }

  bool operator==(const Point& p) const {
    return (_x == p._x && _y == p._y);
  }

  bool operator!=(const Point& p) const {
    return !(*this == p);
  }

  Point inverse();
  bool is_valid();
  
};

inline std::istream& operator>>(std::istream& s, Point& p) {
  int x, y;
  s >> x >> y;
  p = Point(x, y);
  return s;
}

inline std::ostream& operator<<(std::ostream& s, const Point& p) {
  return s << "(" << p.x() << ", " << p.y() << ")";
}

#endif /* POINT_H */
