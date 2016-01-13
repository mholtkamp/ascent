#include "game.h"
#include "util.h"
#include "images/img_rooms.h"
#include "images/img_hero.h"

void game_initialize(GameData* pData)
{
    print("IN-GAME");
    
    // Load palette for room backgrounds
    load_palette_bank(PALETTE_TYPE_BG, 0, arRoomPalette);
    
    // Load tiles for room backgrounds
    load_tiles(0, // CBB 0
               0, // 0 offset
               32, // 32 tiles in BG
               arRoomTiles);
    
    // Load the starting room
    _game_load_room();
    
    // Initialize the hero
    hero_static_initialize(&(pData->hero));
}

void game_update(GameData* pData)
{
    hero_update(&(pData->hero));
}

void _game_load_room()
{
    // Load tile map
    load_map(31, // SBB 31
             arRoom1Map);
}