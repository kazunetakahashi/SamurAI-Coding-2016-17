#include "header/game.h"

using namespace std;

const double State::SCORE_PAINT_EMPTY = 100.0;
const double State::SCORE_PAINT_ENEMY = 110.0;
const double State::SCORE_PRE_PAINT_UNKNOWN = 50.0;
const double State::SCORE_PRE_PAINT_EMPTY = 70.0;
const double State::SCORE_PRE_PAINT_ENEMY = 80.0;

#define DEBUG 1

void Turn::evaluate() {
  remove_prohibited_states();
  for (auto i = 0; i < Game::PLAYER; ++i) {
    if (has_done(i) || is_killed(i)) continue;
    auto it = _states[i].begin();
    while (it != _states[i].end()) {
      calc_paint_score(*it); // pre_paint_score も計算する
      calc_hidden_booleans(*it);
      calc_kill_score(*it);
      calc_death_prob(*it);
      calc_eye_prob(*it);
      calc_place_score(*it);
      calc_total_score(*it);
      it++;
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
  state.pre_paint_score() = calc_pre_paint_score(state);
}

double Turn::calc_pre_paint_score(State& state) {
  const int& player = state.player();
  const Point& goal = state.goal();
  if (table_pre_paint_score(player, goal) >= 0) {
    return table_pre_paint_score(player, goal);
  }
  table_pre_paint_score(player, goal) = 0.0;
  for (auto k = 0; k < Game::DIRECTION; ++k) {
    double temp = 0.0;
    for (auto x : Game::_initial_paint[player][goal.x()][goal.y()][k]) {
      if (is_occupied_by_enemy(x)) {
        if (state.set_paint().find(x) == state.set_paint().end()) {
          temp += State::SCORE_PRE_PAINT_ENEMY;
        }
      } else if (is_perceived(x)) {
        if (!is_occupied_by_player(x)) {
          if (state.set_paint().find(x) == state.set_paint().end()) {
            temp += State::SCORE_PRE_PAINT_EMPTY;
          }
        }
      } else {
        if (state.set_paint().find(x) == state.set_paint().end()) {
          temp += State::SCORE_PRE_PAINT_UNKNOWN;
        }
      }
    }
    table_pre_paint_score(player, goal)
      = max(table_pre_paint_score(player, goal), temp);
  }
  return table_pre_paint_score(player, goal);
}

void Turn::calc_hidden_booleans(State& state) {
  // Game::cost の特殊性で僅かに計算量を減らしている。
  // 仮に Game::cost の値を買えるならここも変更する。
  state.need_hidden_to_revealed() = false;
  if (_is_hidden[state.player()]) {
    state.normal_hidden() = true;
    if (state.does_paint()) {
      state.need_hidden_to_revealed() = true;
      state.normal_hidden() = false;
    } else {
      for (auto x : state.route()) {
        if (!is_occupied_by_player(x)) {
          state.need_hidden_to_revealed() = true;
          state.normal_hidden() = false;
          break;
        }
      }
    }
  } else {
    state.normal_hidden() = false;
  }
  if (state.need_hidden_to_revealed()) {
    state.need_cost() = state.initial_cost() + Game::cost(0, _Hide_);
  } else {
    state.need_cost() = state.initial_cost();
  }
  if ((!_is_hidden[state.player()] || state.need_hidden_to_revealed())
      && state.need_cost() < Game::MAX_COST) {
    state.can_revealed_to_hidden() = true;
  } else {
    state.can_revealed_to_hidden() = false;
  }
}

void Turn::calc_kill_score(State& state) {
  state.estimated_kill_score() = 0.0;
  for (auto i = 0; i < Game::PLAYER; ++i) {
    if (_is_killed[Game::player_to_enemy(i)]) continue;
    int kill_space = 0;
    for (auto x : state.paint()) {
      if (_set_point_enemy[i].find(x) != _set_point_enemy[i].end()) {
        ++kill_space;
      }
    }
    state.estimated_kill_score() += kill_space * _base_prob[i];
    if (kill_space == (int)_set_point_enemy[i].size()) {
      state.killed_enemy().insert(i);
    }
  }
}

void Turn::calc_death_prob(State& state) {
  double death[2][Game::PLAYER];
  double total_death_one_minus[2] = { 1.0, 1.0 };
  for (auto i = 0; i < 2; ++i) {
    for (auto player = 0; player < Game::PLAYER; ++player) {
      death[i][player] = 0.0;
      if (is_killed(Game::player_to_enemy(player))
          || state.killed_enemy().find(player)
          != state.killed_enemy().end()) {
        // 0.0 のまま
      } else {
        for (auto x : _point_enemy[player]) {
          if (state.set_paint().find(x) == state.set_paint().end()) {
            death[i][player]
              += death_prob(player, x, state.goal(), i);
          }
        }
        death[i][player] *= _base_prob[player];
        if (death[i][player] >= 1.0) death[i][player] = 1.0;
      }
      total_death_one_minus[i] *= 1.0 - death[i][player];
    }
  }
  state.total_death_prob_revealed() = 1.0 - total_death_one_minus[0];
  state.total_death_prob_hidden() = 1.0 - total_death_one_minus[1];
}

void Turn::calc_eye_prob(State& state) {
  double eye[Game::PLAYER];
  double total_eye_one_minus = 1.0;
  for (auto player = 0; player < Game::PLAYER; ++player) {
    eye[player] = 0.0;
    if (is_killed(Game::player_to_enemy(player))
        || state.killed_enemy().find(player)
        != state.killed_enemy().end()
        || (int)_point_enemy[player].size() > State::MAX_PLACE_ENEMY) {
      // 0.0 のまま
    } else {
      for (auto x : _point_enemy[player]) {
        if (state.set_paint().find(x) == state.set_paint().end()) {
          eye[player] += eye_prob(x, state.goal());
        }
      }
      eye[player] *= _base_prob[player];
      if (eye[player] >= 1.0) eye[player] = 1.0;
    }
    total_eye_one_minus *= 1.0 - eye[player];
  }
  state.total_eye_prob() = 1.0 - total_eye_one_minus;
}

void Turn::calc_place_score(State& state) {
  state.final_place_score()
    = 2.0 * ((double)Game::FIELD)
    - target_dist(state.samurai(), state.goal());
}

const double State::MAX_DEATH_PROB = 0.2;
const double State::MAX_EYE_PROB = 0.3;
const double State::RATE_ESTIMATED_KILL = 70.0;
const double State::RATE_CONFIRMED_KILL = 180.0;
const double State::RATE_DEATH = 200.0; // 負で作用する
const double State::RATE_PLACE = 25.0;

void Turn::calc_total_score(State& state) {
  if (state.normal_hidden()) {
    state.decided_revealed_to_hidden() = false;
    state.final_hidden() = true;
  } else {
    if (state.can_revealed_to_hidden()) {
      if (state.total_eye_prob() <= State::MAX_EYE_PROB
          && state.total_death_prob_revealed() <= State::MAX_DEATH_PROB) {
        state.decided_revealed_to_hidden() = false;
        state.final_hidden() = false;
      } else {
        state.decided_revealed_to_hidden() = true;
        state.final_hidden() = true;
      }
    } else {
      state.decided_revealed_to_hidden() = false;
      state.final_hidden() = false;
    }
  }
  if (state.final_hidden()) {
    state.final_death_prob() = state.total_death_prob_hidden();
  } else {
    state.final_death_prob() = state.total_death_prob_revealed();
  }
  if (state.final_death_prob() > State::MAX_DEATH_PROB
      || (!state.final_hidden()
          && state.total_eye_prob() > State::MAX_EYE_PROB)) {
    state.total_score() = -1 * state.final_death_prob();
  } else {
    state.total_score()
      = state.paint_score() + state.pre_paint_score()
      + state.estimated_kill_score() * State::RATE_ESTIMATED_KILL
      + state.confirmed_kill_score() * State::RATE_CONFIRMED_KILL
      - state.final_death_prob() * State::RATE_DEATH
      + state.final_place_score() * State::RATE_PLACE;
  }
}
