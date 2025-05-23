
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
