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
int lastSpawnFrame = 0;
int reloadTime = 0;
int score = 0;

void screenDrawMap();
int isWall(int x, int y);
int checkCollision(int x1, int y1, int x2, int y2);
void drawBullet(int x, int y);
void initBullet(struct Bullet *bullet, struct Lena *lena);
void updateBullet(struct Bullet *bullet);

void initLena(struct Lena *lena);
void drawLena(int x, int y);

void showStartArt();
void showInstructions();
void showVictory();
void showGameOver();

void initEnemy(struct Enemy *enemy);
void drawEnemy(int x, int y);
void updateEnemy(struct Enemy *enemy, struct Lena *lena);
void spawnEnemy(struct Lena *lena, int frameCount, double elapsedTime);

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

int showMenu = 1;

int main() {
    screenInit(0);
    keyboardInit();
    timerInit(75);
    srand(time(NULL));

    while (1) {
        showStartArt();
        while (1) {
            if (keyhit()) {
                int key = readch();
                if (key == '1') {
                    showMenu = 0;
                    break;
                }
                else if (key == '2') {
                    showInstructions();
                    showMenu = 1;
                }
                else if (key == '3') {
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

                if (lena.ammo < MAX_BULLETS && reloadTime <= 0) {
                    reloadTime = 2500 / 75;
                    lena.ammo++;
                }
                else if (reloadTime > 0) {
                    reloadTime--;
                }

                screenGotoxy(MAP_WIDTH / 2 - 3, MAP_HEIGHT);
                printf("💀: %d", score);
                screenGotoxy(0, MAP_HEIGHT);
                printf("🔫: %d", lena.ammo);
                screenGotoxy(MAP_WIDTH + 1, 1);
                printf("⌛: ");
                if (remainingTime < 10) {
                    printf("0%.0lf", remainingTime);
                } else {
                    printf("%.0lf", remainingTime);
                }
                screenGotoxy(MAP_WIDTH - 20, MAP_HEIGHT);
                printf(" ");
                screenGotoxy(MAP_WIDTH - 20, MAP_HEIGHT);
                for (int i = 0; i < lena.health; i++)
                    printf("❤");


                if (elapsedTime >= GAME_DURATION / 75) {
                    showVictory();
                    while (1) {
                        if (keyhit()) {
                            int key = readch();
                            if (key == 'r') {
                                lena.health = 10;
                                lena.ammo = MAX_BULLETS;
                                lena.coords.x = MAP_WIDTH/2;
                                lena.coords.y = MAP_HEIGHT/2;
                                score = 0;
                                memset(bullets, 0, MAX_BULLETS * sizeof(*bullets));
                                memset(enemies, 0, enemyCapacity * sizeof(*enemies));
                                screenClear();
                                time(&startTime);
                                lastSpawnFrame = 0;
                                frameCount = 0;
                                break;
                            }
                            else if (key == 'q') {
                                free(bullets);
                                free(enemies);
                                return 0;
                            }
                        }
                    }
                    break;
                }

                if (lena.health <= 0) {
                    showGameOver();
                    while (1) {
                        if (keyhit()) {
                            int key = readch();
                            if (key == 'r') {
                                lena.health = 10;
                                lena.ammo = 8;
                                lena.coords.x = MAP_WIDTH / 2;
                                lena.coords.y = MAP_HEIGHT / 2;
                                score = 0;
                                memset(bullets, 0, MAX_BULLETS * sizeof(struct Bullet));
                                memset(enemies, 0, enemyCapacity * sizeof(struct Enemy));
                                screenClear();
                                time(&startTime);
                                lastSpawnFrame = 0;
                                frameCount = 0;
                                break;
                            }
                            else if (key == 'q') {
                                free(bullets);
                                free(enemies);
                                return 0;
                            }
                        }
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

            if (keyhit()) {
                int key = readch();
                int newX = lena.coords.x;
                int newY = lena.coords.y;

                if (key == 'a') {
                    newX--;
                }
                else if (key == 'd') {
                    newX++;
                }
                else if (key == 'f' && lena.ammo > 0) {
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bullets[i].onScreen) {
                            initBullet(&bullets[i], &lena);
                            bullets[i].onScreen = 1;
                            lena.ammo--;
                            break;
                        }
                    }
                }

                if (!isWall(newX, newY) &&
                    !isWall(newX + 1, newY) &&
                    newX >= 0 && newX < MAP_WIDTH &&
                    newY >= 0 && newY < MAP_HEIGHT)
                {
                    lena.coords.x = newX;
                    lena.coords.y = newY;
                }
            }
        }

        free(bullets);
        free(enemies);
        bullets = NULL;
        enemies = NULL;
    }
    return 0;
}

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

void drawEnemy(int x, int y) {
    screenGotoxy(x, y);
    printf("👾");
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

void spawnEnemy(struct Lena *lena, int frameCount, double elapsedTime) {
    if (elapsedTime < GAME_DURATION / 75 && frameCount - lastSpawnFrame >= ENEMY_SPAWN_INTERVAL) {
        for (int i = 0; i < enemyCapacity; i++) {
            if (!enemies[i].onScreen) {
                initEnemy(&enemies[i]);
                enemies[i].onScreen = 1;
                lastSpawnFrame = frameCount;
                break;
            }
        }
    }

    for (int i = 0; i < enemyCapacity; i++) {
        if (enemies[i].onScreen) {
            updateEnemy(&enemies[i], lena);
            drawEnemy(enemies[i].coords.x, enemies[i].coords.y);
        }
    }
}

void drawLena(int x, int y) {
    screenGotoxy(x, y);
    printf("👩‍🚀");
}

void initLena(struct Lena *lena) {
    lena->coords.x = MAP_WIDTH / 2;
    lena->coords.y = MAP_HEIGHT - 2;
    lena->health = 10;
    lena->ammo = MAX_BULLETS;
}

void drawBullet(int x, int y) {
    screenSetColor(COLOR_BULLET, BLACK);
    screenGotoxy(x, y);
    printf("•");
}

void initBullet(struct Bullet *bullet, struct Lena *lena) {
    bullet->coords.x = lena->coords.x;
    bullet->coords.y = lena->coords.y - 1;
    bullet->onScreen = 1;
    bullet->direction = 0;
}

void updateBullet(struct Bullet *bullet) {
    int nextX = bullet->coords.x;
    int nextY = bullet->coords.y;

    if (bullet->direction == 0) nextY--;
    else if (bullet->direction == 1) nextX--;
    else if (bullet->direction == 2) nextY++;
    else if (bullet->direction == 3) nextX++;

    for (int i = 0; i < enemyCapacity; i++) {
        if (enemies[i].onScreen &&
            enemies[i].coords.x == nextX &&
            enemies[i].coords.y == nextY) {
            bullet->onScreen = 0;
            enemies[i].health--;
            if (enemies[i].health <= 0)
                enemies[i].onScreen = 0;
            score++;
            return;
        }
    }

    if (isWall(nextX, nextY) || nextX < 0 || nextX >= MAP_WIDTH || nextY < 0 || nextY >= MAP_HEIGHT) {
        bullet->onScreen = 0;
        return;
    }

    bullet->coords.x = nextX;
    bullet->coords.y = nextY;
}

int checkCollision(int x1, int y1, int x2, int y2) {
    return x1 == x2 && y1 == y2;
}

int isWall(int x, int y) {
    return (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT || map[y][x] == '#');
}

void showStartArt() {
    screenClear();
    FILE *file = fopen("menu/start.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo start.txt\n");
        return;
    }

    screenSetColor(RED, BLACK);

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);

    screenSetColor(WHITE, BLACK);
    printf("\n\n\t\t\t Bem vindo à estação corrompida: HellPixel\n");
    printf("\n\n\t\t\t\tEscolha uma opção:\n");
    printf("\t\t\t\t1. Iniciar Jogo\n");
    printf("\t\t\t\t2. Instruções\n");
    printf("\t\t\t\t3. Sair\n");
}

void showInstructions() {
    screenClear();
    FILE *file = fopen("menu/instructions.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo start.txt\n");
        return;
    }

    screenSetColor(LIGHTBLUE, BLACK);

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);
}

void showVictory() {
    screenClear();
    FILE *file = fopen("menu/victory.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo victory.txt\n");
        return;
    }

    screenSetColor(GREEN, BLACK);

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);
}

void showGameOver() {
    screenClear();
    FILE *file = fopen("menu/gameover.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo gameover.txt\n");
        return;
    }

    screenSetColor(RED, BLACK);

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);

    screenSetColor(WHITE, BLACK);
    printf("\n\n\t\t\t\t Não foi dessa vez!\n");
    printf("\t\t\t\t Pontuação Final: %d\n", score);
    printf("\t\t\tInsira r para reiniciar o jogo ou q para sair.\n");
}