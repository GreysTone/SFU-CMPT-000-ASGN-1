//
// Created by Danyang Song (Arthur) on 10/02/16.
//

#include "gameLogic.h"

namespace GT_gameLogic {

  // current tile
  vec2 tile[4]; // An array of 4 2d vectors representing displacement from a 'center' piece of the tile, on the grid
  vec2 tilepos = vec2(5, 19); // The position of the current tile using grid coordinates ((0,0) is the bottom left corner)
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

  // VAO and VBO
  GLuint vaoIDs[3]; // One VAO for each object: the grid, the board, the current piece
  GLuint vboIDs[6]; // Two Vertex Buffer Objects for each VAO (specifying vertex positions and colours, respectively)

  // random generator
  //  std::default_random_engine generator;
  //  std::uniform_int_distribution<int> dist(0,4);

//-------------------------------------------------------------------------------------------------------------------

  void initGrid()
  {
    // ***Generate geometry data
    vec4 gridpoints[64]; // Array containing the 64 points of the 32 total lines to be later put in the VBO
    vec4 gridcolours[64]; // One colour per vertex
    // Vertical lines
    for (int i = 0; i < 11; i++){
      gridpoints[2*i] = vec4((33.0 + (33.0 * i)), 33.0, 0, 1);
      gridpoints[2*i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 0, 1);

    }
    // Horizontal lines
    for (int i = 0; i < 21; i++){
      gridpoints[22 + 2*i] = vec4(33.0, (33.0 + (33.0 * i)), 0, 1);
      gridpoints[22 + 2*i + 1] = vec4(363.0, (33.0 + (33.0 * i)), 0, 1);
    }
    // Make all grid lines white
    for (int i = 0; i < 64; i++)
      gridcolours[i] = palette[white];


    // *** set up buffer objects
    // Set up first VAO (representing grid lines)
    glBindVertexArray(vaoIDs[0]); // Bind the first VAO
    glGenBuffers(2, vboIDs); // Create two Vertex Buffer Objects for this VAO (positions, colours)

    // Grid vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]); // Bind the first grid VBO (vertex positions)
    glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridpoints, GL_STATIC_DRAW); // Put the grid points in the VBO
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition); // Enable the attribute

    // Grid vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]); // Bind the second grid VBO (vertex colours)
    glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridcolours, GL_STATIC_DRAW); // Put the grid colours in the VBO
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColor); // Enable the attribute
  }

  void initBoard()
  {
    // *** Generate the geometric data
    vec4 boardpoints[1200];
    for (int i = 0; i < 1200; i++)
      boardcolours[i] = palette[black]; // Let the empty cells on the board be black
    // Each cell is a square (2 triangles with 6 vertices)
    for (int i = 0; i < 20; i++){
      for (int j = 0; j < 10; j++)
      {
        vec4 p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
        vec4 p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);
        vec4 p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
        vec4 p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);

        // Two points are reused
        boardpoints[6*(10*i + j)    ] = p1;
        boardpoints[6*(10*i + j) + 1] = p2;
        boardpoints[6*(10*i + j) + 2] = p3;
        boardpoints[6*(10*i + j) + 3] = p2;
        boardpoints[6*(10*i + j) + 4] = p3;
        boardpoints[6*(10*i + j) + 5] = p4;
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
    glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints, GL_STATIC_DRAW);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);

    // Grid cell vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
    glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
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
    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);

    // Current tile vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColor);
  }

//-------------------------------------------------------------------------------------------------------------------

  // Called at the start of play and every time a tile is placed
  void
  newTile() {
    tileShape = gtShape(rand() % 3);

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

    updateTile();


    // Update the color VBO of current tile
    vec4 newcolours[24];
    for (int i = 0; i < 24; i += 6) {
      // vec4 tiled = palette[dist(generator)]; // randomize the color
      vec4 tiled = palette[rand() % 5]; // randomize the color
      for (int j = 0; j < 6; j++)
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

    updateTile();

    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]); // Bind the VBO containing current tile vertex colours
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  // Given (x,y), tries to move the tile x squares to the right and y squares down
  // Returns true if the tile was successfully moved, or false if there was some issue
  bool
  movetile(vec2 direction) {
    int shifty = (int)direction.y;
#ifdef _GT_DEBUG_
    std::cout << "shifty - " << shifty << std::endl;
#endif
    switch (shifty) {
      case 0: { // Horizontal movement
        int shiftx = (int)direction.x;
#ifdef _GT_DEBUG_
        std::cout << "shiftx - " << shiftx << std::endl;
#endif
        // calculate the range of the tile
        GLfloat xmax = tile[0].x;
        GLfloat xmin = tile[0].x;
        for (int i = 1; i < 4; i++) {
          if (tile[i].x > xmax) xmax = tile[i].x;
          if (tile[i].x < xmin) xmin = tile[i].x;
        }
        switch (shiftx) {
          case -1: { // Left
            std::cout << "xo:" << tilepos.x << "\n";
            if (tilepos.x + xmin > 0) tilepos.x--; // adjust position
            std::cout << "xo:" << tilepos.x << "\n";
            break;
          }
          case 1: { // Right
            if (tilepos.x + xmax < 9) tilepos.x++; // adjust position
            break;
          }
        }
        updateTile();

        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]); // Bind the VBO containing current tile vertex colours
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        break;
      }
      case -1: { // Down
        break;
      }
    }
    return false;
  }

  // When the current tile is moved or rotated (or created), update the VBO containing its vertex position data
  void
  updateTile()
  {
    // Bind the VBO containing current tile vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

    // For each of the 4 'cells' of the tile,
    for (int i = 0; i < 4; i++)
    {
      // Calculate the grid coordinates of the cell
      GLfloat x = tilepos.x + tile[i].x;
      GLfloat y = tilepos.y + tile[i].y;

      // Create the 4 corners of the square - these vertices are using location in pixels
      // These vertices are later converted by the vertex shader
      vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
      vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
      vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
      vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);

      // Two points are used by two triangles each
      vec4 newpoints[6] = {p1, p2, p3, p2, p3, p4};

      // Put new data in the VBO
      glBufferSubData(GL_ARRAY_BUFFER, i*6*sizeof(vec4), 6*sizeof(vec4), newpoints);
    }

    glBindVertexArray(0);
  }

//-------------------------------------------------------------------------------------------------------------------

  // Checks if the specified row (0 is the bottom 19 the top) is full
  // If every cell in the row is occupied, it will clear that cell and everything above it will shift down one row
  void
  checkfullrow(int row) {

  }

  // Places the current tile - update the board vertex colour VBO and the array maintaining occupied cells
  void
  settile() {

  }

//-------------------------------------------------------------------------------------------------------------------

  void
  timer() {

  }

  // Starts the game over - empties the board, creates new tiles, resets line counters
  void
  restart() {
    // Initialize the grid, the board, and the current tile
    initGrid();
    initBoard();
    initCurrentTile();

    // Game initialization
    newTile(); // create new next tile

    // set to default
    glBindVertexArray(0);
    glClearColor(0, 0, 0, 0);
  }

}
