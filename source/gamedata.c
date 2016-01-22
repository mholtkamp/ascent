#include <string.h>
#include "gamedata.h"

void game_data_initialize(GameData* pGameData)
{
    if (pGameData != 0)
    {
        pGameData->nGameState = STATE_GAME;
        pGameData->nLevel = 1;
        pGameData->nRoomX = 0;
        pGameData->nRoomY = 0;
        memset(pGameData->arFloor, 0, FLOOR_WIDTH * FLOOR_HEIGHT);
        hero_initialize(&(pGameData->hero));
        pGameData->nPaused = 0;
        pGameData->nDelay  = 0;
        memset(pGameData->arBullets, 0, MAX_BULLETS * sizeof(Bullet));
        memset(pGameData->arEnemies, 0, MAX_ENEMIES * sizeof(Enemy));
        pGameData->nBulletIndex = 0;
    }
}