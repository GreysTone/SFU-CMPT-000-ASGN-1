//
// Created by Danyang Song on 10/02/16.
//

#ifndef ASSIGNMENT1_GAMELOGIC_H
#define ASSIGNMENT1_GAMELOGIC_H

#include "include/Angel.h"
#include <random>

namespace GT_gameLogic {

  extern int xsize;
  extern int ysize;

  // colors
  extern vec4 palette[7];
  enum gtColor {purple = 0, red, yellow, green, orange, white, black};

  // current tile
  extern vec2 tile[4]; // An array of 4 2d vectors representing displacement from a 'center' piece of the tile, on the grid
  extern vec2 tilepos; // The position of the current tile using grid coordinates ((0,0) is the bottom left corner)

  // An array storing all possible orientations of all possible tiles
  // The 'tile' array will always be some element [i][j] of this array (an array of vec2)
  extern vec2 allRotationsLShape[4][4];
  extern vec2 allRotationsIShape[2][4];
  extern vec2 allRotationsSShape[2][4];
  extern vec2 allRotationsTShape[4][4];

  //board[x][y] represents whether the cell (x,y) is occupied
  extern bool board[10][20];

  //An array containing the colour of each of the 10*20*2*3 vertices that make up the board
  //Initially, all will be set to black. As tiles are placed, sets of 6 vertices (2 triangles; 1 square)
  //will be set to the appropriate colour in this array before updating the corresponding VBO
  extern vec4 boardcolours[1200];

  // location of vertex attributes in the shader program
  extern GLint vPosition;
  extern GLint vColor;

  // locations of uniform variables in shader program
  extern GLint locxsize;
  extern GLint locysize;

  // VAO and VBO
  extern GLuint vaoIDs[3]; // One VAO for each object: the grid, the board, the current piece
  extern GLuint vboIDs[6]; // Two Vertex Buffer Objects for each VAO (specifying vertex positions and colours, respectively)

  // random generator
  extern std::default_random_engine generator;
  extern std::uniform_int_distribution<int> distribution(0,6);

//-------------------------------------------------------------------------------------------------------------------

  // Reshape callback will simply change xsize and ysize variables, which are passed to the vertex shader
  // to keep the game the same from stretching if the window is stretched
  void reshape(GLsizei w, GLsizei h);

  // Handle arrow key keypresses
  void special(int key, int x, int y);

  // Handles standard keypresses
  void keyboard(unsigned char key, int x, int y);



  // Called at the start of play and every time a tile is placed
  void newTile();

  void initGrid();

  void initBoard();

  // No geometry for current tile initially
  void initCurrentTile();

  // Rotates the current tile, if there is room
  void rotate();

  // Checks if the specified row (0 is the bottom 19 the top) is full
  // If every cell in the row is occupied, it will clear that cell and everything above it will shift down one row
  void checkfullrow(int row);

  // Places the current tile - update the board vertex colour VBO and the array maintaining occupied cells
  void settile();

  // Given (x,y), tries to move the tile x squares to the right and y squares down
  // Returns true if the tile was successfully moved, or false if there was some issue
  bool movetile(vec2 direction);

  // Starts the game over - empties the board, creates new tiles, resets line counters
  void restart();


  // When the current tile is moved or rotated (or created), update the VBO containing its vertex position data
  void updateTile();


  void timer(void);

  void idle(void);

  void display();

}

#endif //ASSIGNMENT1_GAMELOGIC_H
