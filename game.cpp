#include <iostream>
#include "header/game.h"

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
  for (; now_turn < MY_TURN; ++now_turn) {
    each_turn();
  }
}

void Game::before_start() {
  // いまのところなし
}

void Game::each_turn() {
  turns[now_turn].input();
  think();
  turns[now_turn].output();
}

void Game::think() {
  // とりあえず空欄
}
