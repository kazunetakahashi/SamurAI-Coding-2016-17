#include <iostream>
#include <queue>

#include "header/game.h"

using namespace std;

vector<Point>** Game::rotate_paint; // [player][dir]
vector<Point>**** Game::initial_paint; // [player][x][y][dir]
vector<State>*** Game::initial_state; // [samurai][x][y]

void Game::init() {
  Point dx[4] = {
    Point(0, 1),
    Point(1, 0),
    Point(0, -1),
    Point(-1, 0),
  };
  // Game::rotate_paint の準備
  Game::rotate_paint = new vector<Point>*[Game::PLAYER];
  for (auto i = 0; i < Game::PLAYER; ++i) {
    Game::rotate_paint[i] = new vector<Point>[Game::DIRECTION];
    for (auto j = 0; j < Game::DIRECTION; ++j) {
#if DEBUG == 1
      cerr << "Game::rotate_paint[" << i << "][" << j << "]" << endl;
#endif
      for (auto x : Game::PAINT[i]) {
        Game::rotate_paint[i][j].push_back(x.rotate(j));
#if DEBUG == 1
        cerr << x.rotate(j) << " <- " << x << endl;
#endif
      }
    }
  }
  // Game::initial_paint の準備
  Game::initial_paint = new vector<Point>***[Game::PLAYER];
  for (auto k = 0; k < Game::PLAYER; ++k) {
    Game::initial_paint[k] = new vector<Point>**[Game::FIELD];
    for (auto i = 0; i < Game::FIELD; ++i) {
      Game::initial_paint[k][i] = new vector<Point>*[Game::FIELD];
      for (auto j = 0; j < Game::FIELD; ++j) {
        Game::initial_paint[k][i][j] = new vector<Point>[Game::DIRECTION];
        Point now(i, j);
        for (auto l = 0; l < Game::DIRECTION; ++l) {
          vector<Point> temp;
          for (auto x : Game::rotate_paint[k][l]) {
            x += now;
            if (x.is_valid() && x.is_paintable()) {
              temp.push_back(x);
            }
          }
          Game::initial_paint[k][i][j][l] = temp;
          cerr << "initial_paint[" << k << "]["
               << i << "][" << j << "][" << l << "] = "; 
#if DEBUG == 1
          for (auto x : temp) {
            cerr << x << " ";
          }
          cerr << endl;
#endif
        }
      }
    }
  }
  // Game::initial_state の準備
  Game::initial_state = new vector<State>**[Game::SAMURAI];
  for (auto k = 0; k < Game::SAMURAI; ++k) {
    int player = Game::samurai_to_player(k);
    Game::initial_state[k] = new vector<State>*[Game::FIELD];
    for (auto i = 0; i < Game::FIELD; ++i) {
      Game::initial_state[k][i] = new vector<State>[Game::FIELD];
      for (auto j = 0; j < Game::FIELD; ++j) {
        priority_queue<State, vector<State>, greater<State> > Q;
        State nothing(Point(i, j), 0);
        Q.push(nothing);
        while (!Q.empty()) {
          State now = Q.top();
          Q.pop();
          initial_state[k][i][j].push_back(now);
          if (now.paint().empty()) {
            for (auto l = 0; l < Game::DIRECTION; ++l) { // 塗り
              State next = now;
              next.initial_cost() += Game::cost(l, _Paint_);
              if (next.initial_cost() > Game::MAX_COST) continue;
              // コストが全部同じなので、上で判定したほうが枝刈りは効率いい。
              next.paint() = Game::initial_paint[player][i][j][l];
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
        cerr << "Game::initial_state[" << k << "][" << i << "]["
             << j << "]" << endl;
        for (auto x : Game::initial_state[k][i][j]) {
          cerr << x << endl;
        }
      }
    }
  }
#endif
}
