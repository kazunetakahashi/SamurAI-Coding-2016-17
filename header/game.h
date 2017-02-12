#ifndef GAME_H
#define GAME_H

#include <vector>
#include <random>
#include <tuple>
#include <queue>

#include "point.h"
#include "field.h"
#include "state.h"
#include "turn.h"

enum Command {_Paint_, _Move_, _Hide_,
              _Commandsize_};

// calc_point_enemy_by_dijkstra まわり
enum Passtype { _Inpuku_, _Kengen_, _Dochirademo_,
                _Passtypesize_ };
typedef std::tuple<int, Passtype, Point> Pass; // cost, Passtype, Point
typedef std::priority_queue<Pass, std::vector<Pass>,
                            std::greater<Pass> > Pqueue;

class Game {
  bool _is_sente;
  Turn* _turns;
  int _now_turn;

public:
  // game.cpp
  Game();
  ~Game();
  bool is_sente() const { return _is_sente; }
  Turn turns(int t) const { return _turns[t]; }
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
  const static int REACH[];
  const static int EYE;
  const static Point INVISIBLE;
  const static Point dx[];
  static int command(int dir, Command obj);
  static int cost(int dir, Command obj) {
    return COST[command(dir, obj)];
  }
  static int samurai_to_player(int samurai);
  static int player_to_enemy(int player);
  int player_to_samurai(int player, bool is_mikata = true); // init だけ

  // init.cpp
  static void init();
  static std::vector<Point>** _rotate_paint; // [player][dir]
  static std::vector<Point>**** _initial_paint; // [player][x][y][dir]
  static std::set<Point>**** _set_initial_paint;
  static std::vector<State>*** _initial_state; // [samurai][x][y]
  static std::vector<State> initial_state(int samurai, Point p) {
    return _initial_state[samurai][p.x()][p.y()];
  }
  static std::random_device RD;
  static std::mt19937 MT;

  // game.cpp
  void input();
  void play();
  void before_start();
  void each_turn();

  // think.cpp
  Turn& current() { return _turns[_now_turn]; }
  Turn& previous() { return _turns[_now_turn-1]; }
  void think();
  // inform.cpp
  void inform();
  int calc_acted_enemy();
  int calc_acted_enemy_0();
  void calc_kappa();
  void calc_point_enemy();
  void calc_point_enemy_0();
  void calc_point_enemy_acted(int player);
  void calc_point_enemy_by_dijkstra(int player, Pqueue& Q);
  void calc_point_enemy_by_sat(int player);
};

#endif /* GAME_H */
