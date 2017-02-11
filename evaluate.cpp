#include "header/game.h"

using namespace std;

const double State::SCORE_PAINT_EMPTY = 100.0;
const double State::SCORE_PAINT_ENEMY = 110.0;
const int Game::REACH[3] = {5, 3, 2};

void Turn::evaluate() {
  remove_prohibited_states();
  for (auto i = 0; i < Game::PLAYER; ++i) {
    if (has_done(i)) continue;
    auto it = _states[i].begin();
    while (it != _states[i].end()) {
      calc_paint_score(*it);
      calc_hidden_booleans(i, *it);
    }
  }
}

void Turn::remove_prohibited_states() {
  for (auto i = 0; i < Game::PLAYER; ++i) {
    if (has_done(i)) continue;
    auto it = _states[i].begin();
    while (it != _states[i].end()) {
      if (it->route_has_key(_set_point_samurai)) {
        it = _states[i].erase(it);
      } else {
        ++it;
      }
    }
  }
}

void Turn::calc_paint_score(State& state) {
  state.paint_score() = 0;
  for (auto x : state.paint()) {
    if (is_occupied_by_enemy(x)) {
      state.paint_score() += State::SCORE_PAINT_ENEMY;      
    } else if (!is_occupied_by_player(x)) {
      state.paint_score() += State::SCORE_PAINT_EMPTY;      
    }
  }
}

void Turn::calc_hidden_booleans(int& player, State& state) {
  // Game::cost の特殊性で僅かに計算量を減らしている。
  // 仮に Game::cost の値を買えるならここも変更する。
  state.need_hidden_to_revealed() = false;
  if (_is_hidden[player]) {
    if (state.does_paint()) {
      state.need_hidden_to_revealed() = true;
    } else {
      for (auto x : state.route()) {
        if (!is_occupied_by_player(x)) {
          state.need_hidden_to_revealed() = true;
          break;
        }
      }
    }
  }
  if (state.need_hidden_to_revealed()) {
    state.need_cost() = state.initial_cost() + Game::cost(0, _Hide_);
  } else {
    state.need_cost() = state.initial_cost();
  }
  if ((!_is_hidden[player] || state.need_hidden_to_revealed())
      && state.need_cost() < Game::MAX_COST) {
    state.can_revealed_to_hidden() = true;
  } else {
    state.can_revealed_to_hidden() = false;
  }
}

void Turn::calc_kill_score(State& state) {
  state.estimated_kill_score() = 0;
  for (auto i = 0; i < Game::PLAYER; ++i) {
    if (_is_killed[Game::player_to_enemy(i)]) continue;
    int kill_space = 0;
    for (auto x : state.route()) {
      if (_set_point_enemy[i].find(x) != _set_point_enemy[i].end()) {
        ++kill_space;
      }
    }
    state.estimated_kill_score()
      += kill_space/(double)(_set_point_enemy[i].size());
    if (kill_space == (int)_set_point_enemy[i].size()) {
      state.killed_enemy().insert(i);
    }
  }
}

void Turn::calc_death_prob(State& state) {
  double death[Game::PLAYER];
  for (auto i = 0; i < Game::PLAYER; ++i) {
    if (is_killed(Game::player_to_enemy(i))
        || state.killed_enemy().find(i) != state.killed_enemy().end()) {
      death[i] = 0.0;
    } else {
      
    }
  }
}
