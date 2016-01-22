
//{{BLOCK(kingman)

//======================================================================
//
//	kingman, 16x16@4, 
//	+ palette 16 entries, not compressed
//	+ 4 tiles not compressed
//	Total size: 32 + 128 = 160
//
//	Time-stamp: 2016-01-12, 17:05:36
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

const unsigned short arHeroHoriTiles[64] __attribute__((aligned(4)))=
{
	0x0030,0x0000,0x3990,0x1000,0x9900,0x1339,0x3430,0x1134,
	0x4400,0x5134,0x9940,0x5149,0x4300,0xA144,0x4940,0xA133,
	0x0000,0x0000,0x0B11,0x0000,0x0011,0x0000,0x0015,0x0BB0,
	0x0155,0x00B0,0xBB5B,0x0BB0,0xBBB5,0x00BB,0x77BB,0x00BB,
	0x4000,0xA133,0x4930,0xA199,0x4400,0x5144,0x4430,0x5139,
	0x9990,0x1199,0x4000,0x1333,0x9930,0x1000,0x0030,0x0000,
	0x77BB,0x00BB,0xBBB5,0x00BB,0xBB5B,0x0BB0,0x0155,0x00B0,
	0x0015,0x0BB0,0x0011,0x0000,0x0B11,0x0000,0x0000,0x0000,
};

const unsigned short arHeroVertTiles[64] __attribute__((aligned(4)))=
{
	0x0000,0x0000,0xB000,0x00B0,0xB000,0xBBBB,0x0000,0xBB00,
	0x0000,0x7BB0,0x00B0,0x7BB1,0x1110,0xBB55,0x5110,0xB5B5,
	0x0000,0x0000,0x0B00,0x000B,0xBBBB,0x000B,0x00BB,0x0000,
	0x0BB7,0x0000,0x1BB7,0x0B00,0x55BB,0x0111,0x5B5B,0x0115,
	0x1110,0xAA55,0x1300,0x1111,0x3300,0x3443,0x4900,0x3494,
	0x3930,0x4494,0x4990,0x9394,0x3093,0x4040,0x0000,0x0000,
	0x55AA,0x0111,0x1111,0x0031,0x3493,0x0039,0x9493,0x0039,
	0x4444,0x0949,0x4490,0x0909,0x3030,0x3309,0x0000,0x0000,
};

const unsigned short arHeroDiagTiles[64] __attribute__((aligned(4)))=
{
	0x0000,0x0000,0x0000,0x0000,0x0000,0x00B0,0x0000,0x0011,
	0x1000,0x1151,0x3000,0x5551,0x3300,0xBB51,0x3493,0x5513,
	0x00B0,0x0000,0x00B0,0x0000,0xBB00,0x0000,0xB000,0x0000,
	0xBBBB,0x000B,0xB7BB,0x0BBB,0x777B,0x0B0B,0xB7BB,0xB00B,
	0x4339,0xA144,0x9443,0x1349,0x9900,0x3334,0x3400,0x9344,
	0x4000,0x4449,0x0000,0x4490,0x0000,0x4400,0x0000,0x9300,
	0xBBBB,0x000B,0x5BAA,0x0001,0x5551,0x0001,0x1514,0x0B15,
	0x1134,0x0011,0x3999,0x0001,0x0399,0x0000,0x0049,0x0000,
};

const unsigned short arHeroPalette[16] __attribute__((aligned(4)))=
{
	0x0000,0x7FE0,0x56B5,0x0096,0x14AE,0x7C25,0x4200,0x7FFF,
	0x31F6,0x001F,0x7EA0,0x03FF,0x7C00,0x7C1F,0x7FE0,0x7FFF,
};

const unsigned short arDamagePalette[16] __attribute__((aligned(4))) =
{
    0x0000,0x001F,0x001F,0x001F,0x001F,0x001F,0x001F,0x001F,
    0x001F,0x001F,0x001F,0x001F,0x001F,0x001F,0x001F,0x001F
};

//}}BLOCK(kingman)
