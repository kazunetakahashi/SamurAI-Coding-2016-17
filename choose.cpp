#define NDEBUG

#include <cassert>
#include <iomanip>
#include <iostream>
#include "header/game.h"

using namespace std;

#define DEBUG 0

void Turn::choose() {
  // 自明な場合は除く
  bool is_there_anyone = false;
  for (auto player = 0; player < Game::PLAYER; ++player) {
    if (!has_done(player) && !is_killed(player)) {
      is_there_anyone = true;
      break;
    }
  }
  if (!is_there_anyone) {
    for (auto player = 0; player < Game::PLAYER; ++player) {
      if (!has_done(player)) {
        trivial_act(player);
        break;
      }
    }
  } else {
    choose_actor_and_act();
  }
}

void Turn::set_act(int player, vector<int> act, bool h_r, bool r_h) {
  _actor = player;
  _act = act;
  _hidden_to_revealed = h_r;
  _revealed_to_hidden = r_h;
}

void Turn::trivial_act(int player) {
  vector<int> V = {};
  set_act(player, V, false, false);
}

void Turn::choose_actor_and_act() {
  calc_initial_death_prob();
#if DEBUG == 1
  cerr << "_initial_death_prob : ";
  for (auto i = 0; i < Game::PLAYER; ++i) {
    cerr << fixed << setprecision(3) << _initial_death_prob[i] << " ";
  }
  cerr << endl;
#endif
  vector<int> players;
  double max_danger = 0.0;
  int danger_player = -1;
  for (auto player = 0; player < Game::PLAYER; ++player) {
    if (!has_done(player) && !is_killed(player)) {
      players.push_back(player);
      if (max_danger >= State::MAX_DEATH_PROB
          && max_danger < _initial_death_prob[player]) {
        max_danger = _initial_death_prob[player];
        danger_player = player;
      }
    }
  }
  if (danger_player != -1) {
    players = { danger_player };
  }
#if DEBUG == 1
  cerr << "players : ";
  for (auto x : players) {
    cerr << x << " ";
  }
  cerr << endl;
#endif
  vector<State>::iterator max_state;
  double max_score = -100000.0;
  for (auto player : players) {
    for (auto it = states(player).begin();
         it != states(player).end(); ++it) {
      if (it->total_score() > max_score) {
        max_score = it->total_score();
        max_state = it;
      }        
    }
  }
#if DEBUG == 1
  max_state->debug(); 
#endif
  set_act(max_state->player(), max_state->act(),
          max_state->need_hidden_to_revealed(),
          max_state->decided_revealed_to_hidden());
}

void Turn::calc_initial_death_prob() {
  double total_death_one_minus[Game::PLAYER];
  double death[Game::PLAYER][Game::PLAYER];
  for (auto i = 0; i < Game::PLAYER; ++i) {
    if (has_done(i) || is_killed(i)) {
      _initial_death_prob[i] = 0.0;
      continue;
    }
    total_death_one_minus[i] = 1.0;
    for (auto j = 0; j < Game::PLAYER; ++j) {
      death[i][j] = 0.0;
      if (treat_num(Game::player_to_enemy(j)) >= Game::PLAYER) {
        // 0.0 のまま
      } else {
        for (auto x : _point_enemy[j]) {
          death[i][j] += death_prob(j, x, point_samurai(i), is_hidden(i));
        }
        death[i][j] *= _base_prob[j];
        if (death[i][j] >= 1.0) death[i][j] = 1.0;
      }
      total_death_one_minus[i] *= 1.0 - death[i][j];
    }
    _initial_death_prob[i] = 1.0 - total_death_one_minus[i];
  }
}
