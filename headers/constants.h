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

// OBJ sprites constants
#define ADDR_OAM     0x07000000
#define OAM_OBJ_SIZE 8
#define OAM_ATTRIB0_OFFSET 0
#define OAM_ATTRIB1_OFFSET 2
#define OAM_ATTRIB2_OFFSET 4


#endif