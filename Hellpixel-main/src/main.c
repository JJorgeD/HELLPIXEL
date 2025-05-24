#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_HEIGHT 24
#define MAP_WIDTH 72

#define COLOR_WALL GREEN
#define COLOR_FLOOR GREEN
#define COLOR_BULLET YELLOW
#define MAX_BULLETS 100
#define ENEMY_SPAWN_INTERVAL 30
#define GAME_DURATION 60 * 75

struct Position { int x, y; };
struct Lena      { struct Position coords; int health; int ammo; };
struct Enemy     { struct Position coords; int health; int onScreen; };
struct Bullet    { struct Position coords; int onScreen; int direction; };

struct Bullet *bullets;
struct Enemy  *enemies;
int enemyCapacity = 15;
int score = 0;

void screenDrawMap();
void initLena(struct Lena *lena);
void drawLena(int x, int y);
void initEnemy(struct Enemy *enemy);
void updateEnemy(struct Enemy *enemy, struct Lena *lena);
void spawnEnemy(struct Lena *lena, int frameCount, double elapsedTime);
void initBullet(struct Bullet *bullet, struct Lena *lena);
int checkCollision(int x1, int y1, int x2, int y2);
void updateBullet(struct Bullet *bullet);
int isWall(int x, int y);
void showStartArt();
void showInstructions();
void showVictory();
void showGameOver();

char map[MAP_HEIGHT][MAP_WIDTH] = {
    "############################ ###########################",
    "############################ ###########################",
    "### ##",
    "### ##",
    "### ##",
    "### ##",
    "### ##",
    "### ##",
    "### ##",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    "### ##",
    "### ##",
    "### ##",
    "### ##",
    "### ##",
    "### ##",
    "### ##",
    "############################ ###########################",

};

void screenDrawMap() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            char cell = map[y][x];
            screenGotoxy(x, y);

            switch (cell) {
                case '#':
                    screenSetColor(COLOR_WALL, BLACK);
                    printf("▓");
                    break;
                default:
                    screenSetColor(COLOR_FLOOR, BLACK);
                    printf(" ");
                    break;
            }
        }
    }
    screenSetColor(WHITE, BLACK);
    fflush(stdout);
}

void initLena(struct Lena *lena) {
    lena->coords.x = MAP_WIDTH / 2;
    lena->coords.y = MAP_HEIGHT - 2;
    lena->health   = 10;
    lena->ammo     = MAX_BULLETS;
}

void drawLena(int x, int y) {
    screenGotoxy(x, y);
    printf("👩‍🚀");
}

void initEnemy(struct Enemy *enemy) {
    enemy->coords.x = rand() % (MAP_WIDTH - 2) + 1;
    enemy->coords.y = 1;
    enemy->health = 1;
}

void updateEnemy(struct Enemy *enemy, struct Lena *lena) {
    enemy->coords.y++;
    if (enemy->coords.y > lena->coords.y) {
        enemy->onScreen = 0;
    }
}

void drawEnemy(int x, int y) {
    screenGotoxy(x, y);
    printf("👾");
}

void spawnEnemy(struct Lena *lena, int frameCount, double elapsedTime) {
    if (frameCount % ENEMY_SPAWN_INTERVAL == 0) {
        for (int i = 0; i < enemyCapacity; i++) {
            if (!enemies[i].onScreen) {
                initEnemy(&enemies[i]);
                enemies[i].onScreen = 1;
                break;
            }
        }
    }
}

void initBullet(struct Bullet *bullet, struct Lena *lena) {
    bullet->coords.x = lena->coords.x;
    bullet->coords.y = lena->coords.y - 1;
    bullet->onScreen = 1;
    bullet->direction = 0;
}

void drawBullet(int x, int y) {
    screenSetColor(COLOR_BULLET, BLACK);
    screenGotoxy(x, y);
    printf("•");
}

int checkCollision(int x1, int y1, int x2, int y2) {
    return x1 == x2 && y1 == y2;
}

void updateBullet(struct Bullet *bullet) {
    int nextX = bullet->coords.x;
    int nextY = bullet->coords.y;
    if (bullet->direction == 0) nextY--;

    for (int i = 0; i < enemyCapacity; i++) {
        if (enemies[i].onScreen &&
            checkCollision(enemies[i].coords.x, enemies[i].coords.y, nextX, nextY)) {
            enemies[i].onScreen = 0;
            bullet->onScreen = 0;
            score++;
            return;
        }
    }

    bullet->coords.x = nextX;
    bullet->coords.y = nextY;
}

int isWall(int x, int y) {
    return x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT || map[y][x] == '#';
}

void showStartArt() {
    screenClear();
    FILE *f = fopen("menu/start.txt", "r");
    if (!f) return;
    char ch;
    while ((ch = fgetc(f)) != EOF) putchar(ch);
    fclose(f);
    printf("\n\n\t\t\t\t1. Iniciar Jogo\n");
    printf("\t\t\t\t2. Instruções\n");
    printf("\t\t\t\t3. Sair\n");
}

void showInstructions() {
    screenClear();
    FILE *f = fopen("menu/instructions.txt", "r");
    if (!f) return;
    char ch;
    while ((ch = fgetc(f)) != EOF) putchar(ch);
    fclose(f);
}

void showVictory() {
    screenClear();
    FILE *f = fopen("menu/victory.txt", "r");
    if (!f) return;
    char ch;
    while ((ch = fgetc(f)) != EOF) putchar(ch);
    fclose(f);
    printf("\n\n\tPressione 'r' para reiniciar ou 'q' para sair.\n");
}

void showGameOver() {
    screenClear();
    FILE *f = fopen("menu/gameover.txt", "r");
    if (!f) return;
    char ch;
    while ((ch = fgetc(f)) != EOF) putchar(ch);
    fclose(f);
    printf("\n\n\tPressione 'r' para tentar novamente ou 'q' para sair.\n");
}

int main() {
    screenInit(0);
    keyboardInit();
    timerInit(75);
    srand(time(NULL));

    int showMenu = 1;
    while (showMenu) {
        showStartArt();
        if (keyhit()) {
            int k = readch();
            if (k == '1') {
                showMenu = 0;
                break;
            } else if (k == '2') {
                showInstructions();
            } else if (k == '3') {
                return 0;
            }
        }
    }

    time_t startTime, currentTime;
    time(&startTime);
    double elapsedTime = 0;
    int frameCount = 0;

    struct Lena lena;
    initLena(&lena);

    bullets = malloc(MAX_BULLETS * sizeof(struct Bullet));
    enemies = malloc(enemyCapacity * sizeof(struct Enemy));
    if (!bullets || !enemies) {
        return 1;
    }

    for (int i = 0; i < enemyCapacity; i++) {
        enemies[i].onScreen = 0;
    }

    while (1) {
        time(&currentTime);
        elapsedTime = difftime(currentTime, startTime);
        if (timerTimeOver()) {
            frameCount++;
            double remainingTime = GAME_DURATION / 75 - elapsedTime;

            screenDrawMap();
            drawLena(lena.coords.x, lena.coords.y);
            spawnEnemy(&lena, frameCount, elapsedTime);

            if (elapsedTime >= GAME_DURATION / 75) {
                showVictory();
                while (1) {
                    int k = readch();
                    if (k == 'r') { break; }
                    if (k == 'q') return 0;
                }
                break;
            }

            if (lena.health <= 0) {
                showGameOver();
                while (1) {
                    int k = readch();
                    if (k == 'r') {break; }
                    if (k == 'q') return 0;
                }
                break;
            }

            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].onScreen) {
                    updateBullet(&bullets[i]);
                    if (bullets[i].onScreen) {
                        drawBullet(bullets[i].coords.x, bullets[i].coords.y);
                    }
                }
            }
            screenUpdate();
        }
    }

    free(bullets);
    free(enemies);
    bullets = NULL;
    enemies = NULL;
    return 0;
}