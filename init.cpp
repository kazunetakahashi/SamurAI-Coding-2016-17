#include <iostream>
#include <queue>

#include "header/game.h"

using namespace std;

vector<Point>** Game::_rotate_paint; // [player][dir]
vector<Point>**** Game::_initial_paint; // [player][x][y][dir]
set<Point>**** Game::_set_initial_paint; // [player][x][y][dir]
vector<State>*** Game::_initial_state; // [samurai][x][y]

random_device Game::RD;
mt19937 Game::MT;

#define DEBUG 0 // change 0 -> 1 if we need debug.

void Game::init() {
  // _rotate_paint の準備
  _rotate_paint = new vector<Point>*[PLAYER];
  for (auto i = 0; i < PLAYER; ++i) {
    _rotate_paint[i] = new vector<Point>[DIRECTION];
    for (auto j = 0; j < DIRECTION; ++j) {
#if DEBUG == 1
      cerr << "_rotate_paint[" << i << "][" << j << "]" << endl;
#endif
      for (auto x : PAINT[i]) {
        _rotate_paint[i][j].push_back(x.rotate(j));
#if DEBUG == 1
        cerr << x.rotate(j) << " <- " << x << endl;
#endif
      }
    }
  }
  // _initial_paint の準備
  _initial_paint = new vector<Point>***[PLAYER];
  _set_initial_paint = new set<Point>***[PLAYER];
  for (auto k = 0; k < PLAYER; ++k) {
    _initial_paint[k] = new vector<Point>**[FIELD];
    _set_initial_paint[k] = new set<Point>**[FIELD];
    for (auto i = 0; i < FIELD; ++i) {
      _initial_paint[k][i] = new vector<Point>*[FIELD];
      _set_initial_paint[k][i] = new set<Point>*[FIELD];
      for (auto j = 0; j < FIELD; ++j) {
        _initial_paint[k][i][j] = new vector<Point>[DIRECTION];
        _set_initial_paint[k][i][j] = new set<Point>[DIRECTION];
        Point now(i, j);
        for (auto l = 0; l < DIRECTION; ++l) {
          vector<Point> temp;
          set<Point> S;
          for (auto x : _rotate_paint[k][l]) {
            x += now;
            if (x.is_valid() && x.is_paintable()) {
              temp.push_back(x);
              S.insert(x);
            }
          }
          _initial_paint[k][i][j][l] = temp;
          _set_initial_paint[k][i][j][l] = S;
#if DEBUG == 1
          cerr << "_initial_paint[" << k << "]["
               << i << "][" << j << "][" << l << "] = "; 
          for (auto x : temp) {
            cerr << x << " ";
          }
          cerr << endl;
#endif
        }
      }
    }
  }
  // _initial_state の準備
  _initial_state = new vector<State>**[SAMURAI];
  for (auto k = 0; k < SAMURAI; ++k) {
    int player = samurai_to_player(k);
    _initial_state[k] = new vector<State>*[FIELD];
    for (auto i = 0; i < FIELD; ++i) {
      _initial_state[k][i] = new vector<State>[FIELD];
      for (auto j = 0; j < FIELD; ++j) {
        priority_queue<State, vector<State>, greater<State> > Q;
        State nothing(player, Point(i, j), 0);
        Q.push(nothing);
        while (!Q.empty()) {
          State now = Q.top();
          Q.pop();
          _initial_state[k][i][j].push_back(now);
          if (now.paint().empty()) {
            for (auto l = 0; l < DIRECTION; ++l) { // 塗り
              State next = now;
              next.initial_cost() += cost(l, _Paint_);
              if (next.initial_cost() > MAX_COST) continue;
              // コストが全部同じなので、上で判定したほうが枝刈りは効率いい。
              next.paint()
                = _initial_paint
                [player][now.goal().x()][now.goal().y()][l];
              next.set_paint()
                = _set_initial_paint
                [player][now.goal().x()][now.goal().y()][l];
              next.act().push_back(command(l, _Paint_));
              Q.push(next);
            }
          }
          for (auto l = 0; l < DIRECTION; ++l) {
            Point dst = now.goal() + dx[l];
            if (dst.is_valid() && dst.is_movable(k)
                && dst.dist(now.start()) == (int)now.route().size()) {
              State next = now;
              next.initial_cost() += cost(l, _Move_);
              if (next.initial_cost() > MAX_COST) continue;            
              next.route().push_back(dst);
              next.act().push_back(command(l, _Move_));
              Q.push(next);
            }
          }
        }
      }
    }
  }
#if DEBUG == 1
  for (auto k = 0; k < SAMURAI; ++k) {
    for (auto i = 0; i < FIELD; ++i) {
      for (auto j = 0; j < FIELD; ++j) {
        cerr << "_initial_state[" << k << "][" << i << "]["
             << j << "]" << endl;
        for (auto x : _initial_state[k][i][j]) {
          cerr << x << endl;
        }
      }
    }
  }
#endif
  // 乱数のシード
  RD();
  MT = mt19937(RD());
}

