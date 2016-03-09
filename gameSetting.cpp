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
  vec4 palette[8] = {
      vec4(1.0, 0.5, 1.0, 1.0),
      vec4(1.0, 0.2, 0.1, 1.0),
      vec4(1.0, 1.0, 0.3, 1.0),
      vec4(0.3, 1.0, 0.6, 1.0),
      vec4(1.0, 0.6, 0.2, 1.0),
      vec4(1.0, 1.0, 1.0, 1.0),
      vec4(0.0, 0.0, 0.0, 0.0),
      vec4(0.5, 0.5, 0.4, 1.0)
  };

  // object
//  std::vector<gtObject> objectStack;

  // current tile
  vec4 tile[4]; // An array of 4 2d vectors representing displacement from a 'center' piece of the tile, on the grid
  vec4 tilepos = vec4(9, 18, 0, 1); // The position of the current tile using grid coordinates ((0,0) is the bottom left corner)
  gtColor tiledColor[4];

  // An array storing all possible orientations of all possible tiles
  // The 'tile' array will always be some element [i][j] of this array (an array of vec2)
  vec4 allRotationsLShape[4][4] = {
      {vec4( 1, 0, 0, 0), vec4( 0, 0, 0, 0), vec4(-1, 0, 0, 0), vec4(-1,-1, 0, 0)},
      {vec4( 0, 1, 0, 0), vec4( 0, 0, 0, 0), vec4( 0,-1, 0, 0), vec4( 1,-1, 0, 0)},
      {vec4(-1, 0, 0, 0), vec4( 0, 0, 0, 0), vec4( 1, 0, 0, 0), vec4( 1, 1, 0, 0)},
      {vec4( 0,-1, 0, 0), vec4( 0, 0, 0, 0), vec4( 0, 1, 0, 0), vec4(-1, 1, 0, 0)}
  };
  vec4 allRotationsIShape[2][4] = {
      {vec4(-2, 0, 0, 0), vec4(-1, 0, 0, 0), vec4( 0, 0, 0, 0), vec4( 1, 0, 0, 0)},
      {vec4( 0,-2, 0, 0), vec4( 0,-1, 0, 0), vec4( 0, 0, 0, 0), vec4( 0, 1, 0, 0)}
  };
  vec4 allRotationsSShape[2][4] = {
      {vec4(-1,-1, 0, 0), vec4( 0,-1, 0, 0), vec4( 0, 0, 0, 0), vec4( 1, 0, 0, 0)},
      {vec4( 1,-1, 0, 0), vec4( 1, 0, 0, 0), vec4( 0, 0, 0, 0), vec4( 0, 1, 0, 0)}
  };
  vec4 allRotationsTShape[4][4] = {
      {vec4(-1, 0, 0, 0), vec4( 0, 0, 0, 0), vec4( 1, 0, 0, 0), vec4( 0,-1, 0, 0)},
      {vec4( 0,-1, 0, 0), vec4( 0, 0, 0, 0), vec4( 0, 1, 0, 0), vec4( 1, 0, 0, 0)},
      {vec4( 1, 0, 0, 0), vec4( 0, 0, 0, 0), vec4(-1, 0, 0, 0), vec4( 0, 1, 0, 0)},
      {vec4( 0, 1, 0, 0), vec4( 0, 0, 0, 0), vec4( 0,-1, 0, 0), vec4(-1, 0, 0, 0)}
  };

  int superPower = 1;

  vec4 *gridVertex = NULL;
  vec4 *gridColour = NULL;
  long int gridPointCount = 0;

  vec4 *boardVertex = NULL;
  vec4 *boardColour = NULL;
  long int boardPointCount = 0;

} // namespace GT_gameSetting
