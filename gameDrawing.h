//
// CMPT 361 Assignment 2 - Fruit Tetris
// Implemented in Feb 2016 by Danyang Song (Arthur, arthur_song@sfu.ca).
// (Network ID: GreysTone)
//

#ifndef ASSIGNMENT1_GAMEDRAWING_H
#define ASSIGNMENT1_GAMEDRAWING_H

#include "include/Angel.h"
#include "gameSetting.h"
#include "gameModel.h"

using namespace GT_gameModel;
using namespace GT_gameModel::ARM;

namespace GT_gameDrawing {
#define GT_GLOBAL_EYE_Z (1500)
#define GT_GLOBAL_PROJECT_ANGLE (45)
#define GT_GLOBAL_PROJECT_Z_NEAR (1)
#define GT_GLOBAL_PROJECT_Z_FAR (40)

  // location of vertex attributes in the shader program
  extern GLint vPosition;
  extern GLint vColor;

  // locations of uniform variables in shader program
  extern GLint locxsize;
  extern GLint locysize;
  extern GLint loczsize;

  // projection
  extern GLint locMVPMatrix;
  extern mat4 ModelMat, ViewMat, ProjectionMat;

  //TODO: [RESET]Projection
  extern mat4 model_view;
  extern mat4 projection;
  extern GLuint ModelView, Projection;

  // projections setting
  extern vec4 projectionEye;
  extern vec4 projectionAt;
  extern vec4 projectionUp;

  // VAO and VBO
  extern GLuint vaoIDs[GT_GLOBAL_OBJECT_AMOUNT]; // One VAO for each object: the grid, the board, the current piece
  extern GLuint vboIDs[GT_GLOBAL_OBJECT_AMOUNT * 2]; // Two Vertex Buffer Objects for each VAO (specifying vertex positions and colours, respectively)

  //An array containing the colour of each of the 10*20*2*3 vertices that make up the board
  //Initially, all will be set to black. As tiles are placed, sets of 6 vertices (2 triangles; 1 square)
  //will be set to the appropriate colour in this array before updating the corresponding VBO
  extern vec4 boardcolours[GT_GLOBAL_VERTEX_BOARD];

  // Initialize OpenGL
  void initOpenGL();
  // Initialize Grid
  void initGrid();
  // Initialize Board
  void initBoard();
  // No geometry for current tile initially
  void initCurrentTile();
  // Initialize Arm
  void initArm();

  // When the current tile is moved or rotated (or created), update the VBO containing its vertex position data
  void updateTile();
  // Update the board's color data
  //TODO: [not handle right now]inline effect in diff namespace
  void updateBoardColor(int x, int y, vec4 color);

  void reshape(GLsizei w, GLsizei h);
  void display();
  void idle();

//  void pipelineUpdate(GT_gameSetting::gtObject);

} // namespace GT_gameDrawing


#endif //ASSIGNMENT1_GAMEDRAWING_H
