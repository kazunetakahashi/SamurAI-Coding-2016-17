#include <iostream>
#include "header/game.h"

using namespace std;

Field::Field() {
  _value = new int*[Game::FIELD];
  for (auto i = 0; i < Game::FIELD; ++i) {
    _value[i] = new int[Game::FIELD];
  }
}

Field::~Field() {
  for (auto i = 0; i < Game::FIELD; ++i) {
    delete[] _value[i];
  }
  delete[] _value;
}

void Field::input() {
  for (auto i = 0; i < Game::FIELD; ++i) {
    for (auto j = 0; j < Game::FIELD; ++j) {
      cin >> _value[i][j];
    }
  }
}
