#ifndef GAME_H
#define GAME_H

#include <vector>

#include "point.h"
#include "field.h"
#include "state.h"
#include "turn.h"

enum Command {_Paint_, _Move_, _Hide_};

class Game {
  // 
private:
  bool _is_sente;
  Turn* _turns;
  int _now_turn;

public:
  Game() : _now_turn(0) {
    _turns = new Turn[MY_TURN];
  };
  ~Game() {
    delete[] _turns;
  }
  bool is_sente() const { return _is_sente; }
  Turn* turns() const { return _turns; }
  int now_turn() const { return _now_turn; }

  // constant.cpp
public:
  const static int TURN;
  const static int MY_TURN;
  const static int PLAYER;
  const static int SAMURAI;
  const static int PERIOD;
  const static int FIELD;
  const static int COST[];
  const static int MAX_COST;
  const static int DIRECTION;
  const static std::vector<Point> PAINT[];
  const static Point HOUSE[];
  static int command(int dir, Command obj);
  inline static int cost(int dir, Command obj) {
    return COST[command(dir, obj)];
  }
  static int samurai_to_player(int samurai);
  int player_to_samurai(int player, bool is_mikata = true);

  // init.cpp
  static void init();
  static std::vector<Point>** rotate_paint; // [player][dir]
  static std::vector<Point>**** initial_paint; // [player][x][y][dir]
  static std::vector<State>*** initial_state; // [samurai][x][y]

  // game.cpp
  void input();
  void play();
  void before_start();
  void each_turn();

  // think.cpp
  void think();
  
};

#endif /* GAME_H */
