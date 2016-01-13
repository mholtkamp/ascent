#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "constants.h"
#include "hero.h"

typedef struct GameData
{
    int nGameState;
    int nLevel;
    int nRoom;
    char arFloor[FLOOR_WIDTH][FLOOR_HEIGHT];
    Hero hero;
    
} GameData;

void game_data_initialize(GameData* pGameData);

#endif