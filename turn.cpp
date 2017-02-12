#include <iostream>
#include "header/game.h"

using namespace std;

#define DEBUG 0

Turn::Turn() : _field() {
  _point_samurai = new Point[Game::SAMURAI];
  _has_done = new bool[Game::SAMURAI];
  _is_hidden = new bool[Game::SAMURAI];
  _treat_num = new int[Game::SAMURAI];
  
  _is_killed = new bool[Game::SAMURAI];
  _point_enemy = new vector<Point>[Game::PLAYER];
  _set_point_enemy = new set<Point>[Game::PLAYER];
  _base_prob = new double[Game::PLAYER];
  _is_remained = new bool[Game::PLAYER];

  _kappa = new bool***[_Kappatypesize_];
  for (auto k = 0; k < _Kappatypesize_; ++k) {
    _kappa[k] = new bool**[Game::PLAYER];
    for (auto i = 0; i < Game::PLAYER; ++i) {
      _kappa[k][i] = new bool*[Game::FIELD];
      for (auto j = 0; j < Game::FIELD; ++j) {
        _kappa[k][i][j] = new bool[Game::FIELD];
      }
    }
  }
  _beta = new bool*[Game::FIELD];
  for (auto i = 0; i < Game::FIELD; ++i) {
    _beta[i] = new bool[Game::PLAYER];
  }
  _states = new vector<State>[Game::PLAYER];
  _table_pre_paint_score = new double**[Game::PLAYER];
  for (auto i = 0; i < Game::PLAYER; ++i) {
    _table_pre_paint_score[i] = new double*[Game::FIELD];
    for (auto j = 0; j < Game::FIELD; ++j) {
      _table_pre_paint_score[i][j] = new double[Game::FIELD];
      for (auto k = 0; k < Game::FIELD; ++k) {
        _table_pre_paint_score[i][j][k] = -1.0;
      }
    }
  }
  _initial_death_prob = new double[Game::PLAYER];
}

Turn::~Turn() {
  delete[] _point_samurai;
  delete[] _has_done;
  delete[] _is_hidden;
  delete[] _treat_num;

  delete[] _is_killed;
  delete[] _point_enemy;
  delete[] _set_point_enemy;
  delete[] _base_prob;
  delete[] _is_remained;  

  for (auto i = 0; i < Game::FIELD; ++i) {
    delete[] _beta[i];
  }
  delete[] _beta;
    for (auto k = 0; k < _Kappatypesize_; ++k) {
    for (auto i = 0; i < Game::PLAYER; ++i) {
      for (auto j = 0; j < Game::FIELD; ++j) {
        delete[] _kappa[k][i][j];
      }
      delete[] _kappa[k][i];
    }
    delete[] _kappa[k];
  }
  delete[] _kappa;
  delete[] _states;
  for (auto i = 0; i < Game::PLAYER; ++i) {
    for (auto j = 0; j < Game::FIELD; ++j) {
      delete[] _table_pre_paint_score[i][j];
    }
    delete[] _table_pre_paint_score[i];
  }
  delete[] _table_pre_paint_score;
  delete[] _initial_death_prob;
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
    if (treat_num(i) != 0) {
      _is_killed[i] = true;
    } else {
      _is_killed[i] = false;
    }
  }
  for (auto i = 0; i < Game::PLAYER; ++i) {
    int enemy = Game::player_to_enemy(i);
    if (has_done(enemy)) {
      _done_enemy.insert(i);
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

/*
void Turn::calc_kill_enemy() {
  for (auto i = 0; i < Game::PLAYER; ++i) {
    if (has_done(i)) continue;
    for (auto it = _states[i].begin(); it != _states[i].end(); ++it) {
      it->count_kill_enemy(_set_point_enemy);
    }
  }  
}
*/
