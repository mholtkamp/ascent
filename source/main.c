#include <string.h>
#include "util.h"
#include "registers.h"
#include "constants.h"
#include "menu.h"
#include "gamedata.h"
#include "game.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

GameData gamedata;

int main()
{	
	// Initialize utility library
    initialize_util();
	
	
	//Sprite Memory Load
    initialize_display(0, // Video mode 0
                       0, // No frame selection needed
                       0, // HBlankFree disabled
                       1, // 1 dimensional sprite mapping
                       0, // No forced blank
                       1, // BG0 Enabled
                       1, // BG1 Enabled
                       0, // BG2 Disabled
                       0, // BG3 Disabled
                       1);// OBJ (sprites) enabled
    
    // Initialize the Vakki 8x8 font
    initialize_font();
                       
    // Clear all sprites before using them
    clear_all_sprites();
	
	//Set up background control register
    initialize_background(1,  // BG 1 
                          1,  // Priority = 1
                          0,  // CharBaseBlock = 0
                          0,  // Mosaic = off
                          0,  // Palette = 16/16
                          31, // last screen block in VRAM (okay because size is 256x256).
                          0,  // wrap = 0
                          0); // screen size  = 1x1 (32t x 32t)
                          
    menu_initialize();
    
    int nGameOverTick = 0;
    int nTransTick    = 0;
	// Game Loop
	while(1)
	{
		sync_with_vblank();
        poll_keys();
		
        if (gamedata.nGameState == STATE_MENU)
        {
            menu_update(&gamedata);
        }
        else if (gamedata.nGameState == STATE_GAME)
        {
            game_update(&gamedata);
        }
        else if (gamedata.nGameState == STATE_GAME_OVER)
        {
            nGameOverTick++;
            
            // After two seconds, set state back to menu state
            if (nGameOverTick >= 120)
            {
                // Clear this variable for the next time that the 
                // game goes into game over state
                nGameOverTick = 0;
                
                // Switch state to the menu state
                gamedata.nGameState = STATE_MENU;
                
                // And make sure the graphics are set up to display
                // the title/menu screen
                menu_initialize();
            }
        }
        else if (gamedata.nGameState == STATE_WIN)
        {

        }
        else if (gamedata.nGameState == STATE_FLOOR_TRANS)
        {
            nTransTick++;
            
            if (nTransTick >= 120)
            {
                nTransTick = 0;
                gamedata.nGameState = STATE_GAME;
                game_load_floor(&gamedata);
            }
        }
		
	}
	return 0;
}