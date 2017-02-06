#include <iostream>
#include "header/game.h"

const int Game::TURN = 96;
const int Game::MY_TURN = TURN/2;
const int Game::PLAYER = 3;
const int Game::SAMURAI = Game::PLAYER * 2;
const int Game::PERIOD = Game::TURN/Game::SAMURAI;
const int Game::FIELD = 15;
const int Game::COST[10] = {0, 4, 4, 4, 4, 2, 2, 2, 2, 1};
const int Game::MAX_COST = 7;

int main() {
  Game game;
  game.input();
  game.play();
}
