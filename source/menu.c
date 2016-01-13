#include "menu.h"
#include "images/img_title.h"
#include "util.h"
#include "constants.h"
#include "gamedata.h"
#include "game.h"

void menu_initialize()
{
	// Load tiles into char block 0
	load_tiles(0, // Char block 0
               0, // offset = 0 tiles
               58, // tile count  = (448 * 2)/32 = 14 tiles
               arTitleTiles); // tile data
               
    // load palette into bank 0
    load_palette_bank(PALETTE_TYPE_BG, 0, arTitlePalette);
    
    // load map into last screen entry
    load_map(31, arTitleMap);
    
    // display text to say press start
    text("PRESS START",9,12);
}

void menu_update(GameData* pData)
{
    if (key_down(KEY_START))
    {
        // Initialize the game data struct
        game_data_initialize(pData);
        
        // Set the game state to STATE_GAME
        // This is actually redundant, but kept for clarity.
        pData->nGameState = STATE_GAME;
        
        // Clear screen text
        clear_text();
        
        // Initialize game, specifically by uploading data to
        // video ram for tiles and maps.
        game_initialize(pData);
    }
}