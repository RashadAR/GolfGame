# 2D Golf Game

A simple 2D golf game implemented in C++ using OpenGL. This game features a two-player mode where players take turns completing each hole, competing for the lowest score across multiple levels.

## Features

- Two-player turn-based gameplay
- Multiple levels with increasing difficulty
- Simple physics simulation for ball movement
- Obstacle avoidance
- Score tracking for each player
- Win condition based on the lowest total score

## Prerequisites

To build and run this game, you'll need the following libraries and tools:

- C++ compiler with C++11 support
- OpenGL
- GLFW3
- GLAD
- GLM
- GLUT

## Building the Game

1. Clone this repository:
https://github.com/RashadAR/GolfGame.git

2. Make sure you have all the required libraries installed on your system.

3. Compile the game using your preferred C++ compiler. For example, using g++:
   g++ -std=c++11 main.cpp -o golf_game -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl

   
Note: The exact compilation command may vary depending on your system and where the libraries are installed.

## Running the Game

After successful compilation, run the game using:
./golf_game

## How to Play

1. The game starts with Player 1 on the first level.
2. Click and drag the mouse to aim, then release to shoot the ball.
3. Try to get the ball into the hole with as few strokes as possible.
4. Once a player completes the hole, it's the next player's turn.
5. After both players complete a level, the game moves to the next level.
6. The player with the lowest total score across all levels wins the game.

## Controls

- Left Mouse Button: Aim and shoot
- ESC: Exit the game
