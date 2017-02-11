#include <cmath>

#include "header/game.h"

using namespace std;

double Turn::death_prob(int player, Point p, Point q, bool is_hidden) {
  if (!is_hidden) {
    if (player == 0) {
      if (p.dist(q) > Game::REACH[player]) {
        return 0.0;
      } else if (abs(p.x() - q.x()) > 1 || abs(p.y() - q.y()) > 1) {
        return 0.0;
      } else {
        return 1.0;
      }
    } else {
      if (p.dist(q) <= Game::REACH[player]) {
        return 1.0;
      } else {
        return 0.0;
      }
    }
  } else {
    if (player == 0) {
      if (p.dist(q) > Game::REACH[player]) {
        return 0.0;
      } else if (abs(p.x() - q.x()) > 1 || abs(p.y() - q.y()) > 1) {
        return 0.0;
      } else if (abs(p.x() - q.x()) == 1 || abs(p.y() - q.y()) == 1) {
        return 0.5;
      } else {
        return 0.8;
      }
    } else if (player == 1) {
      if (p.dist(q) < Game::REACH[player]) {
        return 0.4;
      } else if (p.dist(q) == Game::REACH[player]) {
        return 0.25;
      } else {
        return 0.0;
      }
    } else {
      if (p.dist(q) < Game::REACH[player]) {
        return 0.8;
      } else if (p.dist(q) == Game::REACH[player]) {
        return 0.5;
      } else {
        return 0.0;
      }
    }
  }
}
