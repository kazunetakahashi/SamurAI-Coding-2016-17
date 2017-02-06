#include <iostream>
#include <vector>
#include "header/game.h"

using namespace std;

int main() {
  Game game;
  Game::init();
  game.input();
  game.play();
}
