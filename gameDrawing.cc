//
// CMPT 361 Assignment 2 - Fruit Tetris
// Implemented in Feb 2016 by Danyang Song (Arthur, arthur_song@sfu.ca).
// (Network ID: GreysTone)
//

#include "gameDrawing.h"
#include "gameLogic.h"
//using namespace GT_gameSetting;

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

  vec4 armpoints[GT_GLOBAL_VERTEX_ARM];
  vec4 armcolorus[GT_GLOBAL_VERTEX_ARM];



void setText(char* text)
{
  glDisable(GL_TEXTURE_2D);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0.0, xsize, 0.0, ysize);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glRasterPos2i(100, 600);
  void * font = GLUT_BITMAP_8_BY_13;

  int i = 0;
  while(text[i]) {
    glColor3d(1.0, 1.0, 1.0);
    glutBitmapCharacter(font, text[i]);
    i++;
  }
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glEnable(GL_TEXTURE_2D);
}



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
//  vec4 *gridVertex = NULL;
//  vec4 *gridColour = NULL;
  int n = GT_gameSetting::superPower;
  GT_gameSetting::gridPointCount = 64*(n+1)+(11*21)*2;
  gridVertex = new vec4[gridPointCount];
  gridColour = new vec4[gridPointCount];

  for(int k = 0; k < n+1; k++) {
    // vertical lines
    for(int i=0; i < 11; i++) {
      gridVertex[64*k+2*i] = vec4((GLfloat)(33.0 + (33.0 * i)), 33.0, 33.0*k, 1);
      gridVertex[64*k+2*i+1] = vec4((GLfloat)(33.0 + (33.0 * i)), 693.0, 33.0*k, 1);
    }
    // horizontal lines
    for (int i = 0; i < 21; i++){
      gridVertex[64*k+22+2*i] = vec4(33.0, (GLfloat)(33.0 + (33.0 * i)), 33.0*k, 1);
      gridVertex[64*k+22+2*i+1] = vec4(363.0, (GLfloat)(33.0 + (33.0 * i)), 33.0*k, 1);
    }
  }
  // depth lines
  for (int i = 0; i < GT_GLOBAL_HEIGHT_BOARD + 1; i++){
    for (int j = 0; j < GT_GLOBAL_WIDTH_BOARD + 1; j++) {
      gridVertex[64*(n+1) + 22*i + 2*j] 		= vec4((GLfloat)33.0 + (GLfloat)(j * 33.0), (GLfloat)33.0 + (GLfloat)(i * 33.0), (GLfloat)0.0, 1);
      gridVertex[64*(n+1) + 22*i + 2*j + 1] 	= vec4((GLfloat)33.0 + (GLfloat)(j * 33.0), (GLfloat)33.0 + (GLfloat)(i * 33.0), (GLfloat)33.0*n, 1);
    }
  }

  // Colour
  for (int i = 0; i < gridPointCount; i++)
    gridColour[i] = GT_gameSetting::palette[GT_gameSetting::white];

//  gtPipeCreate(objGrid, gridVertex, gridColour);
  glBindVertexArray(vaoIDs[objGrid]); // Bind the first VAO
  // TODO: [ARS] If bind error, could inspect cuboid
  glGenBuffers(2, &vboIDs[objGrid * 2]); // Create two Vertex Buffer Objects for this VAO (positions, colours)

  // Grid vertex positions
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[objGrid * 2]); // Bind the first grid VBO (vertex positions)
  glBufferData(GL_ARRAY_BUFFER,  GT_gameSetting::gridPointCount*sizeof(vec4), gridVertex, GL_STATIC_DRAW); // Put the grid points in the VBO
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vPosition); // Enable the attribute

  // Grid vertex colours
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[objGrid * 2 + 1]); // Bind the second grid VBO (vertex colours)
  glBufferData(GL_ARRAY_BUFFER,  GT_gameSetting::gridPointCount*sizeof(vec4), gridColour, GL_STATIC_DRAW); // Put the grid colours in the VBO
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vColor); // Enable the attribute
}

void
GT_gameDrawing::initBoard() {
//  GT_gameModel::BOARD::setupModel();
//  gtPipeCreate(objBoard, GT_gameModel::BOARD::vertexArray, GT_gameModel::BOARD::colourArray);

  int n = GT_gameSetting::superPower;

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

void
GT_gameDrawing::initCurrentTile() {
//  gtPipeCreate(objTile, NULL, NULL);
  //   *** set up buffer objects
  // Set up first VAO (representing grid lines)
  glBindVertexArray(vaoIDs[objTile]); // Bind the first VAO
  // TODO: [ARS] If bind error, could inspect cuboid
  glGenBuffers(2, &vboIDs[objTile * 2]); // Create two Vertex Buffer Objects for this VAO (positions, colours)

  // Grid vertex positions
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[objTile * 2]); // Bind the first grid VBO (vertex positions)
  glBufferData(GL_ARRAY_BUFFER, 24*6*sizeof(vec4), NULL, GL_STATIC_DRAW); // Put the grid points in the VBO
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vPosition); // Enable the attribute

  // Grid vertex colours
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[objTile * 2 + 1]); // Bind the second grid VBO (vertex colours)
  glBufferData(GL_ARRAY_BUFFER, 24*6*sizeof(vec4), NULL, GL_STATIC_DRAW); // Put the grid colours in the VBO
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vColor); // Enable the attribute
}


void
GT_gameDrawing::updateArm(int Theta, int Phi) {
//  if(curTheta+Theta < 135 || curTheta+Theta > 225) return;
//  if(curPhi+Phi > 45 || curTheta+Theta < -45) return;

  vec4 p[8], q[8];
  float unit = -16.5;

    GT_gameLogic::Theta = (GT_gameLogic::Theta + Theta) % 360;
    for(int i=0; i<8; i++) {
      p[i] = vec4(((i & 4) ? -unit : unit), ((i & 2) ? -unit : unit), ((i & 1) ? -unit : unit), 1);
    }
    for(int i=0; i<8; i++) {
      p[i] = Scale(1, 8, 1) * p[i];
      p[i] = Translate(0, 4*33, 0) * p[i];
      p[i] = RotateZ(GT_gameLogic::Theta) * p[i];
//      cout << "rotate: Theta:" << (GT_gameLogic::Theta) << " sin:" << sin(DegreesToRadians * GT_gameLogic::Theta) << endl;
      p[i] = Translate(-80, 33, 0) * p[i];
    }

    vec4 cube2[36] = {
        p[0], p[1], p[2], p[1], p[2], p[3],     // Front
        p[0], p[1], p[4], p[1], p[4], p[5],     // Left
        p[4], p[5], p[6], p[5], p[6], p[7],     // Back
        p[2], p[3], p[6], p[3], p[6], p[7],     // Right
        p[1], p[3], p[5], p[3], p[5], p[7],     // Top
        p[0], p[2], p[4], p[2], p[4], p[6]      // Bottom
    };
    for(int k=0; k<GT_GLOBAL_VERTEX_SINGLE_CUBE; k++)
      armpoints[36+k] = cube2[k];

    // JOINT Higher Arm
    GT_gameLogic::Phi = (GT_gameLogic::Phi + Phi) % 360;
    for(int i=0; i<8; i++) {
      q[i] = vec4(((i & 4) ? -unit : unit), ((i & 2) ? -unit : unit), ((i & 1) ? -unit : unit), 1);
    }
    for(int i=0; i<8; i++) {
      q[i] = Scale(1, 16, 1) * q[i];
      q[i] = Translate(0, 8*33, 0) * q[i];
      q[i] = RotateZ(GT_gameLogic::Phi) * q[i];
      q[i] = Translate(-8*33*sin(DegreesToRadians * GT_gameLogic::Theta),8*33*cos(DegreesToRadians * GT_gameLogic::Theta),0) * q[i];
//      cout << "rotate: Phi:" << (GT_gameLogic::Phi) << " sin:" << sin(DegreesToRadians * GT_gameLogic::Phi) << endl;
      q[i] = Translate(-80, 33, 0) * q[i];
    }
//  cout << "POSPOS0: " << q[5] << endl;
//  cout << "POSPOS7: " << q[7] << endl;  //TODO: q[7] is the chosen one


    vec4 cube3[36] = {
        q[0], q[1], q[2], q[1], q[2], q[3],     // Front
        q[0], q[1], q[4], q[1], q[4], q[5],     // Left
        q[4], q[5], q[6], q[5], q[6], q[7],     // Back
        q[2], q[3], q[6], q[3], q[6], q[7],     // Right
        q[1], q[3], q[5], q[3], q[5], q[7],     // Toq
        q[0], q[2], q[4], q[2], q[4], q[6]      // Bottom
    };
    for(int k=0; k<GT_GLOBAL_VERTEX_SINGLE_CUBE; k++)
      armpoints[72+k] = cube3[k];

  // Make all grid lines white
  for (int i = 0; i < 36; i++) //Desktop
    armcolorus[i] = GT_gameSetting::palette[GT_gameSetting::purple];

  for (int i = 36; i < 72; i++) //Arm 1
    armcolorus[i] = GT_gameSetting::palette[GT_gameSetting::red];;

  for (int i = 72; i < 108; i++) //Arm 2
    armcolorus[i] = GT_gameSetting::palette[GT_gameSetting::green];

  // Grid cell vertex positions
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[GT_gameSetting::objArm * 2]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, 108*sizeof(vec4), armpoints);

  //TODO: <POS>
//  cout << "X:" << (int)(q[7].x/33)-1 << " - Y:" << (int)(q[7].y/33)-1 << endl;
  GT_gameSetting::tilepos.x = (int)(q[7].x/33)-1;
  GT_gameSetting::tilepos.y = (int)(q[7].y/33)-1;
  GT_gameDrawing::updateTile();
    
}
//}

void
GT_gameDrawing::initArm() {

  // Build Model - Base
  float unit = 16.5;
  vec4 p[8] = {
      vec4( unit, unit, unit, 1),
      vec4( unit, unit,-unit, 1),
      vec4( unit,-unit, unit, 1),
      vec4( unit,-unit,-unit, 1),
      vec4(-unit, unit, unit, 1),
      vec4(-unit, unit,-unit, 1),
      vec4(-unit,-unit, unit, 1),
      vec4(-unit,-unit,-unit, 1)
  };
  for(int i=0; i<8; i++) {
    p[i] = Scale(5, 1, 4) * p[i];
    p[i] = Translate(-80, 33, 0) * p[i];
  }
  vec4 cube1[36] = {
      p[0], p[1], p[2], p[1], p[2], p[3],     // Front
      p[0], p[1], p[4], p[1], p[4], p[5],     // Left
      p[4], p[5], p[6], p[5], p[6], p[7],     // Back
      p[2], p[3], p[6], p[3], p[6], p[7],     // Right
      p[1], p[3], p[5], p[3], p[5], p[7],     // Top
      p[0], p[2], p[4], p[2], p[4], p[6]      // Bottom
  };
  for(int k=0; k<GT_GLOBAL_VERTEX_SINGLE_CUBE; k++)
    armpoints[k] = cube1[k];


  // Build Model - Lower Arm
  for(int i=0; i<8; i++) {
    p[i] = vec4(((i&4)?-unit:unit), ((i&2)?-unit:unit), ((i&1)?-unit:unit),1);
  }
  for(int i=0; i<8; i++) {
    p[i] = Scale(1, 8, 1) * p[i];
    p[i] = Translate(0, 4*33, 0) * p[i];
    p[i] = RotateZ(GT_gameLogic::Theta) * p[i];
//      cout << "rotate: Theta:" << (GT_gameLogic::Theta) << " sin:" << sin(DegreesToRadians * GT_gameLogic::Theta) << endl;
    p[i] = Translate(-80, 33, 0) * p[i];
  }
  vec4 cube2[36] = {
      p[0], p[1], p[2], p[1], p[2], p[3],     // Front
      p[0], p[1], p[4], p[1], p[4], p[5],     // Left
      p[4], p[5], p[6], p[5], p[6], p[7],     // Back
      p[2], p[3], p[6], p[3], p[6], p[7],     // Right
      p[1], p[3], p[5], p[3], p[5], p[7],     // Top
      p[0], p[2], p[4], p[2], p[4], p[6]      // Bottom
  };
  for(int k=0; k<GT_GLOBAL_VERTEX_SINGLE_CUBE; k++)
    armpoints[36+k] = cube2[k];

  // Build Model - Higher Arm
  for(int i=0; i<8; i++) {
    p[i] = vec4(((i&4)?-unit:unit), ((i&2)?-unit:unit), ((i&1)?-unit:unit),1);
  }
  for(int i=0; i<8; i++) {
    p[i] = Scale(1, 16, 1) * p[i];
    p[i] = Translate(0, 8*33, 0) * p[i];
    p[i] = RotateZ(GT_gameLogic::Phi) * p[i];
    p[i] = Translate(-8*33*sin(DegreesToRadians * GT_gameLogic::Theta),8*33*cos(DegreesToRadians * GT_gameLogic::Theta),0) * p[i];
    p[i] = Translate(-80, 33, 0) * p[i];
  }
  vec4 cube3[36] = {
      p[0], p[1], p[2], p[1], p[2], p[3],     // Front
      p[0], p[1], p[4], p[1], p[4], p[5],     // Left
      p[4], p[5], p[6], p[5], p[6], p[7],     // Back
      p[2], p[3], p[6], p[3], p[6], p[7],     // Right
      p[1], p[3], p[5], p[3], p[5], p[7],     // Top
      p[0], p[2], p[4], p[2], p[4], p[6]      // Bottom
  };
  for(int k=0; k<GT_GLOBAL_VERTEX_SINGLE_CUBE; k++)
    armpoints[72+k] = cube3[k];


  // Make all grid lines white
  for (int i = 0; i < 36; i++) //Desktop
    armcolorus[i] = GT_gameSetting::palette[GT_gameSetting::purple];

  for (int i = 36; i < 72; i++) //Arm 1
    armcolorus[i] = GT_gameSetting::palette[GT_gameSetting::red];

  for (int i = 72; i < 108; i++) //Arm 2
    armcolorus[i] = GT_gameSetting::palette[GT_gameSetting::green];

//   *** set up buffer objects
  // Set up first VAO (representing grid lines)
  glBindVertexArray(vaoIDs[objArm]); // Bind the first VAO
  // TODO: [ARS] If bind error, could inspect cuboid
  glGenBuffers(2, &vboIDs[objArm * 2]); // Create two Vertex Buffer Objects for this VAO (positions, colours)

  // Grid vertex positions
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[objArm * 2]); // Bind the first grid VBO (vertex positions)
  glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_ARM*sizeof(vec4), armpoints, GL_STATIC_DRAW); // Put the grid points in the VBO
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vPosition); // Enable the attribute

  // Grid vertex colours
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[objArm * 2 + 1]); // Bind the second grid VBO (vertex colours)
  glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_ARM*sizeof(vec4), armcolorus, GL_STATIC_DRAW); // Put the grid colours in the VBO
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vColor); // Enable the attribute



//  gtPipeCreate(objArm, armpoints, armcolorus);

//  GT_gameModel::ARM::setupModel();
//  gtPipeCreate(GT_gameSetting::objArm, GT_gameModel::ARM::vertexArray, GT_gameModel::ARM::colourArray);

  GT_gameLogic::newTile();
  GT_gameSetting::tilepos.x = (int)(p[7].x/33)-1;
  GT_gameSetting::tilepos.y = (int)(p[7].y/33)-1;
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
    vec4 p1 = vec4((GLfloat)(33.0 + (x * 33.0)), (GLfloat)(33.0 + (y * 33.0)), (GLfloat)0.0, 1);   // FBL
    vec4 p2 = vec4((GLfloat)(33.0 + (x * 33.0)), (GLfloat)(66.0 + (y * 33.0)), (GLfloat)0.0, 1);   // FTL
    vec4 p3 = vec4((GLfloat)(66.0 + (x * 33.0)), (GLfloat)(33.0 + (y * 33.0)), (GLfloat)0.0, 1);   // FBR
    vec4 p4 = vec4((GLfloat)(66.0 + (x * 33.0)), (GLfloat)(66.0 + (y * 33.0)), (GLfloat)0.0, 1);   // FTR
    vec4 p5 = vec4((GLfloat)(33.0 + (x * 33.0)), (GLfloat)(33.0 + (y * 33.0)), (GLfloat)33.0, 1);  // BBL
    vec4 p6 = vec4((GLfloat)(33.0 + (x * 33.0)), (GLfloat)(66.0 + (y * 33.0)), (GLfloat)33.0, 1);  // BTL
    vec4 p7 = vec4((GLfloat)(66.0 + (x * 33.0)), (GLfloat)(33.0 + (y * 33.0)), (GLfloat)33.0, 1);  // BBR
    vec4 p8 = vec4((GLfloat)(66.0 + (x * 33.0)), (GLfloat)(66.0 + (y * 33.0)), (GLfloat)33.0, 1);  // BTR


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

  // Color reshade
  bool outRange = false;
  for(int i = 0; i < 4; i++) {
    GLfloat x = GT_gameSetting::tilepos.x + GT_gameSetting::tile[i].x;
    GLfloat y = GT_gameSetting::tilepos.y + GT_gameSetting::tile[i].y;
    if((int)x < 0 || (int)x > 9 || (int)y < 0 || (int)y > 19) {
      outRange = true;
      break;
    }
  }
  vec4 newcolour[GT_GLOBAL_VERTEX_TILE];
  if(outRange || GT_gameLogic::collisionDetect(GT_gameLogic::CTN)) {
    for(int i=0; i<GT_GLOBAL_VERTEX_TILE; i++)
      newcolour[i] = palette[GT_gameSetting::grey];
  } else {
    int flag = 0;
    for (int i = 0; i < GT_GLOBAL_VERTEX_TILE; i += 36) {
      // vec4 tiled = palette[dist(generator)]; // randomize the color
      vec4 tiled = palette[tiledColor[flag]]; // randomize the color
      for (int j = 0; j < 36; j++)
        newcolour[i + j] = tiled;
    }
  }
  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolour), newcolour);

#ifdef GT_DEBUG_TILE_POSITION_ONLINE
  cout << endl;
#endif

//  glBindVertexArray(0);
}

void
GT_gameDrawing::updateBoardColor(int x, int y, vec4 c) {
  //TODO: connect Bind Buffer
//  std::cout << "update Board" << c << endl;
//    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[objBoard*2+1]);
  for(int i=0; i<36; i++)
    boardcolours[36*(10*y + x) + i] = c;

  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[GT_gameSetting::objBoard * 2 + 1]);
  glBufferSubData(GL_ARRAY_BUFFER, 0, GT_GLOBAL_VERTEX_BOARD*sizeof(vec4), boardcolours);

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

  gtPipeDraw(objBoard, GL_TRIANGLES, 0, GT_GLOBAL_VERTEX_BOARD);
  gtPipeDraw(objTile, GL_TRIANGLES, 0, GT_GLOBAL_VERTEX_TILE);
  gtPipeDraw(objGrid, GL_LINES, 0, GT_gameSetting::gridPointCount);
  gtPipeDraw(objArm, GL_TRIANGLES, 0, GT_GLOBAL_VERTEX_ARM);

  char text[10] = "Timer: 00";
  switch (GT_gameLogic::countDown) {
    case 10:
      text[7] = '1';
      text[8] = '0';
      GT_gameDrawing::setText(text);
      break;
    case 9:
      text[7] = '0';
      text[8] = '9';
      GT_gameDrawing::setText(text);
      break;
    case 8:
      text[7] = '0';
      text[8] = '8';
      GT_gameDrawing::setText(text);
      break;
    case 7:
      text[7] = '0';
      text[8] = '7';
      GT_gameDrawing::setText(text);
      break;
    case 6:
      text[7] = '0';
      text[8] = '6';
      GT_gameDrawing::setText(text);
      break;
    case 5:
      text[7] = '0';
      text[8] = '5';
      GT_gameDrawing::setText(text);
      break;
    case 4:
      text[7] = '0';
      text[8] = '4';
      GT_gameDrawing::setText(text);
      break;
    case 3:
      text[7] = '0';
      text[8] = '3';
      GT_gameDrawing::setText(text);
      break;
    case 2:
      text[7] = '0';
      text[8] = '2';
      GT_gameDrawing::setText(text);
      break;
    case 1:
      text[7] = '0';
      text[8] = '1';
      GT_gameDrawing::setText(text);
      break;
    case 0:
      text[7] = '0';
      text[8] = '0';
      GT_gameDrawing::setText(text);
      break;
  }

  glutSwapBuffers();
}

void
GT_gameDrawing::idle() {
  glutPostRedisplay();
}

//void
//GT_gameDrawing::gtPipeCreate(GT_gameSetting::gtObject object, vec4 *vertexArray, vec4 *colourArrary) {
//  // *** set up buffer objects
//  // Set up first VAO (representing grid lines)
//  glBindVertexArray(vaoIDs[object]); // Bind the first VAO
//  // TODO: [ARS] If bind error, could inspect cuboid
//  glGenBuffers(2, &vboIDs[object * 2]); // Create two Vertex Buffer Objects for this VAO (positions, colours)
//
//  // Grid vertex positions
//  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[object * 2]); // Bind the first grid VBO (vertex positions)
//  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW); // Put the grid points in the VBO
//  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
//  glEnableVertexAttribArray(vPosition); // Enable the attribute
//
//  // Grid vertex colours
//  glBindBuffer(GL_ARRAY_BUFFER, vboIDs[object * 2 + 1]); // Bind the second grid VBO (vertex colours)
//  glBufferData(GL_ARRAY_BUFFER, sizeof(colourArrary), colourArrary, GL_STATIC_DRAW); // Put the grid colours in the VBO
//  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
//  glEnableVertexAttribArray(vColor); // Enable the attribute
//}

void
GT_gameDrawing::gtPipeDraw(GT_gameSetting::gtObject object, GLenum mode, GLint first, GLsizei count) {
  glBindVertexArray(vaoIDs[object]);
  glDrawArrays(mode, first, count);
}

