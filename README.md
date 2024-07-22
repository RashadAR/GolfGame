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

To build and run this game, you'll need:

- Visual Studio 2022 with C++ development workload
- vcpkg package manager

## Setting Up the Project

1. Clone this repository: https://github.com/RashadAR/GolfGame.git

   cd GolfGame
3. Install the required packages using vcpkg:

   vcpkg install glfw3:x64-windows
   
   vcpkg install glad:x64-windows
   
   vcpkg install glm:x64-windows
   
   vcpkg install freeglut:x64-windows
5. Integrate vcpkg with Visual Studio:
   
   vcpkg integrate install

## Building the Game

1. Open the project in Visual Studio 2022.
2. Set the build configuration to Release and the platform to x64.
3. Build the solution (Ctrl + Shift + B).

## Running the Game

After successful compilation, you can run the game directly from Visual Studio by pressing F5 or clicking the "Start Debugging" button.

Alternatively, you can navigate to the output directory (usually `x64/Release/`) and run the executable directly.

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
