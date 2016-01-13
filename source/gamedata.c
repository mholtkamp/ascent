#include <string.h>
#include "gamedata.h"

void game_data_initialize(GameData* pGameData)
{
    if (pGameData != 0)
    {
        pGameData->nGameState = STATE_GAME;
        pGameData->nLevel = 1;
        pGameData->nRoom = 0;
        memset(pGameData->arFloor, 0, FLOOR_WIDTH * FLOOR_HEIGHT);
        hero_initialize(&(pGameData->hero));
    }
}