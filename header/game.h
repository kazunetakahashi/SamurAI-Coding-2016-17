#ifndef GAME_H
#define GAME_H

#include <vector>

#include "point.h"
#include "turn.h"
#include "field.h"
#include "state.h"

class Game {
  bool _is_sente;
  Turn* turns;
  int now_turn;
  
public:
  const static int TURN;
  const static int MY_TURN;
  const static int PLAYER;
  const static int SAMURAI;
  const static int PERIOD;
  const static int FIELD;
  const static int COST[];
  const static int MAX_COST;
  const static int HOUGAKU;
  const static std::vector<Point> PAINT[];
  
  static std::vector<Point>** rotate_paint; // [プレイヤー][方角]
  static std::vector<State>*** initial_state; // [プレイヤー][x][y]

  static void init();
  
  Game() : now_turn(0) {
    turns = new Turn[MY_TURN];
  };

  ~Game() {
    delete[] turns;
  }
  
  bool is_sente() { return _is_sente; }

  void input();
  void play();

  void before_start();
  void each_turn();
  void think();
  
};

#endif /* GAME_H */
