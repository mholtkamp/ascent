#ifndef CONSTANTS_H
#define CONSTANTS_H

// Key constants
#define KEY_A			0x0001
#define KEY_B	        0x0002
#define KEY_SELECT	    0x0004
#define KEY_START		0x0008
#define KEY_RIGHT		0x0010
#define KEY_LEFT		0x0020
#define KEY_UP		    0x0040
#define KEY_DOWN		0x0080
#define KEY_R           0x0100
#define KEY_L           0x0200

// Palette constants
#define ADDR_PALETTE_BG  0x05000000
#define ADDR_PALETTE_OBJ 0x05000200

#define PALETTE_TYPE_BG   0
#define PALETTE_TYPE_OBJ  1

#define FULL_PALETTE_SIZE 256
#define PALETTE_BANK_SIZE 16
#define FULL_PALETTE_SIZE_BYTES 512
#define PALETTE_BANK_SIZE_BYTES 32

// OBJ sprites constants
#define ADDR_OAM     0x07000000
#define OAM_OBJ_SIZE 8
#define OAM_ATTRIB0_OFFSET 0
#define OAM_ATTRIB1_OFFSET 2
#define OAM_ATTRIB2_OFFSET 4
#define MAX_SPRITES 128

// BG constants
#define BG_0 0
#define BG_1 1
#define BG_2 2
#define BG_3 3

#define ADDR_VRAM 0x06000000
#define CHAR_BLOCK_SIZE 0x4000
#define SCREEN_BLOCK_SIZE 0x0800
#define TILE_4_SIZE 0x20
#define TILE_8_SIZE 0x40
#define SCREEN_ENTRY_SIZE 0x2

// Timers
#define TM_FREQ_1    0
#define TM_FREQ_64   1
#define TM_FREQ_256  2
#define TM_FREQ_1024 3

// Text and Fonts
#define VAKKI_NUM_GLYPHS 64
#define VAKKI_SIZE 2048
#define ASCII_OFFSET 0x20
#define TEXT_SCREENBLOCK 16

#define SCREEN_ENTRIES_PER_LINE 32
#define MAX_LINES_ON_SCREEN 20

#endif