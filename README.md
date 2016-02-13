# FruitTetris

Assignment 1 for *Intros to CG*

Based on "Skeleton Code" (commit 731f2cf343bc525ebcd16e506c7dd5e652c6d228 on GitLab).

***Author: Danyang Song (Arthur)***

## Compile & Deploy

## Settings
* Colors
    * vec4(1.0, 0.5, 1.0, 1.0) - purple
    * vec4(1.0, 0.2, 0.1, 1.0) - red
    * vec4(1.0, 1.0, 0.3, 1.0) - yellow
    * vec4(0.3, 1.0, 0.6, 1.0) - green
    * vec4(1.0, 0.6, 0.2, 1.0) - orange
 
* Drop Speed
    * \#define DROP_SPEED 800

* Keyboard
    * **'UpArrow'**, **'t'** - rotate tile
    * **'LeftArrow'**, **'RightArrow'** - move tile
    * **'DownArrow'** - set tile down to the bottom
    * **'r'** - restart
    * **'ESC'**, **'q'** - exit game

## Features
* Source code style
    * according to *OpenGL Programming Guide*, namespace, multi-files
* MACRO debug features
    * \#define GT_DEBUG_...
* **Makefile** provided
* **'-g'** compile option for *gdb*
* **'-pg'** compile option for *gprof* profiling
