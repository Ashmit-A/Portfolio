#include <stdio.h>
#include <conio.h>  // For _getch() and _kbhit()
#include <windows.h> // For Sleep()

#define WIDTH 20
#define HEIGHT 10
#define INITIAL_SIZE 1

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position body[WIDTH * HEIGHT];
    int size;
    Position food;
    char direction;
    int score;
    int gameOver;
} SnakeGame;

void initializeGame(SnakeGame *game);
void drawGame(SnakeGame *game);
void updateGame(SnakeGame *game);
void handleInput(SnakeGame *game);
void generateFood(SnakeGame *game);
int checkCollision(SnakeGame *game);
void getScore(SnakeGame *game);

int main() {
    SnakeGame game;
    char choice;

    initializeGame(&game);

    while (!game.gameOver) {
        if (_kbhit()) {
            choice = _getch();
            if (choice == 'q') break;
            handleInput(&game);
        }

        updateGame(&game);
        drawGame(&game);
        Sleep(100);
    }

    getScore(&game);
    return 0;
}

void initializeGame(SnakeGame *game) {
    game->size = INITIAL_SIZE;
    game->body[0].x = WIDTH / 2;
    game->body[0].y = HEIGHT / 2;
    game->direction = 'R';
    game->score = 0;
    game->gameOver = 0;
    generateFood(game);
}

void drawGame(SnakeGame *game) {
    system("cls");

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                printf("#");
            } else if (x == game->food.x && y == game->food.y) {
                printf("*");
            } else {
                int isSnakePart = 0;
                for (int i = 0; i < game->size; i++) {
                    if (game->body[i].x == x && game->body[i].y == y) {
                        printf("O");
                        isSnakePart = 1;
                        break;
                    }
                }
                if (!isSnakePart) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

void updateGame(SnakeGame *game) {
    Position prev = game->body[0];
    Position prev2;
    Position head = prev;

    switch (game->direction) {
        case 'U': head.y--; break;
        case 'D': head.y++; break;
        case 'L': head.x--; break;
        case 'R': head.x++; break;
    }

    if (head.x == game->food.x && head.y == game->food.y) {
        game->score += 10;
        game->size++;
        generateFood(game);
    }

    for (int i = 0; i < game->size; i++) {
        prev2 = game->body[i];
        game->body[i] = prev;
        prev = prev2;
    }
    game->body[0] = head;

    if (checkCollision(game)) {
        game->gameOver = 1;
    }
}

void handleInput(SnakeGame *game) {
    char input = _getch();
    switch (input) {
        case 'w': if (game->direction != 'D') game->direction = 'U'; break;
        case 's': if (game->direction != 'U') game->direction = 'D'; break;
        case 'a': if (game->direction != 'R') game->direction = 'L'; break;
        case 'd': if (game->direction != 'L') game->direction = 'R'; break;
    }
}

void generateFood(SnakeGame *game) {
    game->food.x = rand() % (WIDTH - 2) + 1;
    game->food.y = rand() % (HEIGHT - 2) + 1;
}

int checkCollision(SnakeGame *game) {
    Position head = game->body[0];

    if (head.x == 0 || head.x == WIDTH - 1 || head.y == 0 || head.y == HEIGHT - 1) {
        return 1;
    }

    for (int i = 1; i < game->size; i++) {
        if (game->body[i].x == head.x && game->body[i].y == head.y) {
            return 1;
        }
    }

    return 0;
}

void getScore(SnakeGame *game) {
    printf("Game Over! Your score: %d\n", game->score);
}
