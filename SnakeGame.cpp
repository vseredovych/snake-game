#include "SnakeGame.h"

SnakeGame::SnakeGame()
{
    SetupTerminal();
    InitializeBoard();
    InitializeGame();
    snake = new std::vector<Point>();
}

SnakeGame::~SnakeGame()
{
    delete snake;

    for (int i = 0; i < nBoardHeight; i++) {
        delete board[i];
    }
    delete board;
}

void SnakeGame::Start()
{
    GameLoop();
}

void SnakeGame::SetupTerminal()
{
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);
}

void SnakeGame::InitializeBoard()
{
    nBoardWidth = BOARD_WIDTH;
    nBoardHeight = BOARD_HEIGHT;

    board = new char*[nBoardHeight];

    for (int i = 0; i < nBoardHeight; i++)
    {
        board[i] = new char[nBoardWidth];
        for (int j = 0; j < nBoardWidth; j++)
        {
            if (i == nBoardHeight - 1 || i == 0)
            {
                board[i][j] = topBottomEdge;
                continue;
            }

            if (j == nBoardWidth - 1 || j == 0)
            {
                board[i][j] = leftRightEdge;
                continue;
            }

            board[i][j] = voidChar;
        }
    }
}

void SnakeGame::InitializeGame()
{
    snake->clear();
    snakeMoveDirection = Point(1, 0);
    snake->push_back(GetRandomBoardCoordinate());
    score = 0;
    SpawnApple();
}

void SnakeGame::GameLoop()
{
    int action;
    startTime = std::chrono::system_clock::now();

    do
    {
        // Sets the time to wait for the user input
        halfdelay(1);
        keypad(stdscr, true);
        action = getch();

        switch(action)
        {
            case MOVE_LEFT:
                snakeMoveDirection.x = 0;
                snakeMoveDirection.y = -1;
                break;
            case MOVE_RIGHT:
                snakeMoveDirection.x = 0;
                snakeMoveDirection.y = 1;
                break;
            case MOVE_UP:
                snakeMoveDirection.x = -1;
                snakeMoveDirection.y = 0;
                break;
            case MOVE_DOWN:
                snakeMoveDirection.x = 1;
                snakeMoveDirection.y = 0;
                break;
        }

        if (IsGameOver())
        {
            GameOver();
            InitializeGame();
        }

        if (IsAppleEaten()) {
            ExtendSnake();
            SpawnApple();
            score++;
        }

        MoveSnake(snakeMoveDirection);

        clear();
        Draw();
        refresh();
        CountFrameRate();

    } while (action != KEY_EXIT);
}

void SnakeGame::Draw()
{
    DrawBoard();
    DrawSnake();
    DrawApple();
    DrawScore();
    DrawFrameRate();
}

void SnakeGame::DrawBoard()
{
    for (int i = 0; i < nBoardHeight; i++)
    {
        for (int j = 0; j < nBoardWidth; j++)
        {
            mvprintw(i, j, &board[i][j]);
        }
    }
}

void SnakeGame::DrawApple()
{
    char appleString[2];
    snprintf(appleString, sizeof(appleString), "%c", appleChar);
    mvprintw(apple.x, apple.y, appleString);
}

void SnakeGame::DrawSnake()
{
    char headString[2];
    char bodyString[2];

    snprintf(headString, sizeof(headString), "%c", snakeHeadChar);
    snprintf(bodyString, sizeof(bodyString), "%c", snakeBodyChar);

    for (int i = 0; i < snake->size(); i++)
    {
        if (i == 0){
            mvprintw(snake->at(i).x, snake->at(i).y, headString);
        }
        else {
            mvprintw(snake->at(i).x, snake->at(i).y, bodyString);
        }
    }
}

void SnakeGame::DrawScore()
{
    char scoreMessage[10];
    snprintf(scoreMessage, sizeof(scoreMessage), "Score: %d", score);
    mvprintw(nBoardHeight, (nBoardWidth - sizeof(scoreMessage)) / 2, scoreMessage);
}

void SnakeGame::DrawFrameRate()
{
    char frameRateString[8];
    snprintf(frameRateString, sizeof(frameRateString), "FPS: %d", frameRate);
    mvprintw(nBoardHeight, nBoardWidth - sizeof(frameRateString), frameRateString);
}

void SnakeGame::SpawnApple()
{
    apple = GetRandomBoardCoordinate();
}

bool SnakeGame::IsAppleEaten()
{
    Point head = snake->front();
    if (head.x == apple.x && head.y == apple.y) {
        return true;
    }
    return false;
}

void SnakeGame::MoveSnake(Point direction)
{
    Point head = snake->front();
    Point *newHead = new Point(head.x + direction.x, head.y + direction.y);
    snake->insert(snake->begin(), *newHead);
    snake->pop_back();
}

void SnakeGame::ExtendSnake()
{
    Point tail = snake->back();
    Point *newTail = new Point(tail.x, tail.y);
    snake->push_back(*newTail);
}

bool SnakeGame::DoSnakeHitBorder()
{
    Point head = snake->front();
    if (head.x <= 0
        || head.x >= nBoardHeight - 1
        || head.y <= 0
        || head.y >= nBoardWidth - 1)
    {
        return true;
    }
    return false;
}

bool SnakeGame::DoSnakeHitSelf()
{
    Point head = snake->front();

    for (int i = 1; i < snake->size(); i++)
    {
        if (snake->at(i).x == head.x && snake->at(i).y == head.y){
            return true;
        }
    }
    return false;
}


bool SnakeGame::IsGameOver()
{
    return DoSnakeHitBorder() || DoSnakeHitSelf();
}

void SnakeGame::GameOver()
{
    char gameOverMessage[] = "Game Over, press Enter to restart, ESC to quit.";
    int action;

    mvprintw((int)nBoardHeight + 2, 2, gameOverMessage);

    while (true)
    {
        action = getch();

        if (action == START_GAME) {
            break;
        }

        if (action == QUIT_GAME){
            exit(0);
        }
    }
}

Point SnakeGame::GetRandomBoardCoordinate()
{
    Point p;
    p.x = rand() % (nBoardHeight - 2) + 1;
    p.y = rand() % (nBoardWidth - 2) + 1;
    return p;
}


void SnakeGame::CountFrameRate()
{
    endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    frameRate = (int)(frames / elapsed_seconds.count());
    frames++;

    if (elapsed_seconds.count() >= 1) {
        startTime = std::chrono::system_clock::now();
        frames = 0;
    }
}
