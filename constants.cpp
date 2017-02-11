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
const int Game::DIRECTION = 4;

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

const Point Game::HOUSE[6] = {
  Point(0, 0),
  Point(0, 7),
  Point(7, 0),
  Point(14, 14),
  Point(14, 7),
  Point(7, 14)
};

// evaluate.cppで定義
// const int Game::REACH[3] = {5, 3, 2};

const Point Game::INVISIBLE = Point(-1, -1);

const Point Game::dx[4] = {
  Point(0, 1),
  Point(1, 0),
  Point(0, -1),
  Point(-1, 0),
};

int Game::command(int dir, Command obj) {
  if (obj == _Hide_) {
    return 9;
  } else if (obj == _Paint_) {
    return dir + 1;
  } else { // obj == _Move_
    return dir + 5;
  }
}

int Game::samurai_to_player(int samurai) {
  return samurai%3;
}

int Game::player_to_samurai(int player, bool is_mikata) {
  if ((_is_sente && is_mikata) || (!_is_sente && !is_mikata)) {
    return player;
  } else {
    return player + 3;
  }
}

int Game::player_to_enemy(int player) {
  return player + 3;
}
