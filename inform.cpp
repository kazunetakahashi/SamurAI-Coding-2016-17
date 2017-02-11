#include <algorithm>
#include <tuple>
#include <queue>

#include "header/game.h"

using namespace std;

#define DEBUG 1

void Game::inform() {
  for (auto k = 0; k < PLAYER; ++k) {
    current().states(k)
      = initial_state(player_to_samurai(k), current().point_samurai(k));
  }
  if (now_turn() == 0) {
    calc_acted_enemy_0();
  } else {
    calc_acted_enemy();
  }
  calc_kappa();
  if (now_turn() == 0) {
    calc_point_enemy_0();
  } else {
    calc_point_enemy();
  }
  // point_enemy -> set_point_enemy
  for (auto i = 0; i < PLAYER; ++i) {
    for (auto x : current().point_enemy(i)) {
      current().set_point_enemy(i).insert(x);
    }
  }
}

int Game::calc_acted_enemy_0() {
  if (_is_sente) return current().acted_enemy() = -1;
  for (auto i = 0; i < PLAYER; ++i) {
    int enemy = player_to_enemy(i);
    if (current().has_done(enemy)) {
      return current().acted_enemy() = i;
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
        return current().acted_enemy() = i;
      }
    }
    return current().acted_enemy() = -1;
  } else {
    for (auto i = 0; i < PLAYER; ++i) {
      int enemy = player_to_enemy(i);
      if (!previous().has_done(enemy)) {
        return current().acted_enemy() = i;
      }
    }
    return current().acted_enemy() = -1;
  }
}

void Game::calc_kappa() {
  for (auto player = 0; player < Game::PLAYER; ++player) {
    int samurai = player_to_samurai(player, false);
    if (player != current().acted_enemy() && now_turn() >= 1) {
      for (auto i = 0; i < Game::FIELD; ++i) {
        for (auto j = 0; j < Game::FIELD; ++j) {
          current().kappa(_Watched_, player, i, j)
            = previous().kappa(_Watched_, player, i, j);
          current().kappa(_Kept_, player, i, j)
            = previous().kappa(_Kept_, player, i, j);
        }
      }      
    } else {
      for (auto i = 0; i < Game::FIELD; ++i) {
        for (auto j = 0; j < Game::FIELD; ++j) {
          current().kappa(_Watched_, player, i, j) = false;
          current().kappa(_Kept_, player, i, j) = true;
        }
      }
    }
    for (auto i = 0; i < Game::FIELD; ++i) {
      for (auto j = 0; j < Game::FIELD; ++j) {
        if (current().is_perceived(i, j)) {
          current().kappa(_Watched_, player, i, j) = true;
          if (!current().is_occupied_by_enemy(i, j)) {
            current().kappa(_Kept_, player, i, j) = false;
          }
        }
        if (current().kappa(_Watched_, player, i, j)
            && current().kappa(_Kept_, player, i, j)) {
          current().kappa(_Painted_, player, i, j) = true;
        } else {
          current().kappa(_Painted_, player, i, j) = false;
        }
        if ((!current().kappa(_Watched_, player, i, j)
             || current().kappa(_Painted_, player, i, j))
            && (Point(i, j).is_movable(samurai))) {
          current().kappa(_Lying_, player, i, j) = true;
        } else {
          current().kappa(_Lying_, player, i, j) = false;
        }
      }
    }
  }  
}

void Game::calc_point_enemy_0() {
  for (auto player = 0; player < Game::PLAYER; ++player) {
    int enemy = player_to_enemy(player);
    if (!current().is_hidden(enemy)) {
      current().point_enemy(player) = { current().point_samurai(enemy) };
      current().is_remained(player) = true;
    } else {
      if (player == current().acted_enemy()) {
        Pqueue Q;
        Q.push(Pass(0, _Kengen_,
                    Game::HOUSE[player_to_samurai(player, false)]));
        calc_point_enemy_by_dijkstra(player, Q);
        current().is_remained(player) = false;
      } else {
        current().point_enemy(player)
          = { Game::HOUSE[player_to_samurai(player, false)] };
        current().is_remained(player) = true;
      }
    }
  }
}

void Game::calc_point_enemy() {
  for (auto player = 0; player < Game::PLAYER; ++player) {
    int enemy = player_to_enemy(player);
    if (!current().is_hidden(enemy)) {
      current().point_enemy(player) = { current().point_samurai(enemy) };
      current().is_remained(player) = true;
    } else {
      if (player == current().acted_enemy()) {
        calc_point_enemy_acted(player);
        current().is_remained(player) = false;
      } else {
        if (previous().is_remained(player)) {
          vector<Point> V = previous().point_enemy(player); // コピーで渡す
          current().point_enemy(player) = V;
          current().is_remained(player) = true;
        } else {
          for (auto x : previous().point_enemy(player)) {
            if (current().kappa(_Lying_, player, x)) {
              current().point_enemy(player).push_back(x);
            }
            current().is_remained(player) = false;
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
    Pqueue Q;
    for (auto x : previous().point_enemy(player)) {
      if (previous().kappa(_Lying_, player, x)) {
        if (previous().kappa(_Painted_, player, x)) {
          Q.push(Pass(0, _Inpuku_, x));
        } else {
          Q.push(Pass(0, _Dochirademo_, x));
        }
      }
    }
    calc_point_enemy_by_dijkstra(player, Q);
  } else {
    calc_point_enemy_by_sat(player);
  }
}

void Game::calc_point_enemy_by_dijkstra(int player, Pqueue& Q) {
  // int enemy = player_to_enemy(player);
  int samurai = player_to_samurai(player, false);
  bool visited[Game::FIELD][Game::FIELD];
  fill(&visited[0][0], &visited[0][0]+(Game::FIELD*Game::FIELD), false);
  vector<Point> ans;
  while (!Q.empty()) {
    int cost = get<0>(Q.top());
    Passtype type = get<1>(Q.top());
    Point now = get<2>(Q.top());
    Q.pop();
    if (!visited[now.x()][now.y()]) {
      visited[now.x()][now.y()] = true;
      // ゴールか
      if (current().kappa(_Lying_, player, now)) {
        if (current().kappa(_Painted_, player, now)) {
          if (type == _Kengen_) {
            if (cost + Game::cost(0, _Hide_) <= Game::MAX_COST) {
              ans.push_back(now);
            }
          } else {
            if (cost <= Game::MAX_COST) {
              ans.push_back(now);
            }
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
        Passtype newtype;
        Point next = now + dx[i];
        if (!next.is_valid() || !next.is_movable(samurai)) continue;
        if (current().kappa(_Lying_, player, next)) {
          if (current().kappa(_Painted_, player, next)) {
            newtype = type;
          } else {
            newtype = type;
          }
        } else {
          newtype = _Kengen_;
          if (type == _Inpuku_) {
            newcost += Game::cost(0, _Hide_);
          }
        }
        if (!visited[next.x()][next.y()] && newcost <= Game::MAX_COST) {
          Q.push(make_tuple(newcost, newtype, next));
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
          && !current().is_enemy_color(player, i, j)
          && Point(i, j).is_paintable()) {
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
          if (q.is_valid() && current().kappa(_Lying_, player, q)) {
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
            if (x.is_paintable() && current().beta(x)) {
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
      if (s_kouho[p.x()][p.y()]) {
        current().point_enemy(player).push_back(p);
      } else {
        for (auto k = 0; k < Game::DIRECTION; ++k) {
          Point q = p + dx[k];
          if (q.is_valid() && s_kouho[q.x()][q.y()]
              && !previous().kappa(_Painted_, player, q)) {
            current().point_enemy(player).push_back(p);
            break;
          }
        }
      }
    }
  }
}
