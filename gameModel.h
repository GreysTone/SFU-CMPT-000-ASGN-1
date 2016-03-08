//
// CMPT 361 Assignment 2 - Fruit Tetris
// Implemented in Feb 2016 by Danyang Song (Arthur, arthur_song@sfu.ca).
// (Network ID: GreysTone)
//

#ifndef ASSIGNMENT1_GAMEMODEL_H
#define ASSIGNMENT1_GAMEMODEL_H

namespace GT_gameModel {

namespace GRID {
extern vec4 vertexArray[GT_GLOBAL_VERTEX_GRID];
extern vec4 colourArray[GT_GLOBAL_VERTEX_GRID];
}

namespace BOARD {
extern vec4 vertexArray[GT_GLOBAL_VERTEX_BOARD];
extern vec4 colourArray[GT_GLOBAL_VERTEX_BOARD];
}

namespace TILE {
extern vec4 vertexArray[GT_GLOBAL_VERTEX_TILE];
extern vec4 colourArray[GT_GLOBAL_VERTEX_TILE];
}

namespace ARM {
extern vec4 points[GT_GLOBAL_VERTEX_SINGLE_CUBE];
extern vec4 colors[GT_GLOBAL_VERTEX_SINGLE_CUBE];

const GLfloat BASE_HEIGHT      = 2.0;
const GLfloat BASE_WIDTH       = 5.0;
const GLfloat LOWER_ARM_HEIGHT = 5.0;
const GLfloat LOWER_ARM_WIDTH  = 0.5;
const GLfloat UPPER_ARM_HEIGHT = 5.0;
const GLfloat UPPER_ARM_WIDTH  = 0.5;

extern vec4 vertexArray[8];
extern vec4 colourArray[8];

enum { Base = 0, LowerArm = 1, UpperArm = 2, NumAngles = 3 };
extern int Axis;
extern GLfloat Theta[NumAngles];

void setupModel();

}
} //namespace GT_gameModel


#endif //ASSIGNMENT1_GAMEMODEL_H
