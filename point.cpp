#include "header/game.h"

Point Point::inverse() const {
  return Point(Game::FIELD - _x, Game::FIELD - _y);
}

bool Point::is_valid() const {
  return (0 <= _x && _x < Game::FIELD && 0 <= _y && _y < Game::FIELD);
}

Point Point::rotate(int i) const {
  if (i == 0) return *this;
  if (i == 1) return Point(-_y, _x);
  if (i == 2) return -*this;
  return Point(_y, -_x);
}
