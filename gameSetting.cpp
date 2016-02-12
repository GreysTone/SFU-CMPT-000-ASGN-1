//
// Created by greystone on 11/02/16.
//

#include "gameSetting.h"

namespace GT_gameSetting {

  // xsize and ysize represent the window size - updated if window is reshaped to prevent stretching of the game
  int xsize = 400;
  int ysize = 720;

  // color
  vec4 palette[7] = {
      vec4(1.0, 0.0, 1.0, 1.0),
      vec4(1.0, 0.0, 0.0, 1.0),
      vec4(0.0, 1.0, 1.0, 1.0),
      vec4(0.0, 1.0, 0.0, 1.0),
      vec4(1.0, 0.5, 0.0, 1.0),
      vec4(1.0, 1.0, 1.0, 1.0),
      vec4(0.0, 0.0, 0.0, 1.0)
  };

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

  //An array containing the colour of each of the 10*20*2*3 vertices that make up the board
  //Initially, all will be set to black. As tiles are placed, sets of 6 vertices (2 triangles; 1 square)
  //will be set to the appropriate colour in this array before updating the corresponding VBO
  vec4 boardcolours[1200];
}
