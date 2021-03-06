/*
CMPT 361 Assignment 2 - Fruit Tetris

- This work is based on a reformat skeleton code

- Compile and Run:
Type make in terminal, then type ./FruitTetris

This code is extracted from Connor MacLeod's (crmacleo@sfu.ca) assignment submission by Rui Ma (ruim@sfu.ca) on 2014-03-04.
Modified in Sep 2014 by Honghua Li (honghual@sfu.ca).

Implemented in Feb 2016 by Danyang Song (Arthur, arthur_song@sfu.ca).
(Network ID: GreysTone)

*/

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/gl3.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

#include "gameSetting.h"
#include "gameDrawing.h"
#include "gameLogic.h"

#include <iostream>

int main(int argc, char **argv)
{
  std::cout << "input n size:";
  cin >> GT_gameSetting::superPower;
  if(GT_gameSetting::superPower < 1) GT_gameSetting::superPower = 1;
  if(GT_gameSetting::superPower > 100) GT_gameSetting::superPower = 100;

	// OpenGL Utility ToolKit - Configure a window
	glutInit(&argc, argv);
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE);
#else
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
#endif
	glutInitWindowSize(GT_gameSetting::xsize, GT_gameSetting::ysize);
  // center the game window (well, on a 1920x1080 display)
	glutInitWindowPosition(680, 178);
	glutCreateWindow("Fruit Tetris");

#ifndef __APPLE__
	// Initialize GLEW
	if(glewInit()) {
    std::cerr << "Unable to initialize GLEW ... exiting.\n";
		exit(EXIT_FAILURE);
	}
#endif

	GT_gameLogic::init();
  srand ((unsigned int)time(NULL)); // initialize random seed

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Callback functions
  glutReshapeFunc(GT_gameDrawing::reshape);
  glutSpecialFunc(GT_gameLogic::special);
  glutKeyboardFunc(GT_gameLogic::keyboard);
	glutDisplayFunc(GT_gameDrawing::display);

#ifndef GT_DEBUG_CLOSE_TIMER
//	glutTimerFunc((unsigned int)DROP_SPEED, GT_gameLogic::timerDrop, 0);
#endif
  //TODO: Reopen Timer
  glutTimerFunc(1000, GT_gameLogic::newTimer, 0);
	glutIdleFunc(GT_gameDrawing::idle);

	// Start main loop
	glutMainLoop();
	return 0;
}
