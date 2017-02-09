#include <iostream>
#include "header/game.h"

using namespace std;

#define DEBUG 0

Turn::Turn() : _field() {
  _point_samurai = new Point[Game::SAMURAI];
  _has_done = new bool[Game::SAMURAI];
  _is_hidden = new bool[Game::SAMURAI];
  _treat_num = new int[Game::SAMURAI];
  _states = new vector<State>[Game::PLAYER];
}

Turn::~Turn() {
  delete[] _point_samurai;
  delete[] _has_done;
  delete[] _is_hidden;
  delete[] _treat_num;
  delete[] _states;
}

void Turn::input() {
  // 入力
  cin >> _turn_num;
  for (auto i = 0; i < Game::SAMURAI; ++i) {
    cin >> _point_samurai[i];
    int done, hidden;
    cin >> done >> hidden;
    _has_done[i] = (done == 1);
    _is_hidden[i] = (hidden == 1);
    cin >> _treat_num[i];
  }
  _field.input();
  // 入力から作るもの
  for (auto i = 0; i < Game::SAMURAI; ++i) {
    if (is_visible(i)) {
      _set_point_samurai.insert(point_samurai(i));
    }
  }
}

void Turn::output() {
  cout << _actor << " ";
  if (_hidden_to_revealed) {
    cout << Game::command(0, _Hide_) << " ";
  }
  for (auto x : _act) {
    cout << x << " ";
  }
  if (_revealed_to_hidden) {
    cout << Game::command(0, _Hide_) << " ";
  }
  cout << 0 << endl;
}

bool Turn::is_visible(int samurai) {
  return point_samurai(samurai) != Game::INVISIBLE;
}

void Turn::remove_prohibited_states() {
#if DEBUG == 1
  cerr << "_set_point_samurai : ";
  for (auto x : _set_point_samurai) {
    cerr << x << " ";
  }
  cerr << endl;
#endif
  for (auto i = 0; i < Game::PLAYER; ++i) {
    auto it = _states[i].begin();
    while (it != _states[i].end()) {
      if (it->route_has_key(_set_point_samurai)) {
#if DEBUG == 1
        cerr << "We erase %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
        cerr << *it << endl;
#endif
        it = _states[i].erase(it);
      } else {
        ++it;
      }
    }
  }
}
