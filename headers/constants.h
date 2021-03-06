#ifndef CONSTANTS_H
#define CONSTANTS_H

// ****************************************************************************
//  GBA Constants
// ****************************************************************************
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

#define SIZE_8x8 0
#define SIZE_16x8 0
#define SIZE_8x16 0
#define SIZE_16x16 1
#define SIZE_32x8 1
#define SIZE_8x32 1
#define SIZE_32x32 2
#define SIZE_32x16 2
#define SIZE_16x32 2
#define SIZE_64x64 3
#define SIZE_64x32 3
#define SIZE_32x64 3

#define SHAPE_8x8 0
#define SHAPE_16x8 1
#define SHAPE_8x16 2
#define SHAPE_16x16 0
#define SHAPE_32x8 1
#define SHAPE_8x32 2
#define SHAPE_32x32 0
#define SHAPE_32x16 1
#define SHAPE_16x32 2
#define SHAPE_64x64 0
#define SHAPE_64x32 1
#define SHAPE_32x64 2

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
#define SCREEN_BLOCK_MAP_WIDTH 32
#define SCREEN_BLOCK_MAP_HEIGHT 32
#define PIXELS_PER_TILE 8

// Timers
#define TM_FREQ_1    0
#define TM_FREQ_64   1
#define TM_FREQ_256  2
#define TM_FREQ_1024 3

// Text and Fonts
#define VAKKI_NUM_GLYPHS 64
#define VAKKI_SIZE 2048
#define ASCII_OFFSET 0x20
#define TEXT_CBB 1
#define TEXT_SCREENBLOCK 16

#define SCREEN_ENTRIES_PER_LINE 32
#define MAX_LINES_ON_SCREEN 20

// ****************************************************************************
//  Ascent Constants
// ****************************************************************************

// Game States
#define STATE_MENU 0
#define STATE_GAME 1
#define STATE_GAME_OVER 2
#define STATE_WIN 3
#define STATE_FLOOR_TRANS 4

// Floor constants
#define FLOOR_WIDTH 10
#define FLOOR_HEIGHT 10
#define NUM_ROOM_MAPS 6
#define NUM_LEVELS 5
#define ROOM_CLEARED_FLAG 0x80
#define ROOM_SBB 31

#define BG_COLLISION_BIT 0x0010

#define LEVEL_1_ROOM_COUNT 6
#define LEVEL_2_ROOM_COUNT 9
#define LEVEL_3_ROOM_COUNT 12
#define LEVEL_4_ROOM_COUNT 13
#define LEVEL_5_ROOM_COUNT 15
#define NUM_FLOORS 5


#define HERO_SPRITE_INDEX 0 
#define ADDR_ROOM_SBB 0x0600f800

#define MAX_ENEMIES 12
#define MAX_BULLETS 12

// Tiles
#define BG_CBB 0
#define SPRITE_CBB 4
#define ENEMY_TILE_START_INDEX 32

// Palettes
#define HERO_PALETTE_BANK 0
#define BULLET_HERO_PALETTE_BANK 1
#define BULLET_ENEMY_PALETTE_BANK 2
#define ENEMY_PALETTE_BANK_START 3
#define ENEMY_PALETTE_BANK_END 7  // includes bank 7
#define DROP_PALETTE_BANK 8
#define DAMAGE_PALETTE_BANK 15

// Important tile indices
#define MUSHROOM_TILE_INDEX_0 18
#define MUSHROOM_TILE_INDEX_1 19
#define MUSHROOM_TILE_INDEX_2 26
#define MUSHROOM_TILE_INDEX_3 27
#define GROUND_TILE_INDEX 3
#define WALL_TILE_INDEX 20
#define OPEN_DOOR_TILE_INDEX 12
#define STAIRS_TILE_INDEX 11
#define DROP_TILE_START_INDEX 128 //(in CBB1)

// Hero
#define HERO_START_X 120
#define HERO_START_Y 60

#define HERO_HORI_TILE_INDEX 0
#define HERO_VERT_TILE_INDEX 4
#define HERO_DIAG_TILE_INDEX 8

#define HERO_STARTING_HEALTH 3
#define HERO_STARTING_GEMS   3
#define HERO_STARTING_DAMAGE 3
#define HERO_STARTING_SPEED  1

#define HERO_MAX_HEALTH 8
#define HERO_MAX_GEMS   99
#define HERO_MAX_DAMAGE 8
#define HERO_MAX_SPEED  8

#define HERO_RECT_WIDTH  14
#define HERO_RECT_HEIGHT 14
#define HERO_SPRITE_X_OFF 1
#define HERO_SPRITE_Y_OFF 1

// Enemies
#define MAX_ENEMY_TYPES_PER_ROOM 5
#define ENEMY_OBJ_START_INDEX 10
#define MAX_ENEMY_MAPS_PER_ROOM 3
#define ENEMY_SCRATCH_DATA_SIZE 64

#define DIR_RIGHT 0
#define DIR_DOWN  1
#define DIR_LEFT  2
#define DIR_UP    3


#define ENEMY_TYPE_CATERPILLAR 0
#define ENEMY_TYPE_PIXIE 1
#define NUM_ENEMY_TYPES 2

// Drops

#define DROP_TYPE_GEM   0
#define DROP_TYPE_HEART 1
#define DROP_TYPE_BOMB  2
#define DROP_TYPE_KEY   3
#define NUM_DROP_TYPES 4

// Other
#define ENEMY_DAMAGE_COUNTER_START 5
#define HERO_DAMAGE_COUNTER_START 60

// Special Room IDs
#define ROOM_STAIRS   0x41
#define ROOM_ITEM     0x42
#define ROOM_SHOP     0x43
#define ROOM_FOUNTAIN 0x44


#endif