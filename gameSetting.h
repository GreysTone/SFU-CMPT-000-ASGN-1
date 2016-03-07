//
// Created by Danyang Song on 11/02/16.
//

#ifndef ASSIGNMENT1_GAMESETTING_H
#define ASSIGNMENT1_GAMESETTING_H

#include "include/Angel.h"

namespace GT_gameSetting {

//#define GT_DEBUG_TIMER
//#define GT_DEBUG_CLOSE_TIMER
//#define GT_DEBUG_SPECIAL_KEYINFO

#define GT_GLOBAL_VERTEX_GRID (128)         // 2[planes] * (11+21)[lines/plane] * 2[points/line]
#define GT_GLOBAL_VERTEX_BOARD (7200)       // (20*10)[cubes] * 6[planes/cube] * 6[points/plane]
#define GT_GLOBAL_VERTEX_TILE (144)         // (4)[cubes] * 6[planes/cube] * 6[points/plane]
#define GT_GLOBAL_VERTEX_SINGLE_CUBE (36)

  extern int DROP_SPEED;
  extern int DROP_SHIFT;

  extern int xsize;
  extern int ysize;
  extern int zsize;

  // colors
  extern vec4 palette[7];
  enum gtColor {purple = 0, red, yellow, green, orange, white, black};

  // An array storing all possible orientations of all possible tiles
  // The 'tile' array will always be some element [i][j] of this array (an array of vec2)
  extern vec2 allRotationsLShape[4][4];
  extern vec2 allRotationsIShape[2][4];
  extern vec2 allRotationsSShape[2][4];
  extern vec2 allRotationsTShape[4][4];

  //An array containing the colour of each of the 10*20*2*3 vertices that make up the board
  //Initially, all will be set to black. As tiles are placed, sets of 6 vertices (2 triangles; 1 square)
  //will be set to the appropriate colour in this array before updating the corresponding VBO
  extern vec4 boardcolours[GT_GLOBAL_VERTEX_BOARD];
}


#endif //ASSIGNMENT1_GAMESETTING_H
