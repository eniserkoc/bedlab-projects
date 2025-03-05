#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 25

#define PLATFORM_WIDTH 10
#define MIN_VERTICAL_GAP 3
#define MAX_VERTICAL_GAP 6

#define SCROLL_THRESHOLD 10
#define WALL_THICKNESS 1

typedef struct {
    float x,y;
    int width;
    int hasScore;
} Platform;

typedef struct PlatformNode {
    Platform p;
    struct PlatformNode *next;
} PlatformNode;

PlatformNode *platformList = NULL;
float highestPlatformY = 0;
float cameraOffset = 0;
int score = 0;
int jumpLock = 0;

typedef struct {
    float x,y;
    float vx,vy;
} Player;

typedef struct WallNode {
    float y;
    int side;
    struct WallNode *next;
} WallNode;

WallNode *wallList = NULL;
float highestWallYForWalls = 0;

void gotoXY(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

Platform generatePlatform(float y){
    Platform p;
    p.width = PLATFORM_WIDTH;
    p.x = rand() % (CONSOLE_WIDTH - PLATFORM_WIDTH);
    p.y = y;
    p.hasScore = (rand() % 100 < 30) ? 1 : 0;
    return p;
}

void addPlatformNode(Platform p) {
    PlatformNode *newNode = (PlatformNode *)malloc(sizeof(PlatformNode));
    newNode->p = p;
    newNode->next = NULL;
    if(platformList == NULL) {
        platformList = newNode;
    } else {
        PlatformNode *current = platformList;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void initPlatforms() {
    while(platformList != NULL) {
        PlatformNode *temp = platformList;
        platformList = platformList->next;
        free(temp);
    }
    platformList = NULL;

    Platform ground;
    ground.x = 0;
    ground.y = 0;
    ground.width = CONSOLE_WIDTH;
    ground.hasScore = 0;
    addPlatformNode(ground);
    highestPlatformY = 0;

    float y = MIN_VERTICAL_GAP;
    while (y < CONSOLE_HEIGHT * 2) {
        Platform p = generatePlatform(y);
        addPlatformNode(p);
        if (p.y > highestPlatformY)
            highestPlatformY = p.y;
        y += MIN_VERTICAL_GAP + rand() % (MAX_VERTICAL_GAP - MIN_VERTICAL_GAP + 1);
    }
}

void updatePlatforms() {
    PlatformNode **current = &platformList;
    while(*current != NULL) {
        if((*current)->p.y < cameraOffset - 1) {
            PlatformNode *temp = *current;
            *current = (*current)->next;
            free(temp);
        } else {
            current = &((*current)->next);
        }
    }
    while (highestPlatformY < cameraOffset + CONSOLE_HEIGHT) {
        float newY = highestPlatformY + MIN_VERTICAL_GAP + rand() % (MAX_VERTICAL_GAP - MIN_VERTICAL_GAP + 1);
        Platform p = generatePlatform(newY);
        addPlatformNode(p);
        if (p.y > highestPlatformY)
            highestPlatformY = p.y;
    }
}

void addWallSegment(int side, float y) {
    WallNode *newNode = (WallNode*) malloc(sizeof(WallNode));
    newNode->y = y;
    newNode->side = side;
    newNode->next = NULL;
    if(wallList == NULL) {
        wallList = newNode;
    } else {
        WallNode *current = wallList;
        while (current->next != NULL)
            current = current->next;
        current->next = newNode;
    }
}

void initWalls() {
    while(wallList != NULL) {
        WallNode *temp = wallList;
        wallList = wallList->next;
        free(temp);
    }
    wallList = NULL;
    highestWallYForWalls = 0;
    for (int y = 0; y < CONSOLE_HEIGHT; y++) {
        addWallSegment(0, y);
        addWallSegment(1, y);
        if (y > highestWallYForWalls)
            highestWallYForWalls = y;
    }
}

void updateWalls() {
    WallNode **current = &wallList;
    while(*current != NULL) {
        if((*current)->y < cameraOffset -1) {
            WallNode *temp = *current;
            *current = (*current)->next;
            free(temp);
        } else {
            current = &((*current)->next);
        }
    }
    while (highestWallYForWalls < cameraOffset + CONSOLE_HEIGHT) {
        float newY = highestWallYForWalls + 1;
        addWallSegment(0,newY);
        addWallSegment(1,newY);
        highestWallYForWalls = newY;
    }
}

void updatePlayer(Player *player) {
    const float GRAVITY = -0.5f;
    float prevY = player->y;
    player->vy += GRAVITY;
    player->x += player->vx;
    player->y += player->vy;

    if (player->x < 1) {
        player->x = 1;
        player->vx = 0;
    }
    if (player->x > CONSOLE_WIDTH - 2) {
        player->x = CONSOLE_WIDTH -2;
        player->vx = 0;
    }

    if (player->vy < 0 && jumpLock == 0) {
        PlatformNode *current = platformList;
        while (current) {
            Platform p = current->p;
            float platformTop = p.y + 1;
            if (player->x >= p.x && player->x <= p.x +p.width) {
                if (prevY >= platformTop && player->y < platformTop) {
                    player->y = platformTop;
                    player->vy = 0;
                    if (p.hasScore) {
                        score += 10;
                        current->p.hasScore = 0;
                    }
                }
            }
            current = current->next;
        }
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

    WallNode *wNode = wallList;
    while(wNode) {
        int wallScreenY = CONSOLE_HEIGHT - 1 - (int)(wNode->y - cameraOffset);
        if (wallScreenY >= 0 && wallScreenY < CONSOLE_HEIGHT) {
            int x;
            if (wNode->side == 0) {
                x = 0;
            } else {
                x = CONSOLE_WIDTH - 1;
            }
            screen[wallScreenY][x] = '#';
        }
        wNode = wNode->next;
    }

    PlatformNode *current = platformList;
    while (current) {
        Platform p = current->p;
        int platformScreenY = CONSOLE_HEIGHT - 1 - (int)(p.y - cameraOffset);
        if (platformScreenY >= 0 && platformScreenY < CONSOLE_HEIGHT) {
            for (int x = (int)p.x; x < (int)p.x + p.width && x < CONSOLE_WIDTH; x++) {
                screen[platformScreenY][x] = '=';
            }
            if (p.hasScore && platformScreenY - 1 >= 0) {
                int mid = (int)p.x + p.width / 2;
                if (mid < CONSOLE_WIDTH)
                    screen[platformScreenY - 1][mid] = '*';
            }
        }
        current = current->next;
    }

    int playerScreenX = (int)player.x;
    int playerScreenY = CONSOLE_HEIGHT - 1 - (int)(player.y - cameraOffset);
    if (playerScreenY >= 0 && playerScreenY < CONSOLE_HEIGHT &&
        playerScreenX >= 0 && playerScreenX < CONSOLE_WIDTH) {
        screen[playerScreenY][playerScreenX] = 'O';
    }
    gotoXY(0,0);
    for (int i = 0; i < CONSOLE_HEIGHT; i++) {
        printf("%s\n",screen[i]);
    }
    printf("Score: %d\n",score);
}

int main() {
    srand(time(NULL));
    initPlatforms();
    initWalls();

    Player player;
    player.x = CONSOLE_WIDTH / 2;
    player.y = 1;
    player.vx = 0;
    player.vy = 0;

    cameraOffset = 0;
    int gameOver = 0;

    while(!gameOver) {
        if(kbhit()) {
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

        if (player.y < cameraOffset - 1) {
            gameOver = 1;
            break;
        }

        updatePlatforms();
        updateWalls();
        renderGame(player);
        if (jumpLock > 0)
            jumpLock--;

        Sleep(1);
    }

    gotoXY(0,CONSOLE_HEIGHT + 1);
    printf("Game Over! Score: %d\n",score);
    getch();

    while(platformList != NULL) {
        PlatformNode *temp = platformList;
        platformList = platformList->next;
        free(temp);
    }

    while(wallList != NULL) {
        WallNode *temp = wallList;
        wallList = wallList->next;
        free(temp);
    }

    return 0;
}