//
// CMPT 361 Assignment 2 - Fruit Tetris
// Implemented in Feb 2016 by Danyang Song (Arthur, arthur_song@sfu.ca).
// (Network ID: GreysTone)
//

#ifndef ASSIGNMENT1_GAMELOGIC_H
#define ASSIGNMENT1_GAMELOGIC_H

#include "include/Angel.h"
#include "gameSetting.h"
#include "gameDrawing.h"
//#include <random>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;
using namespace GT_gameSetting;
using namespace GT_gameDrawing;

namespace GT_gameLogic {
//#define GT_DEBUG_TIMER
//#define GT_DEBUG_COLLISION
//#define GT_DEBUG_TILE_POSITION_ONLINE
//#define GT_DEBUG_TILE_POSITION
//#define GT_DEBUG_OCCUPATION
//#define GT_DEBUG_SOLID_COLOR
#define GT_DEBUG_COLOR_MATRIX
#define GT_DEBUG_ELIMINATION_MATRIX
//#define GT_DEBUG_ELIMINATION
//#define GT_DEBUG_ELIMINATION_HOR
//#define GT_DEBUG_ELIMINATION_VER
//#define GT_DEBUG_ELIMINATION_LDG
//#define GT_DEBUG_ELIMINATION_RDG
//#define GT_DEBUG_ELIMINATION_OUTPUT
//#define GT_DEBUG_ELIMINATION_POINT_DETAIL

  enum gtShape {sI = 0, sS, sL, sT};
  extern gtShape tileShape;
  extern int tileModule;

  enum gtDirection {UL = 0, UP, UR, LE, RI, DL, DO, DR, CTN};

  // board[x][y] represents whether the cell (x,y) is occupied
  extern bool board[10][20];

  // robot arm rotation
  extern int Theta;
  extern int Phi;
  extern int Delta;

  // Searching Temporary Data
  extern bool removingMatrix[10][20];
  extern bool isRemovingMatrixEmpty;

  extern bool gamePause;
  extern int countDown;

//-------------------------------------------------------------------------------------------------------------------

  void init();
  // Called at the start of play and every time a tile is placed
  void newTile();
  // Rotates the current tile, if there is room

void shuffleColor();

  void rotate();
  // Given (x,y), tries to move the tile x squares to the right and y squares down
  // Returns true if the tile was successfully moved, or false if there was some issue
  bool moveTile(vec2 direction);
  // Places the current tile
  void setTile();

  // Collision Detection Function
  bool collisionDetect(gtDirection towards);

  // Eliminate any possible tile that could be eliminated
  void clearWholeMap();
  // Check if all block in game cannot be eliminated
  bool isWholeMapStatic();
  // Eliminate one single block
  void eliminatePoint(int x, int y);
  // Checks if the specified row (0 is the bottom 19 the top) is full
  // If every cell in the row is occupied, it will clear that cell and everything above it will shift down one row
  inline void checkFullRow(int row);


  // Judge if (vec4)a == (vec4)b
  inline bool isColorSame(vec4 a, vec4 b);
  // Output the color's name, using for debug
  void getColorName(int x, int y);

  // Starts the game over - empties the board, creates new tiles, resets line counters
  void restart();

  void special(int key, int x, int y);
  void keyboard(unsigned char key, int x, int y);
  void timerDrop(int data);
void dropTile();
  void newTimer(int data);
} // namespace GT_gameLogic

#endif //ASSIGNMENT1_GAMELOGIC_H
