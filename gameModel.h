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

void setupModel();
}

namespace BOARD {
extern vec4 vertexArray[GT_GLOBAL_VERTEX_BOARD];
extern vec4 colourArray[GT_GLOBAL_VERTEX_BOARD];

void setupModel();
}

namespace TILE {
extern vec4 vertexArray[GT_GLOBAL_VERTEX_TILE];
extern vec4 colourArray[GT_GLOBAL_VERTEX_TILE];
}

namespace ARM {
extern vec4 vertexArray[GT_GLOBAL_VERTEX_SINGLE_CUBE];
extern vec4 colourArray[GT_GLOBAL_VERTEX_SINGLE_CUBE];

extern int Theta;
extern int Phi;

void setupModel();

}
} //namespace GT_gameModel


#endif //ASSIGNMENT1_GAMEMODEL_H
