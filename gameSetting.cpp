//
// CMPT 361 Assignment 2 - Fruit Tetris
// Implemented in Feb 2016 by Danyang Song (Arthur, arthur_song@sfu.ca).
// (Network ID: GreysTone)
//

#include "gameSetting.h"

namespace GT_gameSetting {

  // window size
  int xsize = 400;
  int ysize = 720;
  int zsize = 33;

  // drop control
  int DROP_SPEED = 600;
  int DROP_SHIFT = 100;

  // palette
  vec4 palette[7] = {
      vec4(1.0, 0.5, 1.0, 1.0),
      vec4(1.0, 0.2, 0.1, 1.0),
      vec4(1.0, 1.0, 0.3, 1.0),
      vec4(0.3, 1.0, 0.6, 1.0),
      vec4(1.0, 0.6, 0.2, 1.0),
      vec4(1.0, 1.0, 1.0, 1.0),
      vec4(0.0, 0.0, 0.0, 1.0)
  };

  // object
//  std::vector<gtObject> objectStack;

  // current tile
  vec2 tile[4]; // An array of 4 2d vectors representing displacement from a 'center' piece of the tile, on the grid
  vec2 tilepos = vec2(5, 19); // The position of the current tile using grid coordinates ((0,0) is the bottom left corner)
  gtColor tiledColor[4];

  // An array storing all possible orientations of all possible tiles
  // The 'tile' array will always be some element [i][j] of this array (an array of vec2)
  vec2 allRotationsLShape[4][4] = {
      {vec2( 1, 0), vec2( 0, 0), vec2(-1, 0), vec2(-1,-1)},
      {vec2( 0, 1), vec2( 0, 0), vec2( 0,-1), vec2( 1,-1)},
      {vec2(-1, 0), vec2( 0, 0), vec2( 1, 0), vec2( 1, 1)},
      {vec2( 0,-1), vec2( 0, 0), vec2( 0, 1), vec2(-1, 1)}
  };
  vec2 allRotationsIShape[2][4] = {
      {vec2(-2, 0), vec2(-1, 0), vec2( 0, 0), vec2( 1, 0)},
      {vec2( 0,-2), vec2( 0,-1), vec2( 0, 0), vec2( 0, 1)}
  };
  vec2 allRotationsSShape[2][4] = {
      {vec2(-1,-1), vec2( 0,-1), vec2( 0, 0), vec2( 1, 0)},
      {vec2( 1,-1), vec2( 1, 0), vec2( 0, 0), vec2( 0, 1)}
  };
  vec2 allRotationsTShape[4][4] = {
      {vec2(-1, 0), vec2( 0, 0), vec2( 1, 0), vec2( 0,-1)},
      {vec2( 0,-1), vec2( 0, 0), vec2( 0, 1), vec2( 1, 0)},
      {vec2( 1, 0), vec2( 0, 0), vec2(-1, 0), vec2( 0, 1)},
      {vec2( 0, 1), vec2( 0, 0), vec2( 0,-1), vec2(-1, 0)}
  };

} // namespace GT_gameSetting
