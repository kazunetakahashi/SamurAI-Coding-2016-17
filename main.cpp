#include <iostream>
#include <vector>
#include "header/game.h"

#define DEBUG 0 // change 0 -> 1 if we need debug.

using namespace std;

int main() {
  Game game;
  Game::init();
  game.input();
  game.play();
}
