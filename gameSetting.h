//
// CMPT 361 Assignment 2 - Fruit Tetris
// Implemented in Feb 2016 by Danyang Song (Arthur, arthur_song@sfu.ca).
// (Network ID: GreysTone)
//

#ifndef ASSIGNMENT1_GAMESETTING_H
#define ASSIGNMENT1_GAMESETTING_H

#include "include/Angel.h"
#include <vector>

namespace GT_gameSetting {
#define GT_DEBUG_CLOSE_TIMER
//#define GT_DEBUG_SPECIAL_KEYINFO
//#define GT_SET_ANTI_ALIASING

#define GT_GLOBAL_OBJECT_AMOUNT (4)
#define GT_GLOBAL_HEIGHT_BOARD (20)
#define GT_GLOBAL_WIDTH_BOARD (10)

  // Maximum SuperPower is 100
//#define GT_GLOBAL_VERTEX_GRID (5900)         // 2[planes] * (11+21)[lines/plane] * 2[points/line]
#define GT_GLOBAL_VERTEX_BOARD (72000)       // (20*10)[cubes] * 6[planes/cube] * 6[points/plane]
#define GT_GLOBAL_VERTEX_TILE (144)         // (4)[cubes] * 6[planes/cube] * 6[points/plane]
#define GT_GLOBAL_VERTEX_ARM (108)          // (3)[cubes] * 6[planes/cube] * 6[points/plane]
#define GT_GLOBAL_VERTEX_SINGLE_CUBE (36)

  // window size
  extern int xsize;
  extern int ysize;
  extern int zsize;

  // drop control
  extern int DROP_SPEED;
  extern int DROP_SHIFT;

  // palette
  extern vec4 palette[8];
  enum gtColor {purple = 0, red, yellow, green, orange, white, black, grey};

  // object
  enum gtObject {objGrid = 0, objBoard, objTile, objArm};
//  extern std::vector<gtObject> objectStack;

  // current tile
  extern vec2 tile[4]; // An array of 4 2d vectors representing displacement from a 'center' piece of the tile, on the grid
  extern vec4 tilepos; // The position of the current tile using grid coordinates ((0,0) is the bottom left corner)
  extern gtColor tiledColor[4];

  // An array storing all possible orientations of all possible tiles
  // The 'tile' array will always be some element [i][j] of this array (an array of vec2)
  extern vec2 allRotationsLShape[4][4];
  extern vec2 allRotationsIShape[2][4];
  extern vec2 allRotationsSShape[2][4];
  extern vec2 allRotationsTShape[4][4];

extern bool CTRL;

  extern int superPower;
  extern vec4 *gridVertex;
  extern vec4 *gridColour;
  extern long int gridPointCount;
  extern vec4 *boardVertex;
  extern vec4 *boardColour;
  extern long int boardPointCount;


} // namespace GT_gameSetting


#endif //ASSIGNMENT1_GAMESETTING_H
