#ifndef GAME_H
#define GAME_H

#include "gamedata.h"

void game_initialize(GameData* pData);

void game_update(GameData* pData);

void game_draw_hud(GameData* pData);

void game_load_floor(GameData* pData);

void _game_attach_room(GameData* pData,
                       char  cRoomNum,
                       int   nNumRooms,
                       int* arRoomListX,
                       int* arRoomListY);
                       
void _game_generate_floor(GameData* pData);

void _game_load_room(GameData* pData);

void _game_clear_room(GameData* pData);

void _game_update_sprites(GameData* pData);


#endif