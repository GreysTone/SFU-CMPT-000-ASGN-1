//
// Created by Danyang Song on 10/02/16.
//

#ifndef ASSIGNMENT1_GAMELOGIC_H
#define ASSIGNMENT1_GAMELOGIC_H

#include "include/Angel.h"
#include "gameSetting.h"
//#include <random>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;
using namespace GT_gameSetting;

namespace GT_gameLogic {

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

  // current tile
  extern vec2 tile[4]; // An array of 4 2d vectors representing displacement from a 'center' piece of the tile, on the grid
  extern vec2 tilepos; // The position of the current tile using grid coordinates ((0,0) is the bottom left corner)
  extern gtColor tiledColor[4];

  enum gtShape {sI = 0, sS, sL, sT};
  extern gtShape tileShape;
  extern int tileModule;

  enum gtDirection {UL = 0, UP, UR, LE, RI, DL, DO, DR, CTN};

  //board[x][y] represents whether the cell (x,y) is occupied
  extern bool board[10][20];

  // location of vertex attributes in the shader program
  extern GLuint vPosition;
  extern GLuint vColor;

  // locations of uniform variables in shader program
  extern GLint locxsize;
  extern GLint locysize;

  // VAO and VBO
  extern GLuint vaoIDs[3]; // One VAO for each object: the grid, the board, the current piece
  extern GLuint vboIDs[6]; // Two Vertex Buffer Objects for each VAO (specifying vertex positions and colours, respectively)

  // random generator
  //  extern std::default_random_engine generator;
  //  extern std::uniform_int_distribution<int> dist;

  // Searching Temporary Data
  extern bool removingMatrix[10][20];
  extern bool isRemovingMatrixEmpty;

  extern bool gamePause;

//-------------------------------------------------------------------------------------------------------------------

  // Initialize Grid
  void initGrid();
  // Initialize Board
  void initBoard();
  // No geometry for current tile initially
  void initCurrentTile();


  // Called at the start of play and every time a tile is placed
  void newTile();
  // Rotates the current tile, if there is room
  void rotate();
  // Given (x,y), tries to move the tile x squares to the right and y squares down
  // Returns true if the tile was successfully moved, or false if there was some issue
  bool moveTile(vec2 direction);
  // Places the current tile - update the board vertex colour VBO and the array maintaining occupied cells
  void setTile();
  // When the current tile is moved or rotated (or created), update the VBO containing its vertex position data
  void updateTile();

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
  // Update the board's color data
  inline void updateBoardColor(int x, int y, vec4 color);

  // Judge if (vec4)a == (vec4)b
  inline bool isColorSame(vec4 a, vec4 b);
  // Output the color's name, using for debug
  void getColorName(int x, int y);

  // Starts the game over - empties the board, creates new tiles, resets line counters
  void restart();
}

#endif //ASSIGNMENT1_GAMELOGIC_H
