#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 25

#define MAX_PLATFORMS 100
#define PLATFORM_WIDTH 10
#define MIN_VERTICAL_GAP 3
#define MAX_VERTICAL_GAP 6

#define SCROLL_THRESHOLD 10

typedef struct {
    float x, y;
    float vx, vy;
} Player;

typedef struct {
    float x, y;
    int width;
    int hasScore;
} Platform;

Platform platforms[MAX_PLATFORMS];
int platformCount = 0;
float highestPlatformY = 0;
float cameraOffset = 0;
int score = 0;

int jumpLock = 0;

void gotoXY(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

Platform generatePlatform(float y) {
    Platform p;
    p.width = PLATFORM_WIDTH;
    p.x = rand() % (CONSOLE_WIDTH - PLATFORM_WIDTH);
    p.y = y;
    p.hasScore = (rand() % 100 < 20) ? 1 : 0;
    return p;
}
void initPlatforms() {
    platformCount = 0;

    platforms[platformCount].x = 0;
    platforms[platformCount].y = 0;
    platforms[platformCount].width = CONSOLE_WIDTH;
    platforms[platformCount].hasScore = 0;
    platformCount++;
    highestPlatformY = 0;

    float y = MIN_VERTICAL_GAP;
    while (y < CONSOLE_HEIGHT * 2 && platformCount < MAX_PLATFORMS) {
        Platform p = generatePlatform(y);
        platforms[platformCount++] = p;
        if (p.y > highestPlatformY)
            highestPlatformY = p.y;
        y += MIN_VERTICAL_GAP + rand() % (MAX_VERTICAL_GAP - MIN_VERTICAL_GAP + 1);
    }
}

void updatePlayer(Player *player) {
    const float GRAVITY = -0.5f;
    float prevY = player->y;
    player->vy += GRAVITY;
    player->x += player->vx;
    player->y += player->vy;

    if (player->x < 0) {
        player->x = 0;
        player->vx = 0;
    }
    if (player->x > CONSOLE_WIDTH - 1) {
        player->x = CONSOLE_WIDTH - 1;
        player->vx = 0;
    }

    if (player->vy < 0 && jumpLock == 0) {
        for (int i = 0; i < platformCount; i++) {
            Platform p = platforms[i];
            float platformTop = p.y + 1;

            if (player->x >= p.x && player->x <= p.x + p.width) {
                if (prevY >= platformTop && player->y < platformTop) {
                    player->y = platformTop;
                    player->vy = 0;
                    if (p.hasScore) {
                        score += 10;
                        platforms[i].hasScore = 0;
                    }
                }
            }
        }
    }
}

void updatePlatforms() {
    for (int i = 0; i < platformCount; i++) {
        if (platforms[i].y < cameraOffset - 1) {
            platforms[i] = platforms[platformCount - 1];
            platformCount--;
            i--;
        }
    }
    while (highestPlatformY < cameraOffset + CONSOLE_HEIGHT) {
        float newY = highestPlatformY + MIN_VERTICAL_GAP + rand() % (MAX_VERTICAL_GAP - MIN_VERTICAL_GAP + 1);
        Platform p = generatePlatform(newY);
        platforms[platformCount++] = p;
        if (p.y > highestPlatformY)
            highestPlatformY = p.y;
    }
}

void renderGame(Player player) {
    char screen[CONSOLE_HEIGHT][CONSOLE_WIDTH + 1];
    for (int i = 0; i < CONSOLE_HEIGHT; i++) {
        for (int j = 0; j < CONSOLE_WIDTH; j++) {
            screen[i][j] = ' ';
        }
        screen[i][CONSOLE_WIDTH] = '\0';
    }
    for (int i = 0; i < platformCount; i++) {
        int platformScreenY = CONSOLE_HEIGHT - 1 - (int)(platforms[i].y - cameraOffset);
        if (platformScreenY >= 0 && platformScreenY < CONSOLE_HEIGHT) {
            for (int x = (int)platforms[i].x; x < (int)platforms[i].x + platforms[i].width && x < CONSOLE_WIDTH; x++) {
                screen[platformScreenY][x] = '=';
            }
            if (platforms[i].hasScore && platformScreenY - 1 >= 0) {
                int mid = (int)platforms[i].x + platforms[i].width / 2;
                if (mid < CONSOLE_WIDTH)
                    screen[platformScreenY - 1][mid] = '*';
            }
        }
    }

    int playerScreenX = (int)player.x;
    int playerScreenY = CONSOLE_HEIGHT - 1 - (int)(player.y - cameraOffset);
    if (playerScreenY >= 0 && playerScreenY < CONSOLE_HEIGHT &&
        playerScreenX >= 0 && playerScreenX < CONSOLE_WIDTH) {
        screen[playerScreenY][playerScreenX] = 'O';
    }

    gotoXY(0, 0);
    for (int i = 0; i < CONSOLE_HEIGHT; i++) {
        printf("%s\n", screen[i]);
    }
    printf("Score: %d\n", score);
}

int main() {
    srand(time(NULL));
    initPlatforms();

    Player player;
    player.x = CONSOLE_WIDTH / 2;
    player.y = 1;
    player.vx = 0;
    player.vy = 0;

    cameraOffset = 0;
    int gameOver = 0;

    while (!gameOver) {
        if (kbhit()) {
            char ch = getch();
            if (ch == 'a')
                player.vx = -2.5f;
            if (ch == 'd')
                player.vx = 2.5f;
            if (ch == 'w' && player.vy == 0) {
                player.vy = 5;
                jumpLock = 5;
            }
            if (ch == 'q')
                break;
        } else {
            player.vx *= 0.9f;
        }

        updatePlayer(&player);
        if (player.y - cameraOffset > SCROLL_THRESHOLD) {
            cameraOffset = player.y - SCROLL_THRESHOLD;
        }

        if(player.y < cameraOffset - 1){
            gameOver = 1;
            break;
        }

        updatePlatforms();
        renderGame(player);
        if (jumpLock > 0)
            jumpLock--;

        Sleep(16);
    }//

    gotoXY(0, CONSOLE_HEIGHT + 1);
    printf("Game Over! Score: %d\n",score);
    getch();

    return 0;
}