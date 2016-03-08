//
// CMPT 361 Assignment 2 - Fruit Tetris
// Implemented in Feb 2016 by Danyang Song (Arthur, arthur_song@sfu.ca).
// (Network ID: GreysTone)
//

#include "gameDrawing.h"

//using namespace GT_gameDrawing
//using namespace GT_gameModel::ARM;

namespace GT_gameDrawing {
  // location of vertex attributes in the shader program
  GLint vPosition;
  GLint vColor;

  // locations of uniform variables in shader program
  GLint locxsize;
  GLint locysize;
  GLint loczsize;

  // projection
  GLint locMVPMatrix;
  mat4 ModelMat = mat4();
  mat4 ViewMat = mat4();
  mat4 ProjectionMat = Perspective(GT_GLOBAL_PROJECT_ANGLE,
                                   (GLfloat)1.0 * GT_gameSetting::xsize / GT_gameSetting::ysize,
                                   GT_GLOBAL_PROJECT_Z_NEAR,
                                   GT_GLOBAL_PROJECT_Z_FAR);

  // projection setting
  vec4 projectionEye = vec4(0, GT_GLOBAL_HEIGHT_BOARD, GT_GLOBAL_EYE_Z, 0);
  vec4 projectionAt = vec4(0, GT_GLOBAL_HEIGHT_BOARD/2, 0, 0);
  vec4 projectionUp = vec4(0, 1, 0, 0);


  // TODO: [RESET] Projection [mat4]model_view & projection
  //  GLfloat angle1 = M_PI / 2;
//  GLfloat angle2 = M_PI / 4 ;
//  const GLfloat armLength1 = 420.0;
//  const GLfloat armLength2 = 400.0;
//  const GLfloat  dr = 5.0 * DegreesToRadians;
  mat4  model_view;  // model-view matrix uniform shader variable location
//
//  GLfloat  lleft = -1.0, rright = 1;
//  GLfloat  bottom = -1.0, top = 1;
//  GLfloat  zNear = 0.1, zFar = 5.0;
  mat4  projection; // projection matrix uniform shader variable location
  GLuint ModelView, Projection;

  // VAO and VBO
  // One VAO for each object: the grid, the board, the current piece
  GLuint vaoIDs[GT_GLOBAL_OBJECT_AMOUNT];
  // Two Vertex Buffer Objects for each VAO (specifying vertex positions and colours, respectively)
  GLuint vboIDs[GT_GLOBAL_OBJECT_AMOUNT * 2];

  //An array containing the colour of each of the 10*20*2*3 vertices that make up the board
  //Initially, all will be set to black. As tiles are placed, sets of 6 vertices (2 triangles; 1 square)
  //will be set to the appropriate colour in this array before updating the corresponding VBO
  vec4 boardcolours[GT_GLOBAL_VERTEX_BOARD];
} // namespace GT_gameDrawing

void
GT_gameDrawing::initOpenGL() {
  // Load shaders and use the shader program
#ifdef __APPLE__
  GLuint program = InitShader( // replace with a absolute path for two shader
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

  // Create 4 Vertex Array Objects, each representing one 'object'. Store the names in array vaoIDs
  glGenVertexArrays(GT_GLOBAL_OBJECT_AMOUNT, &vaoIDs[0]);

  // The location of the uniform variables in the shader program
  locxsize = glGetUniformLocation(program, "xsize");
  locysize = glGetUniformLocation(program, "ysize");
  loczsize = glGetUniformLocation(program, "zsize");
  locMVPMatrix = glGetUniformLocation(program, "MVP");
  //TODO: [RESET] Projection
//  model_view = glGetUniformLocation ( program, "model_view" );
//  projection = glGetUniformLocation( program, "projection" );

  // set to default
  glBindVertexArray(0);
  glClearColor(0, 0, 0, 0);

  glEnable(GL_DEPTH);
//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // anti-aliasing
#ifdef GT_SET_ANTI_ALIASING
  glEnable(GL_MULTISAMPLE);
  glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_POINT_SMOOTH);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
#endif
}

void
GT_gameDrawing::initGrid() {
  // ***Generate geometry data
  vec4 gridpoints[GT_GLOBAL_VERTEX_GRID]; // Array containing the 128 points of the 64 total lines to be later put in the VBO
  vec4 gridcolours[GT_GLOBAL_VERTEX_GRID]; // One colour per vertex

  // Vertical lines [+16.5]
  for (int i = 0; i < 11; i++){
    gridpoints[2*i] = vec4((GLfloat)(33.0 + (33.0 * i)), 33.0, (GLfloat)16.6, 1);
    gridpoints[2*i + 1] = vec4((GLfloat)(33.0 + (33.0 * i)), 693.0, (GLfloat)16.6, 1);
  }
  // Vertical lines [-16.5]
  for (int i = 0; i < 11; i++){
    gridpoints[22 + 2*i] = vec4((GLfloat)(33.0 + (33.0 * i)), 33.0, (GLfloat)-16.6, 1);
    gridpoints[22 + 2*i + 1] = vec4((GLfloat)(33.0 + (33.0 * i)), 693.0, (GLfloat)-16.6, 1);
  }
  // Horizontal lines [+16.5]
  for (int i = 0; i < 21; i++){
    gridpoints[44 + 2*i] = vec4(33.0, (GLfloat)(33.0 + (33.0 * i)), (GLfloat)16.6, 1);
    gridpoints[44 + 2*i + 1] = vec4(363.0, (GLfloat)(33.0 + (33.0 * i)), (GLfloat)16.6, 1);
  }
  // Horizontal lines [-16.5]
  for (int i = 0; i < 21; i++){
    gridpoints[86 + 2*i] = vec4(33.0, (GLfloat)(33.0 + (33.0 * i)), (GLfloat)-16.6, 1);
    gridpoints[86 + 2*i + 1] = vec4(363.0, (GLfloat)(33.0 + (33.0 * i)), (GLfloat)-16.6, 1);
  }
  // Depth lines
  for (int i = 0; i < GT_GLOBAL_HEIGHT_BOARD + 1; i++){
    for (int j = 0; j < GT_GLOBAL_WIDTH_BOARD + 1; j++) {
      gridpoints[128 + 22*i + 2*j] 		= vec4((GLfloat)33.0 + (GLfloat)(j * 33.0), (GLfloat)33.0 + (GLfloat)(i * 33.0), (GLfloat)16.6, 1);
      gridpoints[128 + 22*i + 2*j + 1] 	= vec4((GLfloat)33.0 + (GLfloat)(j * 33.0), (GLfloat)33.0 + (GLfloat)(i * 33.0), (GLfloat)-16.6, 1);
    }
  }

  // Make all grid lines white
  for (int i = 0; i < GT_GLOBAL_VERTEX_GRID; i++)
    gridcolours[i] = GT_gameSetting::palette[GT_gameSetting::white];


  // *** set up buffer objects
  // Set up first VAO (representing grid lines)
  glBindVertexArray(vaoIDs[GT_gameSetting::objGrid]); // Bind the first VAO
  // TODO: [ARS] If bind error, could inspect cuboid
  glGenBuffers(2, &vboIDs[GT_gameSetting::objGrid * 2]); // Create two Vertex Buffer Objects for this VAO (positions, colours)

  // Grid vertex positions
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[GT_gameSetting::objGrid * 2]); // Bind the first grid VBO (vertex positions)
  glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_GRID*sizeof(vec4), gridpoints, GL_STATIC_DRAW); // Put the grid points in the VBO
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vPosition); // Enable the attribute

  // Grid vertex colours
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[GT_gameSetting::objGrid * 2 + 1]); // Bind the second grid VBO (vertex colours)
  glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_GRID*sizeof(vec4), gridcolours, GL_STATIC_DRAW); // Put the grid colours in the VBO
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vColor); // Enable the attribute
}

void
GT_gameDrawing::initBoard() {
  // *** Generate the geometric data
  vec4 boardpoints[GT_GLOBAL_VERTEX_BOARD];
  for (int i = 0; i < GT_GLOBAL_VERTEX_BOARD; i++)
    boardcolours[i] = GT_gameSetting::palette[GT_gameSetting::black]; // Let the empty cells on the board be black
  // Each cell is a cube (12 triangles with 36 vertices)
  for (int i = 0; i < 20; i++){
    for (int j = 0; j < 10; j++)
    {
      // F(ront) B(ack) / T(op) B(ottom) / L(eft) R(ight)
      vec4 p1 = vec4((GLfloat)(33.0 + (j * 33.0)), (GLfloat)(33.0 + (i * 33.0)), (GLfloat)16.5, 1);   // FBL
      vec4 p2 = vec4((GLfloat)(33.0 + (j * 33.0)), (GLfloat)(66.0 + (i * 33.0)), (GLfloat)16.5, 1);   // FTL
      vec4 p3 = vec4((GLfloat)(66.0 + (j * 33.0)), (GLfloat)(33.0 + (i * 33.0)), (GLfloat)16.5, 1);   // FBR
      vec4 p4 = vec4((GLfloat)(66.0 + (j * 33.0)), (GLfloat)(66.0 + (i * 33.0)), (GLfloat)16.5, 1);   // FTR
      vec4 p5 = vec4((GLfloat)(33.0 + (j * 33.0)), (GLfloat)(33.0 + (i * 33.0)), (GLfloat)-16.5, 1);  // BBL
      vec4 p6 = vec4((GLfloat)(33.0 + (j * 33.0)), (GLfloat)(66.0 + (i * 33.0)), (GLfloat)-16.5, 1);  // BTL
      vec4 p7 = vec4((GLfloat)(66.0 + (j * 33.0)), (GLfloat)(33.0 + (i * 33.0)), (GLfloat)-16.5, 1);  // BBR
      vec4 p8 = vec4((GLfloat)(66.0 + (j * 33.0)), (GLfloat)(66.0 + (i * 33.0)), (GLfloat)-16.5, 1);  // BTR

      // Two points are used by two triangles each
      vec4 cube[36] = {
          p1, p2, p3, p2, p3, p4,     // Front
          p1, p2, p5, p2, p5, p6,     // Left
          p5, p6, p7, p6, p7, p8,     // Back
          p3, p4, p7, p4, p7, p8,     // Right
          p2, p4, p6, p4, p6, p8,     // Top
          p1, p3, p5, p3, p5, p7      // Bottom
      };
      for(int k=0; k<GT_GLOBAL_VERTEX_SINGLE_CUBE; k++)
        boardpoints[GT_GLOBAL_VERTEX_SINGLE_CUBE*(10*i + j) + k] = cube[k];
    }
  }

  // *** set up buffer objects
  glBindVertexArray(vaoIDs[GT_gameSetting::objBoard]);
  glGenBuffers(2, &vboIDs[GT_gameSetting::objBoard * 2]);

  // Grid cell vertex positions
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[GT_gameSetting::objBoard * 2]);
  glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_BOARD*sizeof(vec4), boardpoints, GL_STATIC_DRAW);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vPosition);

  // Grid cell vertex colours
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[GT_gameSetting::objBoard * 2 + 1]);
  glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_BOARD*sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vColor);
}

// No geometry for current tile initially
void
GT_gameDrawing::initCurrentTile() {
  glBindVertexArray(vaoIDs[GT_gameSetting::objTile]);
  glGenBuffers(2, &vboIDs[GT_gameSetting::objTile * 2]);

  // Current tile vertex positions
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[GT_gameSetting::objTile * 2]);
  glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_TILE*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vPosition);

  // Current tile vertex colours
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[GT_gameSetting::objTile * 2 + 1]);
  glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_TILE*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vColor);
}

void
GT_gameDrawing::initArm() {
//  using namespace GT_gameModel::ARM;
//  setupModel();

  // Create a vertex array object
//  GLuint vao;
//  glGenVertexArraysAPPLE( 1, &vao );
//  glBindVertexArrayAPPLE( vao );
//  glBindVertexArray(vaoIDs[GT_gameSetting::objArm]);
//  glGenBuffers(2, &vboIDs[GT_gameSetting::objArm * 2]);

//  // Grid cell vertex positions
//  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[GT_gameSetting::objArm * 2]);
//  glBufferData(GL_ARRAY_BUFFER, 576, GT_gameModel::ARM::vertexArray, GL_STATIC_DRAW);
//  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
//  glEnableVertexAttribArray(vPosition);
//
//  // Grid cell vertex colours
//  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[GT_gameSetting::objArm * 2 + 1]);
//  glBufferData(GL_ARRAY_BUFFER, 576, GT_gameModel::ARM::colourArray, GL_DYNAMIC_DRAW);
//  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
//  glEnableVertexAttribArray(vColor);

  // Create and initialize a buffer object
//  GLuint buffer;
//  glGenBuffers( 1, &buffer );
//  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[GT_gameSetting::objArm * 2]);
////  glBindBuffer( GL_ARRAY_BUFFER, buffer );
//  glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
//                NULL, GL_DYNAMIC_DRAW );
//  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
//  glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );
}

// When the current tile is moved or rotated (or created), update the VBO containing its vertex position data
void
GT_gameDrawing::updateTile() {
  // Bind the VBO containing current tile vertex positions
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

#ifdef GT_DEBUG_TILE_POSITION_ONLINE
  cout << "CUR_POS on updateTile()\n\t";
#endif
  // For each of the 4 'cells' of the tile,
  for (int i = 0; i < 4; i++)
  {
    // Calculate the grid coordinates of the cell
    GLfloat x = GT_gameSetting::tilepos.x + GT_gameSetting::tile[i].x;
    GLfloat y = GT_gameSetting::tilepos.y + GT_gameSetting::tile[i].y;

#ifdef GT_DEBUG_TILE_POSITION_ONLINE
    cout << "[" << i << "] X:" << x << " - Y:" << y << " | ";
#endif
    // Create the 4 corners of the square - these vertices are using location in pixels
    // These vertices are later converted by the vertex shader
    vec4 p1 = vec4((GLfloat)(33.0 + (x * 33.0)), (GLfloat)(33.0 + (y * 33.0)), (GLfloat)16.5, 1);   // FBL
    vec4 p2 = vec4((GLfloat)(33.0 + (x * 33.0)), (GLfloat)(66.0 + (y * 33.0)), (GLfloat)16.5, 1);   // FTL
    vec4 p3 = vec4((GLfloat)(66.0 + (x * 33.0)), (GLfloat)(33.0 + (y * 33.0)), (GLfloat)16.5, 1);   // FBR
    vec4 p4 = vec4((GLfloat)(66.0 + (x * 33.0)), (GLfloat)(66.0 + (y * 33.0)), (GLfloat)16.5, 1);   // FTR
    vec4 p5 = vec4((GLfloat)(33.0 + (x * 33.0)), (GLfloat)(33.0 + (y * 33.0)), (GLfloat)-16.5, 1);  // BBL
    vec4 p6 = vec4((GLfloat)(33.0 + (x * 33.0)), (GLfloat)(66.0 + (y * 33.0)), (GLfloat)-16.5, 1);  // BTL
    vec4 p7 = vec4((GLfloat)(66.0 + (x * 33.0)), (GLfloat)(33.0 + (y * 33.0)), (GLfloat)-16.5, 1);  // BBR
    vec4 p8 = vec4((GLfloat)(66.0 + (x * 33.0)), (GLfloat)(66.0 + (y * 33.0)), (GLfloat)-16.5, 1);  // BTR


    // Two points are used by two triangles each
    vec4 newpoints[36] = {
        p1, p2, p3, p2, p3, p4,     // Front
        p1, p2, p5, p2, p5, p6,     // Left
        p5, p6, p7, p6, p7, p8,     // Back
        p3, p4, p7, p4, p7, p8,     // Right
        p2, p4, p6, p4, p6, p8,     // Top
        p1, p3, p5, p3, p5, p7      // Bottom
    };

    // Put new data in the VBO
    glBufferSubData(GL_ARRAY_BUFFER, i*36*sizeof(vec4), 36*sizeof(vec4), newpoints);
  }
#ifdef GT_DEBUG_TILE_POSITION_ONLINE
  cout << endl;
#endif

  glBindVertexArray(0);
}

void
GT_gameDrawing::updateBoardColor(int x, int y, vec4 c) {
//    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
//    cout << "updateBoardColor on x:" << x << " y:" << y << endl;
  for(int i=0; i<36; i++)
    boardcolours[36*(10*y + x) + i] = c;
//    boardcolours[6*(10*y + x)    ] = c;
//    boardcolours[6*(10*y + x) + 1] = c;
//    boardcolours[6*(10*y + x) + 2] = c;
//    boardcolours[6*(10*y + x) + 3] = c;
//    boardcolours[6*(10*y + x) + 4] = c;
//    boardcolours[6*(10*y + x) + 5] = c;
//    glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_BOARD*sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);

}

// Reshape callback will simply change xsize and ysize variables, which are passed to the vertex shader
// to keep the game the same from stretching if the window is stretched
void
GT_gameDrawing::reshape(GLsizei w, GLsizei h) {
  GT_gameSetting::xsize = w;
  GT_gameSetting::ysize = h;
  glViewport(0, 0, w, h);
}

// Draws the game
void
GT_gameDrawing::display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ModelMat = mat4();  // initialize with Identity Matrix
  ModelMat *= Translate(0, GT_GLOBAL_HEIGHT_BOARD/2, 0);
  ModelMat *= Scale((GLfloat)1.0/33, (GLfloat)1.0/33, (GLfloat)1.0/33);
  mat4 MVPMat =  ProjectionMat *  ModelMat * ViewMat;

  glUniform1i(locxsize, GT_gameSetting::xsize); // x and y sizes are passed to the shader program to maintain shape of the vertices on screen
  glUniform1i(locysize, GT_gameSetting::ysize);
  glUniform1i(loczsize, GT_gameSetting::zsize);
  glUniformMatrix4fv(locMVPMatrix, 1, GL_TRUE, MVPMat);

  glBindVertexArray(vaoIDs[GT_gameSetting::objBoard]); // Bind the VAO representing the grid cells (to be drawn first)
  glDrawArrays(GL_TRIANGLES, 0, GT_GLOBAL_VERTEX_BOARD); // Draw the board (10*20*2 = 400 triangles)

  glBindVertexArray(vaoIDs[GT_gameSetting::objTile]); // Bind the VAO representing the current tile (to be drawn on top of the board)
  glDrawArrays(GL_TRIANGLES, 0, GT_GLOBAL_VERTEX_TILE); // Draw the current tile (8 triangles)

  glBindVertexArray(vaoIDs[GT_gameSetting::objGrid]); // Bind the VAO representing the grid lines (to be drawn on top of everything else)
  glDrawArrays(GL_LINES, 0, GT_GLOBAL_VERTEX_GRID); // Draw the grid lines (21+11 = 32 lines)

//  glBindVertexArray(vaoIDs[armVAO]); // Bind the VAO representing the current tile (to be drawn on top of the board)
//  glDrawArrays(GL_TRIANGLES, 0, allPoints); // Draw the current tile (8 triangles)



  glutSwapBuffers();
}

//-------------------------------------------------------------------------------------------------------------------

void
GT_gameDrawing::idle() {
  glutPostRedisplay();
}