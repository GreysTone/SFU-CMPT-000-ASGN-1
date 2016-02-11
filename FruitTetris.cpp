/*
CMPT 361 Assignment 1 - Fruit Tetris

- This work is based on a reformat skeleton code

- Compile and Run:
Type make in terminal, then type ./FruitTetris

This code is extracted from Connor MacLeod's (crmacleo@sfu.ca) assignment submission by Rui Ma (ruim@sfu.ca) on 2014-03-04.
Modified in Sep 2014 by Honghua Li (honghual@sfu.ca).

Implemented in Feb 2016 by Danyang Song (Arthur, arthur_song@sfu.ca).

*/

#include "include/Angel.h"
#include "gameLogic.h"

using namespace std;
using namespace GT_gameLogic;

void init()
{
	// Load shaders and use the shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
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

	// Game initialization
	newTile(); // create new next tile

	// set to default
	glBindVertexArray(0);
	glClearColor(0, 0, 0, 0);
}

int main(int argc, char **argv)
{
	// OpenGL Utility ToolKit - Configure a window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(xsize, ysize);
	glutInitWindowPosition(680, 178); // Center the game window (well, on a 1920x1080 display)
	glutCreateWindow("Fruit Tetris");

	// Initialize GLEW
	if(glewInit()) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit(EXIT_FAILURE);
	}

	init();
  srand (time(NULL)); // initialize random seed

	// Callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	// TODO:	glutTimerFunc(800,)
	glutIdleFunc(idle);

	// Start main loop
	glutMainLoop();
	return 0;
}
