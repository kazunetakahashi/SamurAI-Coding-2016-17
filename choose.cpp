#include <cassert>
#include "header/game.h"

using namespace std;

#define DEBUG 1

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

void Turn::set_act(int player, vector<int>& act, bool h_r, bool r_h) {
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
  vector<State>::iterator max_state;
  double max_score = -100000.0;
  for (auto player = 0; player < Game::PLAYER; ++player) {
    if (!has_done(player) && !is_killed(player)) {
      for (auto it = states(player).begin();
           it != states(player).end(); ++it) {
        if (it->total_score() > max_score) {
          max_score = it->total_score();
          max_state = it;
        }        
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
