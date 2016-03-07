/*
CMPT 361 Assignment 1 - Fruit Tetris

- This work is based on a reformat skeleton code

- Compile and Run:
Type make in terminal, then type ./FruitTetris

This code is extracted from Connor MacLeod's (crmacleo@sfu.ca) assignment submission by Rui Ma (ruim@sfu.ca) on 2014-03-04.
Modified in Sep 2014 by Honghua Li (honghual@sfu.ca).

Implemented in Feb 2016 by Danyang Song (Arthur, arthur_song@sfu.ca).
(Network ID: GreysTone)

*/

#include <iostream>
#include "include/Angel.h"
#include "gameLogic.h"

using namespace GT_gameLogic;

void
init() {
	// Load shaders and use the shader program
#ifdef __APPLE__
    // replace with a absolute path for two shader
    GLuint program = InitShader(
                "/Users/GreysTone/Desktop/Dev/SFU-CMPT-000-ASGN-1/FruitTetris_XCode/FruitTetris_source_XCode/vshader.glsl",
                "/Users/GreysTone/Desktop/Dev/SFU-CMPT-000-ASGN-1/FruitTetris_XCode/FruitTetris_source_XCode/fshader.glsl"
                                );
#else
    GLuint program = InitShader("vshader.glsl", "fshader.glsl");
#endif
	glUseProgram(program);

	// Get the location of the attributes (for glVertexAttribPointer() calls)
	vPosition = glGetAttribLocation(program, "vPosition");
	vColor = glGetAttribLocation(program, "vColor");

	// Create 3 Vertex Array Objects, each representing one 'object'. Store the names in array vaoIDs
	glGenVertexArrays(3, &vaoIDs[0]);

	// Initialize the grid, the board, and the current tile
	initGrid();
	initBoard();
	initCurrentTile();

	// The location of the uniform variables in the shader program
	locxsize = glGetUniformLocation(program, "xsize");
	locysize = glGetUniformLocation(program, "ysize");
  loczsize = glGetUniformLocation(program, "zsize");

	// Game initialization
	newTile(); // create new next tile

	// set to default
	glBindVertexArray(0);
	glClearColor(0, 0, 0, 0);
}

//-------------------------------------------------------------------------------------------------------------------

// Reshape callback will simply change xsize and ysize variables, which are passed to the vertex shader
// to keep the game the same from stretching if the window is stretched
void
reshape(GLsizei w, GLsizei h) {
  xsize = w;
  ysize = h;
  glViewport(0, 0, w, h);
}

// Handle arrow key keypresses
void
special(int key, int x, int y) {
  if(gamePause) return;  // Skip triggering special key when game is pause
  switch (key) {
    case 100: // Left Arrow
#ifdef GT_DEBUG_SPECIAL_KEYINFO
      std::cout << "[LEFT Arrow] Pressed.\n";
#endif
      moveTile(vec2(-1, 0));
      break;
    case 101: // Up Arrow (Rotate Tile)
#ifdef GT_DEBUG_SPECIAL_KEYINFO
      std::cout << "[UP Arrow] Pressed.\n";
#endif
      rotate();
      break;
    case 102: // Right Arrow
#ifdef GT_DEBUG_SPECIAL_KEYINFO
      std::cout << "[RIGHT Arrow] Pressed.\n";
#endif
      moveTile(vec2(1, 0));
      break;
    case 103: // Down Arrow
#ifdef GT_DEBUG_SPECIAL_KEYINFO
      std::cout << "[DOWN Arrow] Pressed.\n";
#endif
      moveTile(vec2(0, -1));
      break;
  }
}

// Handles standard key press
void
keyboard(unsigned char key, int x, int y) {
  switch(key)
  {
    case 033: // Both escape key and 'q' cause the game to exit
      exit(EXIT_SUCCESS);
      break;
    case 'q':
      exit (EXIT_SUCCESS);
      break;
    case 't': // 't' key rotates the tile
      rotate();
      break;
    case 'p': // 'p' key pauses the game
      gamePause = !gamePause;
      break;
    case 'a': // 'a' key accelerates the drop speed
      if(DROP_SPEED > DROP_SHIFT) DROP_SPEED -= DROP_SHIFT;
      break;
    case 'r': // 'r' key restarts the game
      restart();
      break;
  }
  glutPostRedisplay();
}

// Draws the game
void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  glUniform1i(locxsize, xsize); // x and y sizes are passed to the shader program to maintain shape of the vertices on screen
  glUniform1i(locysize, ysize);
  glUniform1i(loczsize, zsize);

  glBindVertexArray(vaoIDs[1]); // Bind the VAO representing the grid cells (to be drawn first)
  glDrawArrays(GL_TRIANGLES, 0, 1200); // Draw the board (10*20*2 = 400 triangles)

  glBindVertexArray(vaoIDs[2]); // Bind the VAO representing the current tile (to be drawn on top of the board)
  glDrawArrays(GL_TRIANGLES, 0, 24); // Draw the current tile (8 triangles)

  glBindVertexArray(vaoIDs[0]); // Bind the VAO representing the grid lines (to be drawn on top of everything else)
  glDrawArrays(GL_LINES, 0, GT_GLOBAL_VERTEX_GRID); // Draw the grid lines (21+11 = 32 lines)

  glutSwapBuffers();
}

//-------------------------------------------------------------------------------------------------------------------

void
timerDrop(int data) {
  //reset timer
  glutTimerFunc(DROP_SPEED, timerDrop, 0);

  if(!gamePause) {  // when game is pause, stop handling tiles
#ifdef GT_DEBUG_TIMER
    std::cout << "timerDrop triggered - " << data << "\n";
#endif
    moveTile(vec2(0, -2));
    glutPostRedisplay();
  }
}

void
idle() {
  glutPostRedisplay();
}

//-------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	// OpenGL Utility ToolKit - Configure a window
	glutInit(&argc, argv);
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE);
#else
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
#endif
	glutInitWindowSize(xsize, ysize);
	glutInitWindowPosition(680, 178); // Center the game window (well, on a 1920x1080 display)
	glutCreateWindow("Fruit Tetris");

#ifndef __APPLE__
	// Initialize GLEW
	if(glewInit()) {
    std::cerr << "Unable to initialize GLEW ... exiting.\n";
		exit(EXIT_FAILURE);
	}
#endif

	init();
  srand (time(NULL)); // initialize random seed

	// Callback functions
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);
  glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);

#ifndef GT_DEBUG_CLOSE_TIMER
	glutTimerFunc(DROP_SPEED, timerDrop, 0);
#endif
	glutIdleFunc(idle);

	// Start main loop
	glutMainLoop();
	return 0;
}
