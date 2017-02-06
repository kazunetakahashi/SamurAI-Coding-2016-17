#include <vector>
#include "header/game.h"

using namespace std;

const int Game::TURN = 96;
const int Game::MY_TURN = TURN/2;
const int Game::PLAYER = 3;
const int Game::SAMURAI = Game::PLAYER * 2;
const int Game::PERIOD = Game::TURN/Game::SAMURAI;
const int Game::FIELD = 15;
const int Game::COST[10] = {0, 4, 4, 4, 4, 2, 2, 2, 2, 1};
const int Game::MAX_COST = 7;
const int Game::HOUGAKU = 4;

const vector<Point> YARI = {
  Point(0, 1),
  Point(0, 2),
  Point(0, 3),
  Point(0, 4),
};
const vector<Point> KATANA = {
  Point(0, 1),
  Point(0, 2),
  Point(1, 0),
  Point(1, 1),
  Point(2, 0),
};
const vector<Point> MASAKARI = {
  Point(-1, -1),
  Point(-1, 0),
  Point(-1, 1),
  Point(0, 1),
  Point(1, -1),
  Point(1, 0),
  Point(1, 1),
};
const vector<Point> Game::PAINT[3] = {YARI, KATANA, MASAKARI};

