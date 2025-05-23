
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAP_HEIGHT 24
#define MAP_WIDTH 72

#define COLOR_WALL GREEN
#define COLOR_FLOOR GREEN
#define COLOR_BULLET YELLOW
#define MAX_BULLETS 8
#define ENEMY_SPAWN_INTERVAL 30
#define GAME_DURATION 60 * 75

struct Position {
    int x, y;
};

struct Lena {
    struct Position coords;
    int health;
    int ammo;
};

struct Enemy {
    struct Position coords;
    int health;
    int onScreen;
};

struct Bullet {
    struct Position coords;
    int direction;
    int onScreen;
};

void screenDrawMap() {
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++) {
            screenGotoxy(x,y);
            if (map[y][x]=='#') {
                screenSetColor(COLOR_WALL, BLACK);
                printf("▓");
            } else {
                screenSetColor(COLOR_FLOOR, BLACK);
                printf(" ");
            }
        }
    screenSetColor(WHITE, BLACK);
    fflush(stdout);
}

void initLena(struct Lena *lena) {
    lena->coords.x = MAP_WIDTH/2;
    lena->coords.y = MAP_HEIGHT-2;
    lena->health   = 10;
    lena->ammo     = MAX_BULLETS;
}

void drawLena(int x, int y) {
    screenGotoxy(x,y);
    printf("👩‍🚀");
}

void initEnemy(struct Enemy *e) {
    e->coords.x = rand() % (MAP_WIDTH-2) + 1;
    e->coords.y = 1;
    e->health   = 1;
    e->onScreen = 1;
}

void updateEnemy(struct Enemy *e, struct Lena *lena) {
    e->coords.y++;
    if (e->coords.y > lena->coords.y) {
        e->onScreen = 0;
    }
}

void drawEnemy(int x, int y) {
    screenGotoxy(x,y);
    printf("👾");
}

void spawnEnemy(struct Lena *lena, int frameCount, double elapsedTime) {
    if (frameCount % ENEMY_SPAWN_INTERVAL == 0) {
        for (int i = 0; i < enemyCapacity; i++) {
            if (!enemies[i].onScreen) {
                initEnemy(&enemies[i]);
                break;
            }
        }
    }
}

int main() {
    struct Lena lena; initLena(&lena);
    bullets = NULL; enemies = malloc(enemyCapacity*sizeof(*enemies));
    for (int i=0;i<enemyCapacity;i++) enemies[i].onScreen=0;

    while (1) { 
        screenDrawMap();
        drawLena(lena.coords.x, lena.coords.y);

        spawnEnemy(&lena, frameCount, elapsedTime);
        for (int i=0;i<enemyCapacity;i++)
            if (enemies[i].onScreen) {
                updateEnemy(&enemies[i], &lena);
                drawEnemy(enemies[i].coords.x, enemies[i].coords.y);
            }

        if (keyhit()) {
            int key = readch();
            if (key=='a' && !isWall(lena.coords.x-1, lena.coords.y))
                lena.coords.x--;
            else if (key=='d' && !isWall(lena.coords.x+1, lena.coords.y))
                lena.coords.x++;
        }

        screenUpdate();
    }
    return 0;
}