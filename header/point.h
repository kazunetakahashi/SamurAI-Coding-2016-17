#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <string>
#include <cmath>

class Point {
  int _x, _y;
  
public:
  Point() {};
  Point(int x, int y) : _x(x), _y(y) {};
  
  int x() const { return _x; }
  int y() const { return _y; }

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

  bool operator<(const Point& p) const {
    if (_x == p._x) return _y < p._y;
    return _x < p._x;
  }

  bool operator>(const Point& p) const {
    if (_x == p._x) return _y > p._y;
    return _x > p._x;
  }

  int dist(const Point& p) const {
    return std::abs(_x - p._x) + std::abs(_y - p._y);
  }

  Point inverse() const;
  Point rotate(int i) const;
  bool is_valid() const;
  bool is_paintable() const;
  bool is_movable(int samurai) const;
  
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
