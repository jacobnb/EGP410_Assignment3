[Github Commit](https://github.com/jacobnb/EGP410_Assignment4)
[Github Tag](https://github.com/jacobnb/EGP410_Assignment4/tree/Assignment4)

# To compile and link the project:
1. open EGP-410\GameAI\pathfinding\game\pathfinding.sln using Visual Studio
2. Compile using visual studio. either Build > Build Solution 
or click on the "Local Windows Debugger" button at the top of the screen
3. I haven't found any linking or targetting problems on the lab computers, 
but you may have to retarget projects for your windows SDK version

**You may also need to add the SDL dlls to the debug environment path.**
properties > Debugging > Environment: PATH=..\..\..\external libs\SDL2\SDL\lib\win32;%PATH%

# Keyboard Inputs
* Left Click = Pathfind to click location
* F = Swap to DFS pathfinder
* A = Swap to A* pathfinder
* D = Swap to Dijkstra pathfinder
* S = Create 10 units.
* Z = Change to Flowfield steering
* I = Change to Interruptable A* pathfinder.
* ESC = quit game


# Student Version
Code modified and accidentally obfuscated by Jacob B.

# EGP-410
Template projects for base code for EGP 410 assignments.

# This repository contains three major directories:
 * DeanLib - Base utility library provided by Dean Lawson. Memory tracking and basic vector mathematics utilities.
 * External Libs - Allegro 5.0.10 support libraries.
 * Game AI - Base source code for EGP 410 Assignments
   * pathfinding
   * state machine
   * steering


