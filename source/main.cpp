#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <test.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define REG_VCOUNT *(volatile unsigned short*)0x04000006
#define REG_KEYS *(volatile unsigned short*)0x04000130
#define K_A			0x0001
#define K_B			0x0002
#define K_SELECT	0x0004
#define K_START		0x0008
#define K_RIGHT		0x0010
#define K_LEFT		0x0020
#define K_UP		0x0040
#define K_DOWN		0x0080
#define NUM_PIPES	3
#define GAP_SIZE	50
#define PIPE_SPACING 70

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;

const unsigned short bgPal[256] __attribute__((aligned(4)))=
{
0x0000,0x316c,0x16a7,0x2303,0x2269,0x56a5,0x001b,0x317b,0x4a32,0x6f7b,0x7fff,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
};

const unsigned short bgTiles[448] __attribute__((aligned(4)))=
{
0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,
0x5555,0x8881,0x5555,0x8881,0x5555,0x1881,0x5555,0x1881,0x5555,0x1881,0x5555,0x1881,0x5555,0x1881,0x5555,0x8881,
0x8888,0x8888,0x8888,0x8888,0x8111,0x1188,0x8111,0x1188,0x8111,0x1188,0x8111,0x1188,0x8111,0x1188,0x8888,0x8888,
0x8888,0x5551,0x8888,0x5551,0x8881,0x5551,0x8881,0x5551,0x8881,0x5551,0x8881,0x5551,0x8881,0x5551,0x8888,0x5551,
0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x9555,0x5555,0x9555,0x5555,0x9555,
0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x9995,0x9955,0xaa99,0xa999,0xaaaa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,
0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5999,0x5555,0x99aa,0x5555,0x9aaa,0x5599,0xaaaa,0x999a,0xaaaa,0xaaaa,
0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5559,0x5555,0x5599,0x5555,
0x5555,0x9555,0x5555,0x9955,0x5555,0xaa95,0x5555,0xa995,0x5555,0x9555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,
0xaaaa,0xaaaa,0xaaaa,0xaaaa,0x99aa,0x9999,0xaaaa,0x9aaa,0xa999,0xaaaa,0x9955,0x99aa,0x5555,0x5599,0x5555,0x5555,
0xa999,0xaaaa,0xa9aa,0xaaaa,0xaaaa,0xaaaa,0xaaaa,0x9999,0xaaaa,0x9aaa,0x9999,0xaaa9,0x5555,0x9999,0x5555,0x5555,
0x559a,0x5555,0x599a,0x5555,0x59aa,0x5555,0x59aa,0x5555,0x59aa,0x5555,0x5599,0x5555,0x5559,0x5555,0x5555,0x5555,
0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x0005,0x0555,0x3300,0x0555,0x3333,0x3005,0x3333,
0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5550,0x5555,0x0000,0x5550,0x3333,0x5550,0x3333,0x5003,
0x5555,0x8881,0x5555,0x8881,0x5555,0x8881,0x5555,0x8881,0x5555,0x0001,0x0555,0x3300,0x0555,0x3333,0x3005,0x3333,
0x8888,0x8888,0x8888,0x8888,0x8888,0x8888,0x8888,0x8888,0x8880,0x8888,0x0000,0x8880,0x3333,0x8880,0x3333,0x8003,
0x8888,0x8888,0x8888,0x8888,0x8888,0x8888,0x8888,0x8888,0x8888,0x0008,0x0888,0x3300,0x0888,0x3333,0x3008,0x3333,
0x8888,0x5551,0x8888,0x5551,0x8888,0x5551,0x8888,0x5551,0x8880,0x5551,0x0000,0x5550,0x3333,0x5550,0x3333,0x5003,
0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x5555,0x1155,0x5555,0x8155,0x5555,0x8155,0x5555,0x8155,0x5555,0x8155,
0x3305,0x2333,0x3330,0x3333,0x2333,0x3322,0x2333,0x3334,0x3333,0x2333,0x3333,0x4333,0x4233,0x3333,0x3333,0x3333,
0x3342,0x5033,0x3333,0x0333,0x2233,0x3333,0x4433,0x3333,0x3334,0x3333,0x3334,0x3333,0x3333,0x3322,0x3333,0x3344,
0x3305,0x2333,0x3330,0x3333,0x7333,0x3326,0x6333,0x3336,0x3333,0x2333,0x3333,0x4333,0x4233,0x3333,0x3333,0x3333,
0x3342,0x5033,0x3333,0x0333,0x2233,0x3333,0x7433,0x3337,0x6334,0x3336,0x3367,0x3333,0x3366,0x3322,0x3333,0x3344,
0x3342,0x8033,0x3333,0x0333,0x2233,0x3333,0x4433,0x3333,0x3334,0x3333,0x3334,0x3333,0x3333,0x3322,0x3333,0x3344,
0x3308,0x2333,0x3330,0x3333,0x2333,0x3322,0x2333,0x3334,0x3333,0x2333,0x3333,0x4333,0x4233,0x3333,0x3333,0x3333,
0x5555,0x5555,0x5555,0x5555,0x5555,0x1111,0x5555,0x8881,0x5555,0x8881,0x5555,0x8881,0x5555,0x8881,0x5555,0x8881,
0x5555,0x8155,0x5555,0x8155,0x1111,0x8111,0x8888,0x8888,0x8888,0x8888,0x8888,0x8888,0x8888,0x8888,0x8888,0x8888,
0x5555,0x5555,0x5555,0x5555,0x1111,0x5551,0x8888,0x5551,0x8888,0x5551,0x8888,0x5551,0x8888,0x5551,0x8888,0x5551
};
const unsigned short bgMap[2048] __attribute__((aligned(4)))=
{
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0004,0x0005,0x0006,0x0007,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0008,0x0009,0x000a,0x000b,0x0000,0x0000,0x0000,0x0004,0x0005,0x0006,0x0007,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0008,0x0009,0x000a,0x000b,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0004,0x0005,0x0006,0x0007,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0004,0x0005,0x0006,0x0007,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0008,0x0009,0x000a,0x000b,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0008,0x0009,0x000a,0x000b,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0004,0x0005,0x0006,0x0007,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0012,0x0412,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0008,0x0009,0x000a,0x000b,0x0000,0x0000,
0x0000,0x0004,0x0005,0x0006,0x0007,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0019,0x001a,0x041a,0x001b,
0x0000,0x0000,0x0004,0x0005,0x0006,0x0007,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0008,0x0009,0x000a,0x000b,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,0x0402,0x0003,
0x0000,0x0000,0x0008,0x0009,0x000a,0x000b,0x0000,0x0012,0x0412,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,0x0402,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0019,0x001a,0x041a,0x001b,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,0x0402,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,0x0402,0x0003,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,0x0402,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,0x0402,0x0003,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,0x0402,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,0x0402,0x0003,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,0x0402,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,0x0402,0x0003,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,0x0402,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,0x0402,0x0003,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,0x0402,0x0003,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0001,0x0002,0x0402,0x0003,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x000c,0x000d,0x000c,0x000d,0x000c,0x000d,0x000c,0x000d,0x000c,0x000d,0x000c,0x000d,0x000e,0x000f,0x0010,0x0011,
0x000c,0x000d,0x000c,0x000d,0x000c,0x000d,0x000e,0x000f,0x0010,0x0011,0x000c,0x000d,0x000c,0x000d,0x000c,0x000d,
0x0013,0x0014,0x0013,0x0014,0x0013,0x0014,0x0015,0x0016,0x0013,0x0014,0x0013,0x0014,0x0013,0x0017,0x0018,0x0014,
0x0013,0x0014,0x0015,0x0016,0x0013,0x0014,0x0013,0x0017,0x0018,0x0014,0x0013,0x0014,0x0013,0x0014,0x0015,0x0016
};
const unsigned short birdPal[256] __attribute__((aligned(4)))=
{
	0x0000,0x7FFF,0x0C75,0x0377,0x5525,0x2537,0x3197,0x55AA,
	0x14B5,0x0842,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};
const unsigned short birdBitmap[128] __attribute__((aligned(4)))=
{
	0x0090,0x0000,0x0989,0x0000,0x9889,0x9909,0x8889,0x6998,
	0x8889,0x9888,0x8890,0x8888,0x8900,0x8888,0x2900,0x8882,
	0x0000,0x0000,0x0000,0x0000,0x0099,0x0000,0x9966,0x0000,
	0x6666,0x0009,0x6669,0x0009,0x4498,0x0096,0x7498,0x0936,
	0x9900,0x2229,0x5900,0x9995,0x5900,0x6555,0x9000,0x5555,
	0x9000,0x5555,0x0000,0x5599,0x0000,0x9900,0x0000,0x0000,
	0x6669,0x9336,0x6666,0x9336,0x6666,0x0996,0x6666,0x0009,
	0x5555,0x0009,0x9955,0x0000,0x0099,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x9900,0x0000,0x6990,
	0x9000,0x6669,0x9000,0x6666,0x6900,0x9996,0x6900,0x8889,
	0x0000,0x0000,0x0000,0x0000,0x0099,0x0000,0x9966,0x0000,
	0x6666,0x0009,0x6666,0x0009,0x4499,0x0096,0x7498,0x0936,
	0x9900,0x8888,0x9900,0x8888,0x8900,0x9888,0x8900,0x5988,
	0x8900,0x5988,0x8900,0x5598,0x8900,0x9909,0x9900,0x0000,
	0x6669,0x9336,0x6669,0x9336,0x6666,0x0996,0x6666,0x0009,
	0x5555,0x0009,0x9955,0x0000,0x0099,0x0000,0x0000,0x0000,
};

const unsigned short pipechunkBitmap[128] __attribute__((aligned(4)))=
{
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,

	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
	0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,
};

const unsigned short pipechunkPal[16] __attribute__((aligned(4)))=
{
	0x0000,0x0010,0x0200,0x0210,0x4000,0x4010,0x4200,0x6318,
	0x4210,0x001F,0x0240,0x03FF,0x7C00,0x7C1F,0x7FE0,0x7FFF
};


u16* dcr;
bool prevDown;
typedef struct
{
	u16 color;
	u32 width;
	u32 height;
	int x;
	int y;
	int xVel;
	int yVel;
	int grav;
} Box;
typedef struct
{
	u16 color;
	Box top;
	Box bot;
	int x;
	int y;
	int xVel;
} Pipe;
void vertSync()
{
	while(REG_VCOUNT >= 160);   // wait till VDraw
    while(REG_VCOUNT < 160);    // wait till VBlank
}
bool keyDown(u16 key)
{
	return (~REG_KEYS) & key;
}
void flipPages(u16* dcr)
{
	/*u16 cr = *dcr;
	cr = cr ^ 0x0010;
	*dcr = cr;*/
}
void initBirdObj()
{
	*((unsigned short*) 0x07000000) = 0x001f;
	*((unsigned short*) 0x07000002) = 0x403f;
	*((unsigned short*) 0x07000004) = 0x0020;
}
void setY(float y)
{
	*((unsigned short*) 0x07000000) = 0x00FF & ((unsigned short) y);
}
int main()
{
	int i;
	srand(time(NULL));
	Box bird;
	bird.x = 20;
	bird.y = -30;
	bird.width = 16;
	bird.height = 16;
	
	
	//Background Memory Load

	// Load palette
	memcpy((unsigned short*)0x05000000, bgPal, 512);
	// Load tiles into char block 0
	memcpy((unsigned short*)0x06000000, bgTiles, 896);
	// Load map into screen block 31
	memcpy((unsigned short*) (0x06000000 + 31*0x800), bgMap, 4096);
	
	
	
	
	//Sprite Memory Load
	
	//Load bird sprite palette
    memcpy((void*)0x05000200, birdPal, 512);
	//Load bird sprite tiles
	memcpy((void*)0x06010020, birdBitmap, 256);
	
	//Load pipe chunk sprite palette
	memcpy((void*)0x05000220, pipechunkPal, 32);
	//Load pipe chunk sprite tiles
	memcpy((void*)0x06010000 + 1*0x800, pipechunkBitmap, 256);
	
	initBirdObj();
	
	//Set up video registers. Video Mode 0, BG0 only.
	*((unsigned short*) 0x04000008)= 0x1f00;
	*((unsigned short*) 0x04000000)= 0x1140;
	// Scroll around some
	float x= 0;
	int y= 0;
	float GRAVITY = 0.1f;
	float yVel = 0.0f;
	bool keyPressed = false;
	
	while(1)
	{
		vertSync();
	
		//Scroll viewport
		x += 0.5;
		*((unsigned short*)0x4000010) = (unsigned short) x;
		
		
		
		if(keyDown(K_A) && !keyPressed)
		{
			yVel = -2.2f;
			keyPressed = true;
		}
		else
		{
			yVel += GRAVITY;
		}
		if(!keyDown(K_A))
		{
			keyPressed = false;
		}
		
		bird.y += yVel;
		if(bird.y > 160 - bird.height)
			bird.y = 160 - bird.height;
		setY(bird.y);
		
			
		y += 1;
		if( y%20 < 10)
				*((unsigned short*) 0x07000004) = 0x0005;
		else
				*((unsigned short*) 0x07000004) = 0x0001;
	}
	return 0;
}