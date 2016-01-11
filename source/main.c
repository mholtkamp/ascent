#include <string.h>
#include "util.h"
#include "registers.h"
#include "constants.h"
#include "menu.h"
#include "images/title.h"


#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

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
    
	// Game Loop
	while(1)
	{
		sync_with_vblank();
		
		
		

	}
	return 0;
}