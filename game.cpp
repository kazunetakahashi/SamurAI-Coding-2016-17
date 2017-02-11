#include "header/game.h"

#define DEBUG 0 // change 0 -> 1 if we need debug.

using namespace std;

Game::Game() : _now_turn(0) {
  _turns = new Turn[MY_TURN];
};

Game::~Game() {
  delete[] _turns;
}


void Game::input() {
  int t;
  cin >> t;
  _is_sente = (t == 0);
}

void Game::play() {
  // 準備 (10秒) 
  before_start();
  // 完了
  cout << 0 << endl;
  // 各ターン
  for (; _now_turn < MY_TURN; ++_now_turn) {
    each_turn();
  }
}

void Game::before_start() {
  // いまのところなし
}

void Game::each_turn() {
  current().input();
  for (auto i = 0; i < Game::SAMURAI; ++i) {
    if (current().treat_num(i) > 0) {
      current().point_samurai(i)
        = Game::HOUSE[player_to_samurai(i%3, (i<=2))];
      current().is_hidden(i) = false;
    }
  }
  think();
  current().output();
}
