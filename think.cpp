#include <random>

#include "header/game.h"

using namespace std;

void Game::think() {
  Turn& current = _turns[_now_turn];
  for (auto k = 0; k < PLAYER; ++k) {
    current.states(k) = initial_state(player_to_samurai(k),
                                      current.point_samurai(k));
  }
  current.hidden_to_revealed() = false;
  current.revealed_to_hidden() = false;
  current.actor() = _now_turn%3;
  if (current.treat_num(current.actor()) > 0) {
    current.act() = {};
  } else {
    int N = current.states(current.actor()).size();
    uniform_int_distribution<int> random(0, N-1);
    current.act() = current.states(current.actor())[random(MT)].act();
  }
}
