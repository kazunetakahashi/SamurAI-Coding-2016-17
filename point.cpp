#include "header/game.h"

Point Point::inverse() {
  return Point(Game::FIELD - _x, Game::FIELD - _y);
}

bool Point::is_valid() {
  return (0 <= _x && _x < Game::FIELD && 0 <= _y && _y < Game::FIELD);
}
