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

  //TODO: Lyken Code
  model_view = glGetUniformLocation( program, "model_view" );
  projection = glGetUniformLocation( program, "projection" );

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

  //TODO: Lyken Code
  // The location of the uniform variables in the shader program
  locMVP = glGetUniformLocation(program, "MVP");

  // Board is now in unit lengths
  vec4 eye = vec4(0, 20, 1000, 0);
  vec4 at = vec4(0, 20/2, 0, 0);
  View = LookAt(eye, at,vec4(0, 1, 0,0));


  // Game initialization
	newTile(); // create new next tile

	// set to default
	glBindVertexArray(0);
	glClearColor(0, 0, 0, 0);

  // Antialiasing
  glEnable(GL_MULTISAMPLE);
  glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_POINT_SMOOTH);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
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
  //TODO: right setting on CTRL
  bool CTRL = false;
  int modifier = glutGetModifiers();
  if (modifier != 0 && modifier == GLUT_ACTIVE_SHIFT) {
    if (modifier == GLUT_ACTIVE_SHIFT) {
      CTRL = 1;
    }
  }
  else if (CTRL) {
    CTRL = 0;
  }

  switch (key) {
    case 100: // Left Arrow
#ifdef GT_DEBUG_SPECIAL_KEYINFO
      std::cout << "[LEFT Arrow] Pressed.\n";
#endif
      if(CTRL) {
        //TODO: rotate left
        View *= RotateY(-5);
      }
      else {
        moveTile(vec2(-1, 0));
      }
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
      if(CTRL) {
        //TODO: rotate right
        View *= RotateY(5);
      }
      else {
        moveTile(vec2(1, 0));
      }
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
    case 'q':
      exit (EXIT_SUCCESS);
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
//  glClear(GL_COLOR_BUFFER_BIT);
//
//  glUniform1i(locxsize, xsize); // x and y sizes are passed to the shader program to maintain shape of the vertices on screen
//  glUniform1i(locysize, ysize);
//  glUniform1i(loczsize, zsize);
//
//  glBindVertexArray(vaoIDs[1]); // Bind the VAO representing the grid cells (to be drawn first)
//  glDrawArrays(GL_TRIANGLES, 0, GT_GLOBAL_VERTEX_BOARD); // Draw the board (10*20*2 = 400 triangles)
//
//  glBindVertexArray(vaoIDs[2]); // Bind the VAO representing the current tile (to be drawn on top of the board)
//  glDrawArrays(GL_TRIANGLES, 0, GT_GLOBAL_VERTEX_TILE); // Draw the current tile (8 triangles)
//
//  glBindVertexArray(vaoIDs[0]); // Bind the VAO representing the grid lines (to be drawn on top of everything else)
//  glDrawArrays(GL_LINES, 0, GT_GLOBAL_VERTEX_GRID); // Draw the grid lines (21+11 = 32 lines)
//
//  glutSwapBuffers();
  //TODO: Lyken
  Projection = Perspective(45, 1.0*xsize/ysize, 10, 200);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*
  point4  eye( radius*sin(theta)*cos(phi),
         radius*sin(theta)*sin(phi),
         radius*cos(theta),
            1.0 );
    point4  at( 0.0, 0.0, 0.0, 1.0 );
    vec4    up( 0.0, 1.0, 0.0, 0.0 );
  */

  // Scale everything to unit length
  mat4 Model = mat4();

  Model *= Translate(0, 20/2, 0);
  Model *= Scale(1.0/33 , 1.0/33 , 1.0/33);  // scale to unit length
  //Model *= Translate(-33*BOARD_WIDTH/2.0 - 33, -33*BOARD_HEIGHT/2.0 - 33, 0); // move to origin
  mat4 mvp =  Projection *  Model * View;
  glUniformMatrix4fv(locMVP, 1, GL_TRUE, mvp);

  /*
    mat4  p = Ortho( lleft, rright, bottom, top, zNear, zFar );
    glUniformMatrix4fv( projection, 1, GL_TRUE, p );
  */

  glUniform1i(locxsize, xsize); // x and y sizes are passed to the shader program to maintain shape of the vertices on screen
  glUniform1i(locysize, ysize);

  glBindVertexArray(vaoIDs[1]); // Bind the VAO representing the grid cells (to be drawn first)
  glDrawArrays(GL_TRIANGLES, 0, GT_GLOBAL_VERTEX_BOARD); // Draw the board (10*20*2 = 400 triangles)

  glBindVertexArray(vaoIDs[2]); // Bind the VAO representing the current tile (to be drawn on top of the board)
  glDrawArrays(GL_TRIANGLES, 0, GT_GLOBAL_VERTEX_TILE); // Draw the current tile (8 triangles)

  glBindVertexArray(vaoIDs[0]); // Bind the VAO representing the grid lines (to be drawn on top of everything else)
  glDrawArrays(GL_LINES, 0, GT_GLOBAL_VERTEX_GRID); // Draw the grid lines (21+11 = 32 lines)

//  glBindVertexArray(vaoIDs[armVAO]); // Bind the VAO representing the current tile (to be drawn on top of the board)
//  glDrawArrays(GL_TRIANGLES, 0, allPoints); // Draw the current tile (8 triangles)


  glutSwapBuffers();

}

//-------------------------------------------------------------------------------------------------------------------

void
timerDrop(int data) {
  //reset timer
  glutTimerFunc((unsigned int)DROP_SPEED, timerDrop, 0);

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
	glutTimerFunc((unsigned int)DROP_SPEED, timerDrop, 0);
#endif
	glutIdleFunc(idle);

	// Start main loop
	glutMainLoop();
	return 0;
}
