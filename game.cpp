#include <iostream>
#include <queue>

#include "header/game.h"

using namespace std;

void init() {
  // Game::rotate_paint の準備
  Game::rotate_paint = new vector<Point>*[Game::PLAYER];
  for (auto i = 0; i < Game::PLAYER; ++i) {
    Game::rotate_paint[i] = new vector<Point>[Game::HOUGAKU];
    for (auto j = 0; j < Game::HOUGAKU; ++j) {
      for (auto x : Game::PAINT[i]) {
        Game::rotate_paint[i][j].push_back(x.rotate(j));
      }
    }
  }
  // Game::initial_state の準備
  Game::initial_state = new vector<State>**[Game::PLAYER];
  for (auto k = 0; k < Game::PLAYER; ++k) {
    Game::initial_state[k] = new vector<State>*[Game::FIELD];
    for (auto i = 0; i < Game::FIELD; ++i) {
      Game::initial_state[k][i] = new vector<State>[Game::FIELD];
      for (auto j = 0; j < Game::FIELD; ++j) {
        priority_queue<State, vector<State>, greater<State> > Q;
        
      }
    }
  }
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
