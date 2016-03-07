//
// CMPT 361 Assignment 2 - Fruit Tetris
// Implemented in Feb 2016 by Danyang Song (Arthur, arthur_song@sfu.ca).
// (Network ID: GreysTone)
//

#include "gameLogic.h"

namespace GT_gameLogic {

  gtShape tileShape = sI;
  int tileModule = 0;

  //board[x][y] represents whether the cell (x,y) is occupied
  bool board[10][20];

  // random generator
  //  std::default_random_engine generator;
  //  std::uniform_int_distribution<int> dist(0,4);

  // Searching Temporary Data
  bool removingMatrix[10][20];
  bool isRemovingMatrixEmpty;

  bool gamePause = false;

} // namespace GT_gameLogic

//-------------------------------------------------------------------------------------------------------------------

void
GT_gameLogic::init() {
    GT_gameDrawing::initOpenGL();

    // initialize the grid, the board, and the current tile
    initGrid();
    initBoard();
    initCurrentTile();

    // initially no cell is occupied
    for (int i = 0; i < 10; i++)
      for (int j = 0; j < 20; j++)
        board[i][j] = false;

    // init projection
    ViewMat = LookAt(projectionEye, projectionAt, projectionUp);

    // game initialization
    DROP_SPEED = 600; // set drop speed
    DROP_SHIFT = 100;
    newTile(); // create new next tile
  }

//-------------------------------------------------------------------------------------------------------------------

  // Called at the start of play and every time a tile is placed
  void
  GT_gameLogic::newTile() {
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
  GT_gameLogic::rotate() {
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
  GT_gameLogic::moveTile(vec2 direction) {
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
  GT_gameLogic::setTile() {
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



//-------------------------------------------------------------------------------------------------------------------

  // Checks if the current tile is collide with the bottom or the stack of tiles
  bool
  GT_gameLogic::collisionDetect(gtDirection towards) {
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
  GT_gameLogic::clearWholeMap() {
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
  GT_gameLogic::isWholeMapStatic() {
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
  GT_gameLogic::eliminatePoint(int x, int y) {
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
  GT_gameLogic::checkFullRow(int row) {
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

//-------------------------------------------------------------------------------------------------------------------

  // Judge if (vec4)a == (vec4)b
  inline bool
  GT_gameLogic::isColorSame(vec4 a, vec4 b) {
    return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
  }

  // Output the color's name, using for debug
  void
  GT_gameLogic::getColorName(int x, int y) {
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
  GT_gameLogic::restart() {
    init();
  }

// Handle arrow key keypresses
void
GT_gameLogic::special(int key, int x, int y) {
  if(gamePause) return;  // Skip triggering special key when game is pause
  bool CTRL = false;
  int modifier = glutGetModifiers();
#ifdef __APPLE__
  if (modifier != 0 && modifier == GLUT_ACTIVE_SHIFT) {
    CTRL = 1;
  }
#else
    if (modifier != 0 && modifier == GLUT_ACTIVE_CTRL) {
      CTRL = 1;
  }
#endif
  else if (CTRL) {
    CTRL = 0;
  }

#ifdef GT_DEBUG_SPECIAL_KEYINFO
  switch (key) {
    case 100: // Left Arrow
      std::cout << "[LEFT Arrow] Pressed.\n";
      break;
    case 101: // Up Arrow (Rotate Tile)
      std::cout << "[UP Arrow] Pressed.\n";
      break;
    case 102: // Right Arrow
      std::cout << "[RIGHT Arrow] Pressed.\n";
      break;
    case 103: // Down Arrow
      std::cout << "[DOWN Arrow] Pressed.\n";
      break;
  }
}
#endif

  switch (key) {
    case 100: // Left Arrow
      if(CTRL) { ViewMat *= RotateY(-5);}
      else { moveTile(vec2(-1, 0)); }
      break;
    case 101: // Up Arrow (Rotate Tile)
      if(CTRL) { }
      else { rotate(); }
      break;
    case 102: // Right Arrow
      if(CTRL) { ViewMat *= RotateY(5); }
      else { moveTile(vec2(1, 0)); }
      break;
    case 103: // Down Arrow
      if(CTRL) { }
      else { moveTile(vec2(0, -1)); }
      break;
  }
}

// Handles standard key press
void
GT_gameLogic::keyboard(unsigned char key, int x, int y) {
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

void
GT_gameLogic::timerDrop(int data) {
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