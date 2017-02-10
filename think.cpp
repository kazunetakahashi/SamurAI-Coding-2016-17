#include <random>
#include <cassert>
#include <algorithm>
#include <tuple>
#include <queue>

#include "header/game.h"

using namespace std;

# define DEBUG 1

void Game::think() {
  Game::inform_turn();
  Game::inform_current_and_previous();
  // ここから
  current().hidden_to_revealed() = false;
  current().revealed_to_hidden() = false;
  current().actor() = _now_turn%3;
  if (current().treat_num(current().actor()) > 0) {
    current().act() = {};
  } else {
    int N = current().states(current().actor()).size();
    uniform_int_distribution<int> random(0, N-1);
    State& temp = current().states(current().actor())[random(MT)];
    current().act() = temp.act();
#if DEBUG == 1
    cerr << temp << endl;
#endif
  }
}

void Game::inform_turn() {
  for (auto k = 0; k < PLAYER; ++k) {
    current().states(k) = initial_state(player_to_samurai(k),
                                      current().point_samurai(k));
  }
  current().remove_prohibited_states();
  current().calc_kill_enemy();
}

void Game::inform_current_and_previous() {
  if (now_turn() == 0) {
    Game::calc_acted_enemy_0();
  } else {
    Game::calc_acted_enemy();
  }
  Game::calc_kappa();
  if (now_turn() == 0) {
    Game::calc_point_enemy_0();
  } else {
    Game::calc_point_enemy();
  }
}

int Game::calc_acted_enemy_0() {
  if (_is_sente) return current().acted_enemy() = -1;
  for (auto i = 0; i < PLAYER; ++i) {
    int enemy = player_to_enemy(i);
    if (current().has_done(enemy)) {
      return current().acted_enemy() = enemy;
    }
  }
  return current().acted_enemy() = -1;
}

int Game::calc_acted_enemy() {
  bool is_there_done = false;
  for (auto i = 0; i < PLAYER; ++i) {
    int enemy = player_to_enemy(i);
    if (current().has_done(enemy)) {
      is_there_done = true;
      break;
    }
  }
  if (is_there_done) {
    for (auto i = 0; i < PLAYER; ++i) {
      int enemy = player_to_enemy(i);
      if (!previous().has_done(enemy) && current().has_done(enemy)) {
        return current().acted_enemy() = enemy;
      }
    }
    return current().acted_enemy() = -1;
  } else {
    for (auto i = 0; i < PLAYER; ++i) {
      int enemy = player_to_enemy(i);
      if (!previous().has_done(enemy)) {
        return current().acted_enemy() = enemy;
      }
    }
    return current().acted_enemy() = -1;
  }
}

void Game::calc_kappa() {
  for (auto t = 0; t < Game::PLAYER; ++t) {
    int enemy = player_to_enemy(t);
    if (enemy != current().acted_enemy() && now_turn() >= 1) {
      for (auto i = 0; i < Game::FIELD; ++i) {
        for (auto j = 0; j < Game::FIELD; ++j) {
          current().kappa(1, enemy, i, j) = previous().kappa(1, enemy, i, j);
          current().kappa(2, enemy, i, j) = previous().kappa(1, enemy, i, j);
        }
      }      
    } else {
      for (auto i = 0; i < Game::FIELD; ++i) {
        for (auto j = 0; j < Game::FIELD; ++j) {
          current().kappa(1, enemy, i, j) = true;
          current().kappa(2, enemy, i, j) = false;
        }
      }
    }
    for (auto i = 0; i < Game::FIELD; ++i) {
      for (auto j = 0; j < Game::FIELD; ++j) {
        if (current().is_perceived(i, j)) {
          current().kappa(1, enemy, i, j) = true;
          if (!current().is_occupied_by_enemy(i, j)) {
            current().kappa(2, enemy, i, j) = false;
          }
        }
        if (current().kappa(1, enemy, i, j)
            && current().kappa(2, enemy, i, j)) {
          current().kappa(0, enemy, i, j) = true;
        } else {
          current().kappa(0, enemy, i, j) = false;
        }
      }
    }
  }  
}

void Game::calc_point_enemy_0() {
  for (auto t = 0; t < Game::PLAYER; ++t) {
    int enemy = player_to_enemy(t);
    if (!current().is_hidden(enemy)) {
      current().point_enemy(t) = { current().point_samurai(enemy) };
      current().is_remained(t) = true;
    } else {
      if (enemy == current().acted_enemy()) {
        vector<Point> V = { Game::HOUSE[player_to_samurai(t, false)] };
        calc_point_enemy_by_dijkstra(t, V); 
        current().is_remained(t) = false;
      } else {
        current().point_enemy(t)
          = { Game::HOUSE[player_to_samurai(t, false)] };
        current().is_remained(t) = true;
      }
    }
  }
}

void Game::calc_point_enemy() {
  for (auto t = 0; t < Game::PLAYER; ++t) {
    int enemy = player_to_enemy(t);
    if (!current().is_hidden(enemy)) {
      current().point_enemy(t) = { current().point_samurai(enemy) };
      current().is_remained(t) = true;
    } else {
      if (enemy == current().acted_enemy()) {
        calc_point_enemy_acted(t);
        current().is_remained(t) = false;
      } else {
        if (previous().is_remained(t)) {
          vector<Point> V = previous().point_enemy(t);
          current().point_enemy(t) = V;
          current().is_remained(t) = true;
        } else {
          for (auto x : previous().point_enemy(t)) {
            if (!current().lambda(x)) {
              current().point_enemy(t).push_back(x);
            }
            current().is_remained(t) = false;
          }
        }
      }
    }
  }
}

void Game::calc_point_enemy_acted(int player) {
  // int enemy = player_to_enemy(player);
  // gamma の計算
  for (auto i = 0; i < Game::FIELD; ++i) {
    for (auto j = 0; j < Game::FIELD; ++j) {
      if (current().is_enemy_color(player, i, j)
          && previous().is_perceived(i, j)
          && !previous().is_enemy_color(player, i, j)) {
        current().gamma().push_back(Point(i, j));
      }
    }
  }
  if (current().gamma().empty()) {
    calc_point_enemy_by_dijkstra(player, previous().point_enemy(player));
  } else {
    calc_point_enemy_by_sat(player);
  }
}

typedef std::tuple<int, int, Point> Pass; // どうせここでしか使わない
// cost, 顕現の状態, point
// 顕現の状態 0:隠伏、1:顕現、2:どちらでもいい

void Game::calc_point_enemy_by_dijkstra(int player, vector<Point>& V) {
  // int enemy = player_to_enemy(player);
  int samurai = player_to_samurai(player, false);
  priority_queue<Pass, vector<Pass>, greater<Pass> > Q;
  for (auto x : V) {
    if (current().kappa(0, player, x)) {
      Q.push(make_tuple(0, 0, x));
    } else {
      Q.push(make_tuple(0, 2, x));
    }
  }
  bool visited[Game::FIELD][Game::FIELD];
  fill(&visited[0][0], &visited[0][0]+(Game::FIELD*Game::FIELD), false);
  vector<Point> ans;
  while (!Q.empty()) {
    int cost = get<0>(Q.top());
    int kengen = get<1>(Q.top());
    Point now = get<2>(Q.top());
    Q.pop();
    if (!visited[now.x()][now.y()]) {
      visited[now.x()][now.y()] = true;
      // ゴールか
      if (!current().is_perceived(now)) {
        if (cost <= Game::MAX_COST) {
          ans.push_back(now);
        }        
      } else if (!current().lambda(now)) {
        if (kengen == 1) {
          if (cost + Game::cost(0, _Hide_) <= Game::MAX_COST) {
            ans.push_back(now);
          }
        } else {
          if (cost <= Game::MAX_COST) {
            ans.push_back(now);
          }
        }
      }
      // 次へ
      for (auto i = 0; i < Game::DIRECTION; ++i) {
        int newcost = cost + Game::cost(0, _Move_);
        int newkengen;
        Point next = now + dx[i];
        if (!next.is_valid() || !next.is_movable(samurai)) continue;
        if (!current().is_perceived(next)) {
          newkengen = kengen;
        } else if (!current().lambda(now)) {
          newkengen = kengen;
        } else {
          newkengen = 1;
        }
        if (kengen != 2 && kengen != newkengen) {
          newcost += Game::cost(0, _Hide_);
        }
        if (!visited[next.x()][next.y()] && newcost <= Game::MAX_COST) {
          Q.push(make_tuple(newcost, newkengen, next));
        }
      }
    }
  }
  current().point_enemy(player) = ans;
}

void Game::calc_point_enemy_by_sat(int player) {
  // int enemy = player_to_enemy(player);
  int samurai = player_to_samurai(player, false);
  // beta, delta 作成 (よく考えたら delta と beta は同じでした)
  for (auto i = 0; i < Game::FIELD; ++i) {
    for (auto j = 0; j < Game::FIELD; ++j) {
      if (current().is_perceived(i, j)
          && !current().is_enemy_color(player, i, j)) {
        current().beta(i, j) = true;
        current().delta().push_back(Point(i, j));
      } else {
        current().beta(i, j) = false;
      }
    }
  }
  bool s_kouho[Game::FIELD][Game::FIELD];
  fill(&s_kouho[0][0], &s_kouho[0][0]+(Game::FIELD*Game::FIELD), 0);
  for (auto x : previous().point_enemy(player)) {
    s_kouho[x.x()][x.y()] = true;
  }
  bool g_kouho[Game::FIELD][Game::FIELD];
  fill(&g_kouho[0][0], &g_kouho[0][0]+(Game::FIELD*Game::FIELD), 0);
  for (auto i = 0; i < Game::FIELD; ++i) {
    for (auto j = 0; j < Game::FIELD; ++j) {
      if (s_kouho[i][j]) {
        g_kouho[i][j] = true;
        Point p = Point(i, j);
        for (auto k = 0; k < Game::DIRECTION; ++k) {
          Point q = p + dx[k];
          if (q.is_valid() && q.is_movable(samurai) && !current().lambda(q)) {
            g_kouho[q.x()][q.y()] = true;
          }
        }
      }
    }
  }
  vector<Point> d_kouho;
  for (auto i = 0; i < Game::FIELD; ++i) {
    for (auto j = 0; j < Game::FIELD; ++j) {
      if (g_kouho[i][j]) {
        for (auto k = 0; k < Game::DIRECTION; ++k) {
          vector<Point>& V = _initial_paint[player][i][j][k];
          set<Point>& S = _set_initial_paint[player][i][j][k];
          bool ok = true;
          for (auto x : V) {
            if (current().beta(x)) {
              ok = false;
              break;
            }
          }
          if (!ok) continue;
          for (auto x : current().gamma()) {
            if (S.find(x) == S.end()) {
              ok = false;
              break;
            }
          }
          if (!ok) continue;
          for (auto x : current().delta()) {
            if (S.find(x) != S.end()) {
              ok = false;
              break;
            }
          }
          if (ok) {
            d_kouho.push_back(Point(i, j));
            break;
          }
        }
      }
    }
  }
  // コスト 8 を弾く
  if (previous().is_remained(player)) {
    current().point_enemy(player) = d_kouho;
  } else {
    for (auto p : d_kouho) {
      if (s_kouho[p.x()][p.y()] || !current().kappa(0, player, p)) {
        current().point_enemy(player).push_back(p);
      } else {
        for (auto k = 0; k < Game::DIRECTION; ++k) {
          Point q = p + dx[k];
          if (q.is_valid() && s_kouho[q.x()][q.y()]
              && !previous().kappa(0, player, q)) {
            current().point_enemy(player).push_back(p);
            break;
          }
        }
      }
    }
  }
}
