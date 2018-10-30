[Github Commit](https://github.com/jacobnb/EGP-410/commit/18f5cf95af528e2b427c2c071537549722ded34c)
[Github Tag](https://github.com/jacobnb/EGP-410/tree/Assignment3)

# To compile and link the project:
1. open EGP-410\GameAI\pathfinding\game\pathfinding.sln using Visual Studio
2. Compile using visual studio. either Build > Build Solution 
or click on the "Local Windows Debugger" button at the top of the screen
3. I haven't found any linking or targetting problems on the lab computers, 
but you may have to retarget projects for your windows SDK version

# Keyboard Inputs
* Left Click = Pathfind to click location
* F = Swap to DFS pathfinder
* A = Swap to A* pathfinder
* D = Swap to Dijkstra pathfinder
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

Building instructions are left up to the student. 

