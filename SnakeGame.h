//
// Created by vseredov on 04.07.2021.
//

#ifndef SNAKE_SNAKEGAME_H
#define SNAKE_SNAKEGAME_H

#include <iostream>
#include <ncurses.h>
#include <vector>
#include <chrono>

#define BOARD_WIDTH 30
#define BOARD_HEIGHT 20
#define VOID_CHAR ' '
#define LEFT_RIGHT_EDGE_CHAR '|'
#define TOP_BOTTOM_EDGE_CHAR '-'
#define SNAKE_BODY_CHAR '0'
#define SNAKE_HEAD_CHAR 'O'
#define APPLE_CHAR '@'

// ASCII keys for the Snake game
enum SNAKE_GAME_KEYS {
    MOVE_LEFT=97,   // A
    MOVE_RIGHT=100, // D
    MOVE_UP=119,    // W
    MOVE_DOWN=115,  // S
    START_GAME=10,  // Enter
    QUIT_GAME=27,   // Esc
};

struct Point {
public:
    Point(int x, int y){
        this->x = x;
        this->y = y;
    }
    Point() : Point(0, 0) {}
    int x;
    int y;
};

class SnakeGame {
public:
    SnakeGame();
    ~SnakeGame();
    void Start();

private:
    int nBoardHeight = BOARD_HEIGHT;
    int nBoardWidth = BOARD_WIDTH;
    char leftRightEdge = LEFT_RIGHT_EDGE_CHAR;
    char topBottomEdge = TOP_BOTTOM_EDGE_CHAR;
    char snakeBodyChar = SNAKE_BODY_CHAR;
    char snakeHeadChar = SNAKE_HEAD_CHAR;
    char appleChar = APPLE_CHAR;
    char voidChar = VOID_CHAR;

    char **board;
    std::vector<Point> *snake;
    Point apple;
    Point snakeMoveDirection;

    int score = 0;
    int frames = 0;
    int frameRate = 0;
    std::chrono::time_point<std::chrono::system_clock> startTime;
    std::chrono::time_point<std::chrono::system_clock> endTime;

    void SetupTerminal();

    void InitializeBoard();

    void InitializeGame();

    void GameLoop();

    void Draw();

    void DrawBoard();

    void DrawSnake();

    void DrawApple();

    void DrawScore();

    void DrawFrameRate();

    void SpawnApple();

    bool IsAppleEaten();

    void ExtendSnake();

    void MoveSnake(Point direction);

    bool DoSnakeHitBorder();

    bool DoSnakeHitSelf();

    bool IsGameOver();

    void GameOver();

    void CountFrameRate();

    Point GetRandomBoardCoordinate();
};

#endif //SNAKE_SNAKEGAME_H
