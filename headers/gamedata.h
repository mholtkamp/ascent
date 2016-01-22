#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "constants.h"
#include "hero.h"
#include "bullet.h"
#include "enemy.h"

typedef struct GameData
{
    int nGameState;
    int nLevel;
    int nRoomX;
    int nRoomY;
    char arFloor[FLOOR_WIDTH][FLOOR_HEIGHT];
    Hero hero;
    int nPaused;
    int nDelay;
    Bullet arBullets[MAX_BULLETS];
    Enemy arEnemies[MAX_ENEMIES];
    int nBulletIndex;
} GameData;

void game_data_initialize(GameData* pGameData);

#endif