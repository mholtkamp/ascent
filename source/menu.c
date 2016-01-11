#include "menu.h"
#include "images/title.h"
#include "util.h"
#include "constants.h"

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

void menu_update()
{

}