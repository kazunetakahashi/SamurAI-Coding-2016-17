#include "header/game.h"

#define DEBUG 0 // change 0 -> 1 if we need debug.

using namespace std;

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
  _turns[_now_turn].input();
  think();
  _turns[_now_turn].output();
}
