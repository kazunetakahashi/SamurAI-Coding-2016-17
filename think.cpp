#include <random>
#include <cassert>
#include <chrono>
#include <iostream>
#include <iomanip>

#include "header/game.h"

using namespace std;

# define DEBUG 1

void Game::think() {
#if DEBUG == 1
  cerr << "think() start." << endl;
  auto start = chrono::system_clock::now();  
#endif
  inform();
#if DEBUG == 1
  cerr << current() << endl;
  auto end = chrono::system_clock::now();
  double elapsed
    = chrono::duration_cast<chrono::milliseconds>(end-start).count();
  cerr << "inform: " << fixed << setprecision(2)
       << elapsed << " [ms]" << endl;  
#endif
  current().evaluate();
#if DEBUG == 1
  end = chrono::system_clock::now();
  elapsed
    = chrono::duration_cast<chrono::milliseconds>(end-start).count();
  cerr << "evaluate: " << fixed << setprecision(2)
       << elapsed << " [ms]" << endl;  
#endif
  current().choose();
#if DEBUG == 1
  end = chrono::system_clock::now();
  elapsed
    = chrono::duration_cast<chrono::milliseconds>(end-start).count();
  cerr << "choose: " << fixed << setprecision(2)
       << elapsed << " [ms]" << endl;  
#endif
}
