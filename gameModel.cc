//
// CMPT 361 Assignment 2 - Fruit Tetris
// Implemented in Feb 2016 by Danyang Song (Arthur, arthur_song@sfu.ca).
// (Network ID: GreysTone)
//

#include "include/Angel.h"
#include "gameSetting.h"
#include "gameModel.h"

#include <iostream>
using namespace std;

namespace GT_gameModel {

GLfloat modelUnit = 16.5;
void unitCube(vec4 *Mat) {
  for(int i=0; i<8; i++) {
    Mat[i] = vec4(((i&4)?-modelUnit:modelUnit), ((i&2)?-modelUnit:modelUnit), ((i&1)?-modelUnit:modelUnit), 1);
  }
}
void shadeFace(vec4 *cube, vec4 p1, vec4 p2, vec4 p3, vec4 p4) {
  cube[0] = p1; cube[1] = p2; cube[2] = p3;
  cube[3] = p2; cube[4] = p3; cube[5] = p4;

}
void shadeCube(vec4 *cube, vec4 *p) {
  shadeFace(&cube[ 0], p[0], p[1], p[2], p[3]); // front
  shadeFace(&cube[ 6], p[0], p[1], p[4], p[5]); // left
  shadeFace(&cube[12], p[4], p[5], p[6], p[7]); // back
  shadeFace(&cube[18], p[2], p[3], p[6], p[7]); // right
  shadeFace(&cube[24], p[1], p[3], p[5], p[7]); // top
  shadeFace(&cube[30], p[0], p[2], p[4], p[6]); // bottom
}

//  vec4 cube3[36] = {
//      p[0], p[1], p[2], p[1], p[2], p[3],     // Front
//      p[0], p[1], p[4], p[1], p[4], p[5],     // Left
//      p[4], p[5], p[6], p[5], p[6], p[7],     // Back
//      p[2], p[3], p[6], p[3], p[6], p[7],     // Right
//      p[1], p[3], p[5], p[3], p[5], p[7],     // Top
//      p[0], p[2], p[4], p[2], p[4], p[6]      // Bottom
//  };

namespace GRID {
vec4 vertexArray[GT_GLOBAL_VERTEX_GRID];
vec4 colourArray[GT_GLOBAL_VERTEX_GRID];

void setupModel() {
  // Vertex
  // vertical lines [+16.5]
  for (int i = 0; i < 11; i++){
    vertexArray[2*i] = vec4((GLfloat)(33.0 + (33.0 * i)), 33.0, (GLfloat)16.6, 1);
    vertexArray[2*i + 1] = vec4((GLfloat)(33.0 + (33.0 * i)), 693.0, (GLfloat)16.6, 1);
  }
  // vertical lines [-16.5]
  for (int i = 0; i < 11; i++){
    vertexArray[22 + 2*i] = vec4((GLfloat)(33.0 + (33.0 * i)), 33.0, (GLfloat)-16.6, 1);
    vertexArray[22 + 2*i + 1] = vec4((GLfloat)(33.0 + (33.0 * i)), 693.0, (GLfloat)-16.6, 1);
  }
  // horizontal lines [+16.5]
  for (int i = 0; i < 21; i++){
    vertexArray[44 + 2*i] = vec4(33.0, (GLfloat)(33.0 + (33.0 * i)), (GLfloat)16.6, 1);
    vertexArray[44 + 2*i + 1] = vec4(363.0, (GLfloat)(33.0 + (33.0 * i)), (GLfloat)16.6, 1);
  }
  // horizontal lines [-16.5]
  for (int i = 0; i < 21; i++){
    vertexArray[86 + 2*i] = vec4(33.0, (GLfloat)(33.0 + (33.0 * i)), (GLfloat)-16.6, 1);
    vertexArray[86 + 2*i + 1] = vec4(363.0, (GLfloat)(33.0 + (33.0 * i)), (GLfloat)-16.6, 1);
  }
  // depth lines
  for (int i = 0; i < GT_GLOBAL_HEIGHT_BOARD + 1; i++){
    for (int j = 0; j < GT_GLOBAL_WIDTH_BOARD + 1; j++) {
      vertexArray[128 + 22*i + 2*j] 		= vec4((GLfloat)33.0 + (GLfloat)(j * 33.0), (GLfloat)33.0 + (GLfloat)(i * 33.0), (GLfloat)16.6, 1);
      vertexArray[128 + 22*i + 2*j + 1] 	= vec4((GLfloat)33.0 + (GLfloat)(j * 33.0), (GLfloat)33.0 + (GLfloat)(i * 33.0), (GLfloat)-16.6, 1);
    }
  }

  // Colour
  for (int i = 0; i < GT_GLOBAL_VERTEX_GRID; i++)
    colourArray[i] = GT_gameSetting::palette[GT_gameSetting::white];
}
} // namespace GRID

namespace BOARD {
vec4 vertexArray[GT_GLOBAL_VERTEX_BOARD];
vec4 colourArray[GT_GLOBAL_VERTEX_BOARD];

void setupModel() {
  //Vertex
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
        vertexArray[GT_GLOBAL_VERTEX_SINGLE_CUBE*(10 * i + j) + k] = cube[k];
    }
  }

  //Colour
  for (int i = 0; i < GT_GLOBAL_VERTEX_BOARD; i++)
    colourArray[i] = GT_gameSetting::palette[GT_gameSetting::black];
}
}

namespace TILE {
vec4 vertexArray[GT_GLOBAL_VERTEX_TILE];
vec4 colourArray[GT_GLOBAL_VERTEX_TILE];
}
namespace ARM{
vec4 vertexArray[GT_GLOBAL_VERTEX_SINGLE_CUBE];
vec4 colourArray[GT_GLOBAL_VERTEX_SINGLE_CUBE];

GLfloat BASE_Scale_X     = 5.0;
GLfloat BASE_Scale_Y     = 1.0;
GLfloat BASE_Scale_Z     = 4.0;
GLfloat BASE_Trans_X     = (GLfloat)-80.0;
GLfloat BASE_Trans_Y     = 33.0;
GLfloat LOW_Scale_X      = 1.0;
GLfloat LOW_Scale_Y      = 8.0;
GLfloat LOW_Scale_Z      = 1.0;
GLfloat HIGH_Scale_X     = 1.0;
GLfloat HIGH_Scale_Y     = 16.0;
GLfloat HIGH_Scale_Z     = 1.0;

int Theta = 0;
int Phi = -45;

void setupModel() {
  vec4 cuboid[8];

//  // Vertex - Base
//  unitCube(cuboid);
//  for(int i=0; i<8; i++) {
//    cuboid[i] = Scale(BASE_Scale_X, BASE_Scale_Y, BASE_Scale_Z) * cuboid[i];
//    cuboid[i] = Translate(BASE_Trans_X, BASE_Trans_Y, 0) * cuboid[i];
//    cout << i << "-" << cuboid[i] << endl;
//  }
//
//  vec4 base[36];
//  shadeCube(base, cuboid);
//  for(int k=0; k<GT_GLOBAL_VERTEX_SINGLE_CUBE; k++) {
//    vertexArray[k] = base[k];
//    cout << k << "~" << base[k] << endl;
//  }

    // Build Model - Base
//  float unit = 16.5;
//  vec4 p[8] = {
//      vec4( unit, unit, unit, 1),
//      vec4( unit, unit,-unit, 1),
//      vec4( unit,-unit, unit, 1),
//      vec4( unit,-unit,-unit, 1),
//      vec4(-unit, unit, unit, 1),
//      vec4(-unit, unit,-unit, 1),
//      vec4(-unit,-unit, unit, 1),
//      vec4(-unit,-unit,-unit, 1)
//  };
//  for(int i=0; i<8; i++) {
//    p[i] = Scale(5, 1, 4) * p[i];
//    p[i] = Translate(-80, 33, 0) * p[i];
//  }
//  vec4 cube1[36] = {
//      p[0], p[1], p[2], p[1], p[2], p[3],     // Front
//      p[0], p[1], p[4], p[1], p[4], p[5],     // Left
//      p[4], p[5], p[6], p[5], p[6], p[7],     // Back
//      p[2], p[3], p[6], p[3], p[6], p[7],     // Right
//      p[1], p[3], p[5], p[3], p[5], p[7],     // Top
//      p[0], p[2], p[4], p[2], p[4], p[6]      // Bottom
//  };
//  for(int k=0; k<GT_GLOBAL_VERTEX_SINGLE_CUBE; k++)
//    vertexArray[k] = cube1[k];


//  // Build Model - Lower Arm
//  unitCube(cuboid);
//  for(int i=0; i<8; i++) {
//    cuboid[i] = Scale(LOW_Scale_X, LOW_Scale_Y, LOW_Scale_Z) * cuboid[i];
//    cuboid[i] = Translate(0, LOW_Scale_Y/2*33, 0) * cuboid[i];
//    cuboid[i] = RotateZ(Theta) * cuboid[i];
//    cuboid[i] = Translate(BASE_Trans_X, BASE_Trans_Y, 0) * cuboid[i];
//  }
//  vec4 lower[36];
//  shadeCube(lower, cuboid);
//  for(int k=0; k<GT_GLOBAL_VERTEX_SINGLE_CUBE; k++)
//    vertexArray[36+k] = lower[k];
//
//  // Build Model - Higher Arm
//  unitCube(cuboid);
//  for(int i=0; i<8; i++) {
//    cuboid[i] = Scale(HIGH_Scale_X, HIGH_Scale_Y, HIGH_Scale_Z) * cuboid[i];
//    cuboid[i] = Translate(0, HIGH_Scale_Y/2*33, 0) * cuboid[i];
//    cuboid[i] = RotateZ(Phi) * cuboid[i];
//    cuboid[i] = Translate(-HIGH_Scale_Y/2*33*sin(DegreesToRadians * Theta),HIGH_Scale_Y/2*33*cos(DegreesToRadians * Theta),0) * cuboid[i];
//    cuboid[i] = Translate(BASE_Trans_X, BASE_Trans_Y, 0) * cuboid[i];
//  }
//  vec4 higher[36];
//  shadeCube(higher, cuboid);
//  for(int k=0; k<GT_GLOBAL_VERTEX_SINGLE_CUBE; k++)
//    vertexArray[72+k] = higher[k];


  // Colour
  for (int i = 0; i < 36; i++)
    colourArray[i] = GT_gameSetting::palette[GT_gameSetting::purple];

  for (int i = 36; i < 72; i++)
    colourArray[i] = GT_gameSetting::palette[GT_gameSetting::red];;

  for (int i = 72; i < 108; i++)
    colourArray[i] = GT_gameSetting::palette[GT_gameSetting::green];
}

} // namespace ARM
} // namespace GT_gameModel