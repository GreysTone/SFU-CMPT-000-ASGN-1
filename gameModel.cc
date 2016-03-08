//
// CMPT 361 Assignment 2 - Fruit Tetris
// Implemented in Feb 2016 by Danyang Song (Arthur, arthur_song@sfu.ca).
// (Network ID: GreysTone)
//

#include "include/Angel.h"
#include "gameSetting.h"
#include "gameModel.h"

namespace GT_gameModel {
namespace GRID {
vec4 vertexArray[GT_GLOBAL_VERTEX_GRID];
vec4 colourArray[GT_GLOBAL_VERTEX_GRID];
}

namespace BOARD {
vec4 vertexArray[GT_GLOBAL_VERTEX_BOARD];
vec4 colourArray[GT_GLOBAL_VERTEX_BOARD];
}

namespace TILE {
vec4 vertexArray[GT_GLOBAL_VERTEX_TILE];
vec4 colourArray[GT_GLOBAL_VERTEX_TILE];
}
namespace ARM{

vec4 points[GT_GLOBAL_VERTEX_SINGLE_CUBE];
vec4 colors[GT_GLOBAL_VERTEX_SINGLE_CUBE];

vec4 vertexArray[8] = {
    vec4( -0.5, -0.5,  0.5, 1.0 ),
    vec4( -0.5,  0.5,  0.5, 1.0 ),
    vec4(  0.5,  0.5,  0.5, 1.0 ),
    vec4(  0.5, -0.5,  0.5, 1.0 ),
    vec4( -0.5, -0.5, -0.5, 1.0 ),
    vec4( -0.5,  0.5, -0.5, 1.0 ),
    vec4(  0.5,  0.5, -0.5, 1.0 ),
    vec4(  0.5, -0.5, -0.5, 1.0 )
};

vec4 colourArray[8] = {
    vec4( 0.0, 0.0, 0.0, 1.0 ),  // black
    vec4( 1.0, 0.0, 0.0, 1.0 ),  // red
    vec4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    vec4( 0.0, 1.0, 0.0, 1.0 ),  // green
    vec4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    vec4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    vec4( 1.0, 1.0, 1.0, 1.0 ),  // white
    vec4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};

int Index = 0;

void
quad( int a, int b, int c, int d ) {
  colors[Index] = colourArray[a]; points[Index] = vertexArray[a]; Index++;
  colors[Index] = colourArray[a]; points[Index] = vertexArray[b]; Index++;
  colors[Index] = colourArray[a]; points[Index] = vertexArray[c]; Index++;
  colors[Index] = colourArray[a]; points[Index] = vertexArray[a]; Index++;
  colors[Index] = colourArray[a]; points[Index] = vertexArray[c]; Index++;
  colors[Index] = colourArray[a]; points[Index] = vertexArray[d]; Index++;
}

int      Axis = Base;
GLfloat  Theta[NumAngles] = { 0.0 };


void setupModel() {
  quad( 1, 0, 3, 2 );
  quad( 2, 3, 7, 6 );
  quad( 3, 0, 4, 7 );
  quad( 6, 5, 1, 2 );
  quad( 4, 5, 6, 7 );
  quad( 5, 4, 0, 1 );
}

} //namespace ARM
} // namespace GT_gameModel