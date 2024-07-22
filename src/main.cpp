#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <string>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct Level {
    glm::vec2 ballStart;
    glm::vec2 holePosition;
    std::vector<glm::vec2> obstacles;
};

// Golf ball properties
glm::vec2 ballPosition(400.0f, 300.0f);
glm::vec2 ballVelocity(0.0f, 0.0f);
float ballRadius = 10.0f;

// Golf course properties
std::vector<glm::vec2> obstacles;
glm::vec2 holePosition(700.0f, 300.0f);
float holeRadius = 20.0f;

// Game state
bool isAiming = false;
glm::vec2 aimStart;
float power = 0.0f;
int currentPlayer = 0;
int strokes[2] = { 0, 0 };
bool playerFinished[2] = { false, false };
bool levelCompleted = false;

// Level system
std::vector<Level> levels;
int currentLevel = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void loadLevel(int levelIndex) {
    if (levelIndex < levels.size()) {
        const Level& level = levels[levelIndex];
        ballPosition = level.ballStart;
        holePosition = level.holePosition;
        obstacles = level.obstacles;
        ballVelocity = glm::vec2(0.0f, 0.0f);
        strokes[0] = strokes[1] = 0;
        playerFinished[0] = playerFinished[1] = false;
        currentPlayer = 0;
        levelCompleted = false;
    }
    else {
        std::cout << "All levels completed!" << std::endl;
        if (strokes[0] < strokes[1]) {
            std::cout << "Player 1 wins with " << strokes[0] << " total strokes!" << std::endl;
        }
        else if (strokes[1] < strokes[0]) {
            std::cout << "Player 2 wins with " << strokes[1] << " total strokes!" << std::endl;
        }
        else {
            std::cout << "It's a tie!" << std::endl;
        }
        glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
    }
}

void switchPlayer() {
    currentPlayer = 1 - currentPlayer;
    if (!playerFinished[currentPlayer]) {
        ballPosition = levels[currentLevel].ballStart;
        ballVelocity = glm::vec2(0.0f, 0.0f);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (levelCompleted) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            currentLevel++;
            loadLevel(currentLevel);
        }
    }
    else if (!playerFinished[currentPlayer]) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS && glm::length(ballVelocity) < 1.0f) {
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                aimStart = glm::vec2(xpos, ypos);
                isAiming = true;
            }
            else if (action == GLFW_RELEASE && isAiming) {
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                glm::vec2 aimEnd(xpos, ypos);
                glm::vec2 aimDirection = aimStart - aimEnd;
                power = glm::length(aimDirection) * 1.5f;
                ballVelocity = glm::normalize(aimDirection) * power;
                isAiming = false;
                strokes[currentPlayer]++;
            }
        }
    }
}

void drawCircle(float cx, float cy, float radius, int num_segments) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void drawLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void update(float deltaTime) {
    if (!levelCompleted && !playerFinished[currentPlayer]) {
        ballPosition += ballVelocity * deltaTime;
        ballVelocity *= 0.995f;

        if (ballPosition.x - ballRadius < 0 || ballPosition.x + ballRadius > SCR_WIDTH) {
            ballVelocity.x = -ballVelocity.x * 0.9f;
            ballPosition.x = glm::clamp(ballPosition.x, ballRadius, SCR_WIDTH - ballRadius);
        }
        if (ballPosition.y - ballRadius < 0 || ballPosition.y + ballRadius > SCR_HEIGHT) {
            ballVelocity.y = -ballVelocity.y * 0.9f;
            ballPosition.y = glm::clamp(ballPosition.y, ballRadius, SCR_HEIGHT - ballRadius);
        }

        for (const auto& obstacle : obstacles) {
            glm::vec2 diff = ballPosition - obstacle;
            float distance = glm::length(diff);
            if (distance < ballRadius + 20.0f) {
                glm::vec2 normal = glm::normalize(diff);
                ballVelocity = glm::reflect(ballVelocity, normal) * 0.9f;
                ballPosition = obstacle + normal * (ballRadius + 20.0f);
            }
        }

        float distanceToHole = glm::distance(ballPosition, holePosition);
        if (distanceToHole < holeRadius) {
            playerFinished[currentPlayer] = true;
            std::cout << "Player " << (currentPlayer + 1) << " finished in " << strokes[currentPlayer] << " strokes!" << std::endl;

            if (playerFinished[0] && playerFinished[1]) {
                levelCompleted = true;
                std::cout << "Level " << (currentLevel + 1) << " completed!" << std::endl;
                std::cout << "Player 1: " << strokes[0] << " strokes, Player 2: " << strokes[1] << " strokes" << std::endl;
                if (currentLevel < levels.size() - 1) {
                    std::cout << "Click to proceed to the next level." << std::endl;
                }
                else {
                    std::cout << "Game Over!" << std::endl;
                    if (strokes[0] < strokes[1]) {
                        std::cout << "Player 1 wins with " << strokes[0] << " total strokes!" << std::endl;
                    }
                    else if (strokes[1] < strokes[0]) {
                        std::cout << "Player 2 wins with " << strokes[1] << " total strokes!" << std::endl;
                    }
                    else {
                        std::cout << "It's a tie!" << std::endl;
                    }
                    std::cout << "Click to exit the game." << std::endl;
                }
            }
            else {
                switchPlayer();
            }
        }

        if (glm::length(ballVelocity) < 1.0f) {
            ballVelocity = glm::vec2(0.0f, 0.0f);
        }
    }
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.8f, 0.0f);
    glRectf(0, 0, SCR_WIDTH, SCR_HEIGHT);

    glColor3f(0.5f, 0.5f, 0.5f);
    for (const auto& obstacle : obstacles) {
        drawCircle(obstacle.x, obstacle.y, 20.0f, 32);
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(holePosition.x, holePosition.y, holeRadius, 32);

    if (!playerFinished[currentPlayer]) {
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCircle(ballPosition.x, ballPosition.y, ballRadius, 32);

        if (isAiming) {
            double xpos, ypos;
            glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
            glColor3f(1.0f, 0.0f, 0.0f);
            drawLine(ballPosition.x, ballPosition.y, xpos, ypos);
        }
    }

    // Draw level and player information
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(10, 20);
    std::string levelText = "Level: " + std::to_string(currentLevel + 1);
    for (char c : levelText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2f(10, 40);
    std::string playerText = "Current Player: " + std::to_string(currentPlayer + 1);
    for (char c : playerText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2f(10, 60);
    std::string scoreText = "P1: " + std::to_string(strokes[0]) + " | P2: " + std::to_string(strokes[1]);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void setupLevels() {
    // Level 1
    levels.push_back({
        glm::vec2(100.0f, 300.0f),
        glm::vec2(700.0f, 300.0f),
        {glm::vec2(400.0f, 300.0f)}
        });

    // Level 2
    levels.push_back({
        glm::vec2(100.0f, 100.0f),
        glm::vec2(700.0f, 500.0f),
        {glm::vec2(300.0f, 300.0f), glm::vec2(500.0f, 300.0f)}
        });

    // Level 3
    levels.push_back({
        glm::vec2(400.0f, 50.0f),
        glm::vec2(400.0f, 550.0f),
        {glm::vec2(200.0f, 200.0f), glm::vec2(600.0f, 200.0f), glm::vec2(400.0f, 400.0f)}
        });

    // Add more levels as needed
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2D Golf Game - 2 Players", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, SCR_WIDTH, SCR_HEIGHT, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    setupLevels();
    loadLevel(currentLevel);

    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        update(deltaTime);
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}