# FruitTetris

Assignment 2 for *Intros to CG*

Based on "Skeleton Code" (the first submit 731f2cf343bc525ebcd16e506c7dd5e652c6d228).

***Author: Danyang Song (Arthur)***

*More details on completing this project, see https://github.com/GreysTone/SFU-CMPT-000-ASGN-1.*

***All steps (all functions) are implemented in the assignment, extra information are as below.***
******

## Compile, Deploy & OS Environment
* Linux (**tested on Ubuntu 14.04.03 LTS in VMware 8**)
    * Dependency: glew, freeglut3, mesa (possibly need other package based on your environment)
    * In an initial environment
        * make && ./FruitTetris
    * Otherwise
        * make clean
        * make && ./FruitTetris
* OS X (**tested on OS X El Caption 10.11.3**)
  * Dependency: XCode 7.2.1
  * Using the project file in *./FruitTetris_XCode/FruitTetris.xcodeproj*
    * add **GLEW.framework** & **OpenGL.framework** if the framework are not automatically import to the project.
  * Some header files and functions are *deprecated* and(or) even *not supported* by OS X. See more details in source code with the MACRO **#ifdef \_\_APPLE\_\_** or **#ifndef \_\_APPLE\_\_**
  * Some MACRO for debugging in *mat.h* are *commented* to make the program building successfully in OS X environment.
  * The *GL Shader Language* supported by OS X is on **version 150**, therefore the .glsl files are provided in ./FruitTetris_XCode/FruitTetris_source_XCode/\*.glsl.

## Features
* ***Anti_Aliasing Feature***
* ***On Mac, using SHIFT rather than CTRL to rotate 3D projection***
* To make this game more complicated, only on the row of the block that will be eliminated will get a downward movement
* When the game is over, there will be a prompt in CLI, to decide to continue or quit the game.
* If the random position for new tile is not available, it will find a possible position to generate it, otherwise game is over.
* Game could pause by 'p' pressed.
* Could accelerate the dorp speed during game by 'a' pressed.
* Source code style
  * according to *OpenGL Programming Guide*, namespace, multi-files
* MACRO debug features
  * \#define GT_DEBUG_...
* **Makefile** provided
* **'-g'** compile option for *gdb*
* **'-pg'** compile option for *gprof* profiling

## MACRO DEBUG Description
* gameSetting.h (GT_gameSetting namespace)
  * **GT_SET_ANTI_ALIASING** - Open anti-aliasing feature
  * GT_DEBUG_TIMER - Output **glutTimerFunc** info
  * **GT_DEBUG_CLOSE_TIMER** - Close **glutTimerFunc** callback, only make tile downward by *DownArrow* Key
  * GT_DEBUG_SPECIAL_KEYINFO - Output key pressed info
* gameSetting.h (GT_gameLogic namespace)
  * GT_DEBUG_COLLISION - Output collision detection info
  * GT_DEBUG_TILE_POSITION_ONLINE - Output tile's current position (online)
  * GT_DEBUG_TILE_POSITION - Output tile's current position
  * GT_DEBUG_OCCUPATION - Output Occupation Matrix
  * **GT_DEBUG_SOLID_COLOR** - Make new tile with all same color (red)
  * GT_DEBUG_COLOR_MATRIX - Output Occupation Matrix with color's name on that block
  * GT_DEBUG_ELIMINATION_MATRIX - Output Elimination Matrix
  * GT_DEBUG_ELIMINATION - Output elimination info
  * GT_DEBUG_ELIMINATION_HOR - Output elimination info in more details on Horizontal elimination
  * GT_DEBUG_ELIMINATION_VER - Output elimination info in more details on Vertical elimination
  * GT_DEBUG_ELIMINATION_LDG - Output elimination info in more details on Left-Down Diagonal elimination
  * GT_DEBUG_ELIMINATION_RDG - Output elimination info in more details on Right-Down Diagonal elimination
  * GT_DEBUG_ELIMINATION_OUTPUT - Output eliminated point in each loop
  * GT_DEBUG_ELIMINATION_POINT_DETAIL - Confirmed eliminated point in each loop


## Elimination Priority
* From High to Low
    * Horizontal
    * Vertical
    * Left-Down Diagonal
    * Right-Down Diagonal
    * Full Row

## Settings
* Colors
    * vec4(1.0, 0.5, 1.0, 1.0) - purple
    * vec4(1.0, 0.2, 0.1, 1.0) - red
    * vec4(1.0, 1.0, 0.3, 1.0) - yellow
    * vec4(0.3, 1.0, 0.6, 1.0) - green
    * vec4(1.0, 0.6, 0.2, 1.0) - orange

* Drop Speed
    * Initial DROP_SPEED is 600, and DROP_SHIFT is 100
    * Once 'a' pressed, DROP_SPEED -= DROP_SHIFT

* Keyboard
    * **'CTRL(SHIFT)' + 'LeftArrow'/'RightArrow'** - rotate projection
    * **'UpArrow'**, **'t'** - rotate tile
    * **'LeftArrow'**, **'RightArrow'** - move tile
    * **'DownArrow'** - set tile down to the bottom
    * **'a'** - accelerate the drop time
    * **'p'** - pause, when game is pause, other special key will not be triggered
    * **'r'** - restart
    * **'ESC'**, **'q'** - exit game
