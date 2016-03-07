//
// Created by Danyang Song on 10/02/16.
//

#include "gameLogic.h"

namespace GT_gameLogic {

  // current tile
  vec2 tile[4]; // An array of 4 2d vectors representing displacement from a 'center' piece of the tile, on the grid
  vec2 tilepos = vec2(5, 19); // The position of the current tile using grid coordinates ((0,0) is the bottom left corner)
  gtColor tiledColor[4];
  gtShape tileShape = sI;
  int tileModule = 0;

  //board[x][y] represents whether the cell (x,y) is occupied
  bool board[10][20];

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
                                   (GLfloat)1.0*xsize/ysize,
                                   GT_GLOBAL_PROJECT_Z_NEAR,
                                   GT_GLOBAL_PROJECT_Z_FAR);


  // VAO and VBO
  GLuint vaoIDs[3]; // One VAO for each object: the grid, the board, the current piece
  GLuint vboIDs[6]; // Two Vertex Buffer Objects for each VAO (specifying vertex positions and colours, respectively)

  // random generator
  //  std::default_random_engine generator;
  //  std::uniform_int_distribution<int> dist(0,4);

  // Searching Temporary Data
  bool removingMatrix[10][20];
  bool isRemovingMatrixEmpty;

  bool gamePause = false;

//  GLfloat angle1 = M_PI / 2;
//  GLfloat angle2 = M_PI / 4 ;
//  const GLfloat armLength1 = 420.0;
//  const GLfloat armLength2 = 400.0;
//  const GLfloat  dr = 5.0 * DegreesToRadians;
//  GLuint  model_view;  // model-view matrix uniform shader variable location
//
//  GLfloat  lleft = -1.0, rright = 1;
//  GLfloat  bottom = -1.0, top = 1;
//  GLfloat  zNear = 0.1, zFar = 5.0;
//  GLuint  projection; // projection matrix uniform shader variable location

//-------------------------------------------------------------------------------------------------------------------

  void initGrid()
  {
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
      gridcolours[i] = palette[white];


    // *** set up buffer objects
    // Set up first VAO (representing grid lines)
    glBindVertexArray(vaoIDs[0]); // Bind the first VAO
    glGenBuffers(2, vboIDs); // Create two Vertex Buffer Objects for this VAO (positions, colours)

    // Grid vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]); // Bind the first grid VBO (vertex positions)
    glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_GRID*sizeof(vec4), gridpoints, GL_STATIC_DRAW); // Put the grid points in the VBO
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition); // Enable the attribute

    // Grid vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]); // Bind the second grid VBO (vertex colours)
    glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_GRID*sizeof(vec4), gridcolours, GL_STATIC_DRAW); // Put the grid colours in the VBO
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColor); // Enable the attribute
  }

  void initBoard()
  {
    // *** Generate the geometric data
    vec4 boardpoints[GT_GLOBAL_VERTEX_BOARD];
    for (int i = 0; i < GT_GLOBAL_VERTEX_BOARD; i++)
      boardcolours[i] = palette[black]; // Let the empty cells on the board be black
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

    // Initially no cell is occupied
    for (int i = 0; i < 10; i++)
      for (int j = 0; j < 20; j++)
        board[i][j] = false;


    // *** set up buffer objects
    glBindVertexArray(vaoIDs[1]);
    glGenBuffers(2, &vboIDs[2]);

    // Grid cell vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
    glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_BOARD*sizeof(vec4), boardpoints, GL_STATIC_DRAW);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);

    // Grid cell vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
    glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_BOARD*sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColor);
  }

  // No geometry for current tile initially
  void initCurrentTile()
  {
    glBindVertexArray(vaoIDs[2]);
    glGenBuffers(2, &vboIDs[4]);

    // Current tile vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
    glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_TILE*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);

    // Current tile vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
    glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_TILE*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColor);
  }

//-------------------------------------------------------------------------------------------------------------------

  // Called at the start of play and every time a tile is placed
  void
  newTile() {
    tileShape = gtShape(rand() % 4);

    // Update the geometry VBO of current tile
    switch (tileShape) {
      case sI: {
        tileModule = rand() % 2;
        for (int i = 0; i < 4; i++)
          tile[i] = allRotationsIShape[tileModule][i]; // Get the 4 pieces of the new tile
        break;
      }
      case sS: {
        tileModule = rand() % 2;
        for (int i = 0; i < 4; i++)
          tile[i] = allRotationsSShape[tileModule][i]; // Get the 4 pieces of the new tile
        break;
      }
      case sL: {
        tileModule = rand() % 4;
        for (int i = 0; i < 4; i++)
          tile[i] = allRotationsLShape[tileModule][i]; // Get the 4 pieces of the new tile
        break;
      }
      case sT: {
        tileModule = rand() % 4;
        for (int i = 0; i < 4; i++)
          tile[i] = allRotationsTShape[tileModule][i]; // Get the 4 pieces of the new tile
        break;
      }
    }

    // calculate the range of the tile
    GLfloat xmax = tile[0].x;
    GLfloat ymax = tile[0].y;
    GLfloat xmin = tile[0].x;
    for (int i = 1; i < 4; i++) {
      if (tile[i].x > xmax) xmax = tile[i].x;
      if (tile[i].y > ymax) ymax = tile[i].y;
      if (tile[i].x < xmin) xmin = tile[i].x;
    }

    // randomize the position
    GLfloat x = (rand() % (int)(10 - xmax + xmin)) - xmin;
    GLfloat y = 19 - ymax;
    tilepos = vec2(x , y);


    // check if there is possible to contain a new tile
    if(collisionDetect(CTN)) {
      int col;
      for(col = 0; col < 10; col++) {
        int allIn = 0;
        for(int i = 0; i < 4; i++) {
          int nxtPosX = col + (int)tile[i].x;
          int nxtPosY = (int)tilepos.y + (int)tile[i].y;
          if(nxtPosX > 0 && nxtPosX < 10 && nxtPosY > 0 && nxtPosY < 20 && !board[nxtPosX][nxtPosY]) allIn++;
        }
        if(allIn == 4) break;
      }
      if(col == 10) { // Game Over
        char input;
        cout << "Game Over!\nType 'r' to restart, 'q' to quit.\n";
        while(cin >> input) {
          if(input == 'r') break;
          if(input == 'q') {
            exit (EXIT_SUCCESS);
          }
        }
        restart();
      }
      else tilepos = vec2(col, y);
    }

    updateTile();
    //TODO: update colours here

    // Update the color VBO of current tile
    vec4 newcolours[GT_GLOBAL_VERTEX_TILE];
    int flag = 0;
    for (int i = 0; i < GT_GLOBAL_VERTEX_TILE; i += 36) {
      // vec4 tiled = palette[dist(generator)]; // randomize the color
      tiledColor[flag] = (gtColor)((rand() % 32765) % 5); // (rand()%32765) % 5 -> makes color separated
#ifdef GT_DEBUG_SOLID_COLOR
      tiledColor[flag] = red;
#endif
      vec4 tiled = palette[tiledColor[flag]]; // randomize the color
      flag++;
      for (int j = 0; j < 36; j++)
        newcolours[i + j] = tiled;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]); // Bind the VBO containing current tile vertex colours
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours); // Put the colour data in the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
  }

  // Rotates the current tile, if there is room
  void
  rotate() {
    // Update the geometry VBO of current tile
    switch (tileShape) {
      case sI: {
        tileModule = (tileModule + 1) % 2;
        for (int i = 0; i < 4; i++)
          tile[i] = allRotationsIShape[tileModule][i]; // Get the 4 pieces of the new tile
        break;
      }
      case sS: {
        tileModule = (tileModule + 1) % 2;
        for (int i = 0; i < 4; i++)
          tile[i] = allRotationsSShape[tileModule][i]; // Get the 4 pieces of the new tile
        break;
      }
      case sL: {
        tileModule = (tileModule + 1) % 4;
        for (int i = 0; i < 4; i++)
          tile[i] = allRotationsLShape[tileModule][i]; // Get the 4 pieces of the new tile
        break;
      }
      case sT: {
        tileModule = (tileModule + 1) % 4;
        for (int i = 0; i < 4; i++)
          tile[i] = allRotationsTShape[tileModule][i]; // Get the 4 pieces of the new tile
        break;
      }
    }

    // calculate the range of the tile
    GLfloat originx = tilepos.x;
    GLfloat originy = tilepos.y;
    GLfloat xmax = tile[0].x;
    GLfloat ymax = tile[0].y;
    GLfloat xmin = tile[0].x;
    GLfloat ymin = tile[0].y;
    for (int i = 1; i < 4; i++) {
      if (tile[i].x > xmax) xmax = tile[i].x;
      if (tile[i].y > ymax) ymax = tile[i].y;
      if (tile[i].x < xmin) xmin = tile[i].x;
      if (tile[i].y < ymin) ymin = tile[i].y;
    }

    // adjust the position
    while(tilepos.x + xmax >  9) tilepos.x--;
    while(tilepos.y + ymax > 19) tilepos.y--;
    while(tilepos.x + xmin <  0) tilepos.x++;
    while(tilepos.y + ymin <  0) tilepos.y++;

    // if some tiles prevent rotation
    if(collisionDetect(CTN)) {
      switch (tileShape) {
        case sI: {
          tileModule = (tileModule + 1) % 2;
          for (int i = 0; i < 4; i++)
            tile[i] = allRotationsIShape[tileModule][i]; // Get the 4 pieces of the new tile
          break;
        }
        case sS: {
          tileModule = (tileModule + 1) % 2;
          for (int i = 0; i < 4; i++)
            tile[i] = allRotationsSShape[tileModule][i]; // Get the 4 pieces of the new tile
          break;
        }
        case sL: {
          tileModule = (tileModule + 3) % 4;
          for (int i = 0; i < 4; i++)
            tile[i] = allRotationsLShape[tileModule][i]; // Get the 4 pieces of the new tile
          break;
        }
        case sT: {
          tileModule = (tileModule + 3) % 4;
          for (int i = 0; i < 4; i++)
            tile[i] = allRotationsTShape[tileModule][i]; // Get the 4 pieces of the new tile
          break;
        }
      }
      tilepos.x = originx;
      tilepos.y = originy;
    }

    updateTile();

    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]); // Bind the VBO containing current tile vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  // Given (x,y), tries to move the tile x squares to the right and y squares down
  // Returns true if the tile was successfully moved, or false if there was some issue
  bool
  moveTile(vec2 direction) {
    // calculate the range of the tile
    GLfloat xmax = tile[0].x;
    GLfloat xmin = tile[0].x;
    GLfloat ymin = tile[0].y;
    for (int i = 1; i < 4; i++) {
      if (tile[i].x > xmax) xmax = tile[i].x;
      if (tile[i].x < xmin) xmin = tile[i].x;
      if (tile[i].y < ymin) ymin = tile[i].y;
    }
    int shifty = (int)direction.y;
    switch (shifty) {
      case 0: { // Horizontal movement
        int shiftx = (int)direction.x;
        switch (shiftx) {
          case -1: { // Left
            if (tilepos.x + xmin > 0 && !collisionDetect(LE)) tilepos.x--; // adjust position
            break;
          }
          case 1: { // Right
            if (tilepos.x + xmax < 9 && !collisionDetect(RI)) tilepos.x++; // adjust position
            break;
          }
          default:
            break;
        }
        break;
      }
      case -1: // Down
      case -2: { // Timer Drop
#ifdef GT_DEBUG_TIMER
        if(shifty == -2) std::cout << "timer tigger in movtile(vec2)\n";
#endif
        if(!collisionDetect(DO)) { // Can move downward
#ifdef GT_DEBUG_COLLISION
          std::cout << "no collision, moving downward...\n";
#endif
          tilepos.y--; // adjust position
        } else { // Collision
#ifdef GT_DEBUG_COLLISION
          std::cout << "[!!!]collision detected!\n";
#endif
          // Solid tile (update board occupation & color)
          setTile();
          // Clear map
          clearWholeMap();
          // Render Board (update VBO to OpenGL engine)
          // *** set up buffer objects
          glBindVertexArray(vaoIDs[1]);
          glGenBuffers(2, &vboIDs[2]);

          // Grid cell vertex colours
          glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
          glBufferData(GL_ARRAY_BUFFER, GT_GLOBAL_VERTEX_BOARD*sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
          glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
          glEnableVertexAttribArray(vColor);

          // New tile && Judge if the game is Over
          newTile();
        }
        break;
      }
      default:
        break;
    }

    updateTile();

    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]); // Bind the VBO containing current tile vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return true;
  }

  // Places the current tile - update the board vertex colour VBO and the array maintaining occupied cells
  void
  setTile() {
    for(int i = 0; i < 4; i++) {
      int offsetX = (int)tilepos.x + (int)tile[i].x;
      int offsetY = (int)tilepos.y + (int)tile[i].y;
#ifdef GT_DEBUG_TILE_POSITION
      cout << "CUR_POS on setTile() : X:" << offsetX << " - Y:" << offsetY << endl;
#endif
      board[offsetX][offsetY] = true;
      // Two points are reused
//      cout << "call set tile" << endl;
      vec4 offsetColor = palette[tiledColor[i]];
      updateBoardColor(offsetX, offsetY, offsetColor);
    }
#ifdef GT_DEBUG_OCCUPATION
    cout << "OCCUPATION\n";
    for(int i = 19; i >= 0; i--) {
      for(int j = 0; j < 10; j++)
        cout << board[j][i] << " ";
      cout << endl;
    } cout << endl;
#endif
  }

  // When the current tile is moved or rotated (or created), update the VBO containing its vertex position data
  void
  updateTile() {
    // Bind the VBO containing current tile vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

#ifdef GT_DEBUG_TILE_POSITION_ONLINE
    cout << "CUR_POS on updateTile()\n\t";
#endif
    // For each of the 4 'cells' of the tile,
    for (int i = 0; i < 4; i++)
    {
      // Calculate the grid coordinates of the cell
      GLfloat x = tilepos.x + tile[i].x;
      GLfloat y = tilepos.y + tile[i].y;

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

//-------------------------------------------------------------------------------------------------------------------

  // Checks if the current tile is collide with the bottom or the stack of tiles
  bool
  collisionDetect(gtDirection towards) {
    switch (towards) {
      case DO: { // Downward collision detection
        for(int i = 0; i < 4; i++) {
          int nxtPosX = (int)tilepos.x + (int)tile[i].x;
          int nxtPosY = (int)tilepos.y + (int)tile[i].y - 1;
#ifdef GT_DEBUG_COLLISION
          std::cout << "detecting nxtX:" << nxtPosX << " - nxtY:" << nxtPosY << ".\n";
#endif
          if(nxtPosY < 0 || board[nxtPosX][nxtPosY]) return true;
        }
        break;
      }
      case LE: { // Movement collision detection - moving left
        for(int i = 0; i < 4; i++) {
          int nxtPosX = (int)tilepos.x + (int)tile[i].x - 1;
          int nxtPosY = (int)tilepos.y + (int)tile[i].y;
#ifdef GT_DEBUG_COLLISION
          std::cout << "detecting nxtX:" << nxtPosX << " - nxtY:" << nxtPosY << ".\n";
#endif
          if(nxtPosX < 0 || board[nxtPosX][nxtPosY]) return true;
        }
        break;
      }
      case RI: { // Movement collision detection - moving left
        for(int i = 0; i < 4; i++) {
          int nxtPosX = (int)tilepos.x + (int)tile[i].x + 1;
          int nxtPosY = (int)tilepos.y + (int)tile[i].y;
#ifdef GT_DEBUG_COLLISION
          std::cout << "detecting nxtX:" << nxtPosX << " - nxtY:" << nxtPosY << ".\n";
#endif
          if(nxtPosX > 9 || board[nxtPosX][nxtPosY]) return true;
        }
        break;
      }
      case CTN: { // Rotation collision detection
        for(int i = 0; i < 4; i++) {
          int nxtPosX = (int)tilepos.x + (int)tile[i].x;
          int nxtPosY = (int)tilepos.y + (int)tile[i].y;
#ifdef GT_DEBUG_COLLISION
          std::cout << "detecting nxtX:" << nxtPosX << " - nxtY:" << nxtPosY << ".\n";
#endif
          if(board[nxtPosX][nxtPosY]) return true;
        }
        break;
      }
      default: {
        return false;
      }
    }
    return false;
  }

//-------------------------------------------------------------------------------------------------------------------

  // Eliminate any possible tile that could be eliminated
  void
  clearWholeMap() {
#ifdef GT_DEBUG_ELIMINATION
    cout << "Called clearWholeMap()\n";
#endif
#ifdef GT_DEBUG_COLOR_MATRIX
    cout << "OCCUPATION on COLOR (status)\n";
    for(int i = 19; i >= 0; i--) {
      for(int j = 0; j < 10; j++) {
        if(!board[j][i]) cout << "- ";
        else getColorName(j, i);
      }
      cout << endl;
    } cout << "*************************\n";
#endif
    while(!isWholeMapStatic()) {
#ifdef GT_DEBUG_COLOR_MATRIX
      cout << "OCCUPATION on COLOR (before)\n";
      for(int i = 19; i >= 0; i--) {
        for(int j = 0; j < 10; j++) {
          if(!board[j][i]) cout << "- ";
          else getColorName(j, i);
        }
        cout << endl;
      } cout << "*************************\n";
#endif
#ifdef GT_DEBUG_ELIMINATION_MATRIX
      cout << "LoopDetection, while(RES), RES=" << isRemovingMatrixEmpty << "\n";
      cout << "ELIMINATION MATRIX\n";
      for(int i = 19; i >= 0; i--) {
        for(int j = 0; j < 10; j++)
          cout << removingMatrix[j][i] << " ";
        cout << "\n";
      } cout << "*************************\n";
#endif
      // elimination
      for (int y = 19; y >= 0; y--) {
        for (int x = 0; x < 10; x++) {
          if (removingMatrix[x][y]) {
#ifdef GT_DEBUG_ELIMINATION_POINT_DETAIL
            cout << "removing row:" << y << "\tcol:" << x << endl;
#endif
            eliminatePoint(x, y);
          }
        }
      }
#ifdef GT_DEBUG_COLOR_MATRIX
      cout << "OCCUPATION on COLOR (eliminated)\n";
      for(int i = 19; i >= 0; i--) {
        for(int j = 0; j < 10; j++) {
          if(!board[j][i]) cout << "- ";
          else getColorName(j, i);
        }
        cout << endl;
      } cout << "*************************\n";
#endif
    }
  }

  // Check if all block in game cannot be eliminated
  bool
  isWholeMapStatic() {
#ifdef GT_DEBUG_ELIMINATION
    cout << "Called isWholeMapStatic()\n";
#endif

    // clear temporary data
    for(int i = 0; i < 10; i++)
      for (int j = 0; j < 20; j++)
        removingMatrix[i][j] = false;
    isRemovingMatrixEmpty = true;

#ifdef GT_DEBUG_ELIMINATION
    cout << "\tiWMS() checking horizontal...\n";
#endif
    // check horizontal
    for(int i = 19; i >= 0 ; i--) {
      for (int j = 0; j < 10; j++) {
        if(!board[j][i]) continue;  // if the board[j][i] is empty
#ifdef GT_DEBUG_ELIMINATION_HOR
        cout << "\t\tHOR on row:" << i << " - col:" << j;
#endif
        int count = 0;    // store the count of the same color
        vec4 targetColor = boardcolours[GT_GLOBAL_VERTEX_SINGLE_CUBE*(10*i+j)];
#ifdef GT_DEBUG_ELIMINATION_HOR_COLOR
        cout << "\t\t targetColor" << targetColor << "\n";
#endif
        for (int k = j; k < 10; k++) {
#ifdef GT_DEBUG_ELIMINATION_HOR_COLOR
          cout << "\t\t [" << k << "] boardColor" << boardcolours[GT_GLOBAL_VERTEX_SINGLE_CUBE*(10*i+k)] << "\n";
#endif
          if(isColorSame(boardcolours[GT_GLOBAL_VERTEX_SINGLE_CUBE*(10*i+k)], targetColor)) {
            count++;
            continue;
          } else break;
        }
#ifdef GT_DEBUG_ELIMINATION_HOR
        cout << "  count:" << count << endl;
#endif
        // prepare to eliminate if more than 3
        if(count >= 3) {
          for(int k = 0; k < count; k++) {
            removingMatrix[j+k][i] = true;
#ifdef GT_DEBUG_ELIMINATION_OUTPUT
            cout << "\t\t HOR_ELI >>> row:" << i << " col:" << j+k << "  count:" << count << "\n";
#endif
          }
          isRemovingMatrixEmpty = false;
        }
      }
    }

#ifdef GT_DEBUG_ELIMINATION
    cout << "\tiWMS() checking vertical...\n";
#endif
    // check vertical
    for(int i = 19; i >= 0 ; i--) {
      for (int j = 0; j < 10; j++) {
        if(!board[j][i]) continue;  // if the board[j][i] is empty
#ifdef GT_DEBUG_ELIMINATION_VER
        cout << "\t\tVER on row:" << i << " - col:" << j;
#endif
        int count = 0;    // store the count of the same color
        vec4 targetColor = boardcolours[GT_GLOBAL_VERTEX_SINGLE_CUBE*(10*i+j)];
        for (int k = i; k >= 0; k--) {
          if(isColorSame(boardcolours[GT_GLOBAL_VERTEX_SINGLE_CUBE*(10*k+j)], targetColor)) {
            count++;
            continue;
          } else break;
        }
#ifdef GT_DEBUG_ELIMINATION_VER
        cout << "  count:" << count << endl;
#endif
        // prepare to eliminate if more than 3
        if(count >= 3) {
          for(int k = 0; k < count; k++) {
            removingMatrix[j][i-k] = true;
#ifdef GT_DEBUG_ELIMINATION_OUTPUT
            cout << "\t\t VER_ELI >>> row:" << i-k << " col:" << j << "  count:" << count << "\n";
#endif
          }
          isRemovingMatrixEmpty = false;
        }
      }
    }

#ifdef GT_DEBUG_ELIMINATION
    cout << "\tiWMS() checking diagonal-LEFT...\n";
#endif
    // check diagonal - DL
    for(int i = 19; i >= 0 ; i--) {
      for (int j = 0; j < 10; j++) {
        if (!board[j][i]) continue;  // if the board[j][i] is empty
#ifdef GT_DEBUG_ELIMINATION_LDG
        cout << "\t\tLDG on row:" << i << " - col:" << j;
#endif
        int count = 0;    // store the count of the same color
        vec4 targetColor = boardcolours[GT_GLOBAL_VERTEX_SINGLE_CUBE*(10 * i + j)];
        int offsetX = j, offsetY = i;
        while(isColorSame(boardcolours[GT_GLOBAL_VERTEX_SINGLE_CUBE*(10*offsetY+offsetX)], targetColor)) {
          count++;
          if(!(offsetX-1>=0 && offsetY-1>=0)) break;
          offsetX--;
          offsetY--;
        }
#ifdef GT_DEBUG_ELIMINATION_LDG
        cout << "  count:" << count << endl;
#endif
        if(count >= 3) {
          for(int k = 0; k < count; k++) {
            removingMatrix[j-k][i-k] = true;
#ifdef GT_DEBUG_ELIMINATION_OUTPUT
            cout << "\t\t LDG_ELI >>> row:" << i-k << " col:" << j-k << "  count:" << count << "\n";
#endif
          }
          isRemovingMatrixEmpty = false;
        }
      }
    }

#ifdef GT_DEBUG_ELIMINATION
    cout << "\tiWMS() checking diagonal-RIGHT...\n";
#endif
    // check diagonal - DR
    for(int i = 19; i >= 0 ; i--) {
      for (int j = 0; j < 10; j++) {
        if (!board[j][i]) continue;  // if the board[j][i] is empty
#ifdef GT_DEBUG_ELIMINATION_RDG
        cout << "\t\tRDG on row:" << i << " - col:" << j;
#endif
        int count = 0;    // store the count of the same color
        vec4 targetColor = boardcolours[GT_GLOBAL_VERTEX_SINGLE_CUBE*(10 * i + j)];
        int offsetX = j, offsetY = i;
        while(isColorSame(boardcolours[GT_GLOBAL_VERTEX_SINGLE_CUBE*(10*offsetY+offsetX)], targetColor)) {
          count++;
          if(!(offsetX+1<=9 && offsetY-1>=0)) break;
          offsetX++;
          offsetY--;
        }
#ifdef GT_DEBUG_ELIMINATION_RDG
        cout << "  count:" << count << endl;
#endif
        if(count >= 3) {
          for(int k = 0; k < count; k++) {
            removingMatrix[j+k][i-k] = true;
#ifdef GT_DEBUG_ELIMINATION_OUTPUT
            cout << "\t\t RDG_ELI >>> row:" << i-k << " col:" << j+k << "  count:" << count << "\n";
#endif
          }
          isRemovingMatrixEmpty = false;
        }
      }
    }

    // check full row
    for(int i = 0; i < 20; i++)
      checkFullRow(i);

#ifdef GT_DEBUG_ELIMINATION
    cout << "\t\tisRemovingMatrixEmpty:" << isRemovingMatrixEmpty << "\n";
#endif

    return isRemovingMatrixEmpty;
  }

  // Eliminate one single block
  void
  eliminatePoint(int x, int y) {
    board[x][y] = false; // eliminate occupation
    int movement = 0;
    // calculate distance of downward
    for(int i = y; i >=0; i--) {
      if(!board[x][i]) movement++;
      else break;
    }
    // move upside blocks downward
    for(int i = y + 1; i < 20; i++) {
      board[x][i-movement] = board[x][i];
      updateBoardColor(x, i-movement, boardcolours[GT_GLOBAL_VERTEX_SINGLE_CUBE*(10*i+x)]);
    }
    // update the top block on this column
    for(int i = 19; i >= 20 - movement; i--) {
      board[x][i] = false;
      updateBoardColor(x, i, palette[black]);
    }
  }

  // Checks if the specified row (0 is the bottom 19 the top) is full
  // If every cell in the row is occupied, it will clear that cell and everything above it will shift down one row
  inline void
  checkFullRow(int row) {
    bool isFull = true;
    for(int i = 0; i < 10; i++) {
      if (board[i][row]) {
        continue;
      }
      else {
        isFull = false;
        break;
      }
    }
    // detect a full row
    if(isFull) {
#ifdef GT_DEBUG_ELIMINATION
      cout << "detect a full row:" << row << ", eliminating...\n";
#endif
      for(int i = 0; i < 10; i++)
        removingMatrix[i][row] = true;
      isRemovingMatrixEmpty = false;
    }
  }

  inline void
  updateBoardColor(int x, int y, vec4 c) {
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


//    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
//
//
//      // Create the 4 corners of the square - these vertices are using location in pixels
//      // These vertices are later converted by the vertex shader
//      vec4 p1 = vec4((GLfloat)(33.0 + (x * 33.0)), (GLfloat)(33.0 + (y * 33.0)), (GLfloat)16.5, 1);   // FBL
//      vec4 p2 = vec4((GLfloat)(33.0 + (x * 33.0)), (GLfloat)(66.0 + (y * 33.0)), (GLfloat)16.5, 1);   // FTL
//      vec4 p3 = vec4((GLfloat)(66.0 + (x * 33.0)), (GLfloat)(33.0 + (y * 33.0)), (GLfloat)16.5, 1);   // FBR
//      vec4 p4 = vec4((GLfloat)(66.0 + (x * 33.0)), (GLfloat)(66.0 + (y * 33.0)), (GLfloat)16.5, 1);   // FTR
//      vec4 p5 = vec4((GLfloat)(33.0 + (x * 33.0)), (GLfloat)(33.0 + (y * 33.0)), (GLfloat)-16.5, 1);  // BBL
//      vec4 p6 = vec4((GLfloat)(33.0 + (x * 33.0)), (GLfloat)(66.0 + (y * 33.0)), (GLfloat)-16.5, 1);  // BTL
//      vec4 p7 = vec4((GLfloat)(66.0 + (x * 33.0)), (GLfloat)(33.0 + (y * 33.0)), (GLfloat)-16.5, 1);  // BBR
//      vec4 p8 = vec4((GLfloat)(66.0 + (x * 33.0)), (GLfloat)(66.0 + (y * 33.0)), (GLfloat)-16.5, 1);  // BTR
//
//
//      // Two points are used by two triangles each
//      vec4 newpoints[36] = {
//          p1, p2, p3, p2, p3, p4,     // Front
//          p1, p2, p5, p2, p5, p6,     // Left
//          p5, p6, p7, p6, p7, p8,     // Back
//          p3, p4, p7, p4, p7, p8,     // Right
//          p2, p4, p6, p4, p6, p8,     // Top
//          p1, p3, p5, p3, p5, p7      // Bottom
//      };
//
//      // Put new data in the VBO
//      glBufferSubData(GL_ARRAY_BUFFER, i*36*sizeof(vec4), 36*sizeof(vec4), newpoints);

  }

//-------------------------------------------------------------------------------------------------------------------

  // Judge if (vec4)a == (vec4)b
  inline bool
  isColorSame(vec4 a, vec4 b) {
    return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
  }

  // Output the color's name, using for debug
  void
  getColorName(int x, int y) {
    vec4 tarColor = boardcolours[GT_GLOBAL_VERTEX_SINGLE_CUBE*(10*y + x)];
    if(isColorSame(tarColor, palette[purple])) cout << "P ";
    if(isColorSame(tarColor, palette[red]))    cout << "R ";
    if(isColorSame(tarColor, palette[yellow])) cout << "Y ";
    if(isColorSame(tarColor, palette[green]))  cout << "G ";
    if(isColorSame(tarColor, palette[orange])) cout << "O ";
    if(isColorSame(tarColor, palette[white]))  cout << "EW";
    if(isColorSame(tarColor, palette[black]))  cout << "EB";
  }

//-------------------------------------------------------------------------------------------------------------------

  // Starts the game over - empties the board, creates new tiles, resets line counters
  void
  restart() {
    //TODO: reset restart() func location
    // Initialize the grid, the board, and the current tile
    initGrid();
    initBoard();
    initCurrentTile();

    // Reset drop speed
    DROP_SPEED = 600;
    DROP_SHIFT = 100;

    // Game initialization
    newTile(); // create new next tile

    // set to default
    glBindVertexArray(0);
    glClearColor(0, 0, 0, 0);
  }

}
