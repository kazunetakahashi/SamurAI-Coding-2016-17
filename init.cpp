#include <iostream>
#include <queue>

#include "header/game.h"

using namespace std;

vector<Point>** Game::_rotate_paint; // [player][dir]
vector<Point>**** Game::_initial_paint; // [player][x][y][dir]
vector<State>*** Game::_initial_state; // [samurai][x][y]

random_device Game::RD;
mt19937 Game::MT;

#define DEBUG 0 // change 0 -> 1 if we need debug.

void Game::init() {
  Point dx[4] = {
    Point(0, 1),
    Point(1, 0),
    Point(0, -1),
    Point(-1, 0),
  };
  // Game::_rotate_paint の準備
  Game::_rotate_paint = new vector<Point>*[Game::PLAYER];
  for (auto i = 0; i < Game::PLAYER; ++i) {
    Game::_rotate_paint[i] = new vector<Point>[Game::DIRECTION];
    for (auto j = 0; j < Game::DIRECTION; ++j) {
#if DEBUG == 1
      cerr << "Game::_rotate_paint[" << i << "][" << j << "]" << endl;
#endif
      for (auto x : Game::PAINT[i]) {
        Game::_rotate_paint[i][j].push_back(x.rotate(j));
#if DEBUG == 1
        cerr << x.rotate(j) << " <- " << x << endl;
#endif
      }
    }
  }
  // Game::_initial_paint の準備
  Game::_initial_paint = new vector<Point>***[Game::PLAYER];
  for (auto k = 0; k < Game::PLAYER; ++k) {
    Game::_initial_paint[k] = new vector<Point>**[Game::FIELD];
    for (auto i = 0; i < Game::FIELD; ++i) {
      Game::_initial_paint[k][i] = new vector<Point>*[Game::FIELD];
      for (auto j = 0; j < Game::FIELD; ++j) {
        Game::_initial_paint[k][i][j] = new vector<Point>[Game::DIRECTION];
        Point now(i, j);
        for (auto l = 0; l < Game::DIRECTION; ++l) {
          vector<Point> temp;
          for (auto x : Game::_rotate_paint[k][l]) {
            x += now;
            if (x.is_valid() && x.is_paintable()) {
              temp.push_back(x);
            }
          }
          Game::_initial_paint[k][i][j][l] = temp;
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
  // Game::_initial_state の準備
  Game::_initial_state = new vector<State>**[Game::SAMURAI];
  for (auto k = 0; k < Game::SAMURAI; ++k) {
    int player = Game::samurai_to_player(k);
    Game::_initial_state[k] = new vector<State>*[Game::FIELD];
    for (auto i = 0; i < Game::FIELD; ++i) {
      Game::_initial_state[k][i] = new vector<State>[Game::FIELD];
      for (auto j = 0; j < Game::FIELD; ++j) {
        priority_queue<State, vector<State>, greater<State> > Q;
        State nothing(Point(i, j), 0);
        Q.push(nothing);
        while (!Q.empty()) {
          State now = Q.top();
          Q.pop();
          _initial_state[k][i][j].push_back(now);
          if (now.paint().empty()) {
            for (auto l = 0; l < Game::DIRECTION; ++l) { // 塗り
              State next = now;
              next.initial_cost() += Game::cost(l, _Paint_);
              if (next.initial_cost() > Game::MAX_COST) continue;
              // コストが全部同じなので、上で判定したほうが枝刈りは効率いい。
              next.paint() = Game::_initial_paint[player][i][j][l];
              next.act().push_back(Game::command(l, _Paint_));
              Q.push(next);
            }
          }
          for (auto l = 0; l < Game::DIRECTION; ++l) {
            Point dst = now.goal() + dx[l];
            if (dst.is_valid() && dst.is_movable(k)
                && dst.dist(now.start()) == (int)now.route().size()) {
              State next = now;
              next.initial_cost() += Game::cost(l, _Move_);
              if (next.initial_cost() > Game::MAX_COST) continue;            
              next.route().push_back(dst);
              next.act().push_back(Game::command(l, _Move_));
              Q.push(next);
            }
          }
        }
      }
    }
  }
#if DEBUG == 1
  for (auto k = 0; k < Game::SAMURAI; ++k) {
    for (auto i = 0; i < Game::FIELD; ++i) {
      for (auto j = 0; j < Game::FIELD; ++j) {
        cerr << "Game::_initial_state[" << k << "][" << i << "]["
             << j << "]" << endl;
        for (auto x : Game::_initial_state[k][i][j]) {
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
