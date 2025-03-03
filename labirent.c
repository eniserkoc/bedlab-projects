#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 12

int dx[] = {0, 0, -1, 1};
int dy[] = {-1, 1, 0, 0};

void initializeMaze(char maze[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            maze[i][j] = '#';
        }
    }
}

void printMaze(char maze[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }
}

int isValid(int x, int y) {
    return x > 0 && x < SIZE - 1 && y > 0 && y < SIZE - 1;
}

void generateMaze(char maze[SIZE][SIZE], int x, int y) {
    maze[x][y] = '.';
    int directions[] = {0, 1, 2, 3};
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temp = directions[i];
        directions[i] = directions[r];
        directions[r] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[directions[i]] * 2;
        int ny = y + dy[directions[i]] * 2;

        if (isValid(nx, ny) && maze[nx][ny] == '#') {
            maze[x + dx[directions[i]]][y + dy[directions[i]]] = '.';

            generateMaze(maze, nx, ny);
        }
    }
}

int main() {
    srand(time(NULL));

    char maze[SIZE][SIZE];
    initializeMaze(maze);

    generateMaze(maze, 1, 1);

    maze[1][0] = '.';
    maze[SIZE - 3][SIZE - 1] = '.';
    maze[SIZE - 3][SIZE - 2] = '.';
    printf("Olusturulan Labirent:\n");
    printMaze(maze);

    return 0;
}
