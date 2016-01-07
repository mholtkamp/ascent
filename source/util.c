#include "util.h"
#include "registers.h"
#include "constants.h"
#include  <string.h>

static unsigned short s_usRandVal = 0;

//*****************************************************************************
// initialize_util
//*****************************************************************************
void initialize_util()
{
    // Start up the timer so that it can be used in seed_random().
    unsigned short* pTimerData = (unsigned short*) REG_TM0D;
    unsigned short* pTimerCont = (unsigned short*) REG_TM0CNT;
    
    // Reset counter starting position to 0
    *pTimerData = 0x0;
    // Enable timer (bit 7)
    *pTimerCont = (1 << 7);
}

//*****************************************************************************
// initialize_display
//*****************************************************************************
void initialize_display(int nVideoMode,
                        int nDisplayFrame,
                        int nHBlankFree,
                        int nSpriteMappingMode,
                        int nForcedBlank,
                        int nBG0Enable,
                        int nBG1Enable,
                        int nBG2Enable,
                        int nBG3Enable,
                        int nOBJEnable)
{
    unsigned short usRegVal = 0;
    
    // Create the value to set the display register.
    usRegVal |= nVideoMode;
    usRegVal |= nDisplayFrame << 4;
    usRegVal |= nHBlankFree << 5;
    usRegVal |= nSpriteMappingMode << 6;
    usRegVal |= nForcedBlank << 7;
    usRegVal |= nBG0Enable << 8;
    usRegVal |= nBG1Enable << 9;
    usRegVal |= nBG2Enable << 10;
    usRegVal |= nBG3Enable << 11;
    usRegVal |= nOBJEnable << 12;
    
    // Load the value into the display register (0x04000000)
    *((unsigned short*) REG_DISPCNT) = usRegVal;
    
}

//*****************************************************************************
// sync_with_vblank
//*****************************************************************************
void sync_with_vblank()
{
    while(*((volatile unsigned short*)REG_VCOUNT) >= 160);   // wait till VDraw
    while(*((volatile unsigned short*)REG_VCOUNT) < 160);    // wait till VBlank
}

//*****************************************************************************
// key_down
//*****************************************************************************
int key_down(unsigned short usKey)
{
    return (~*((volatile unsigned short*)REG_KEYINPUT)) & usKey;
}

//*****************************************************************************
// load_palette
//*****************************************************************************
void load_palette(int                   nPaletteType,
                  const unsigned short* pData)
{
    if (nPaletteType == PALETTE_TYPE_BG)
    {
        memcpy((unsigned short*) ADDR_PALETTE_BG,
                pData,
                FULL_PALETTE_SIZE * sizeof(unsigned short));
    }
    else
    {
        memcpy((unsigned short*) ADDR_PALETTE_OBJ,
                pData,
                FULL_PALETTE_SIZE * sizeof(unsigned short));
    }
}

//*****************************************************************************
// load_palette_bank
//*****************************************************************************
void load_palette_bank(int                   nPaletteType,
                       int                   nPaletteBank,
                       const unsigned short* pData)
{
    if (nPaletteType == PALETTE_TYPE_BG)
    {
        memcpy((unsigned short*) (ADDR_PALETTE_BG + nPaletteBank*PALETTE_BANK_SIZE_BYTES),
                pData,
                PALETTE_BANK_SIZE * sizeof(unsigned short));
    }
    else
    {
        memcpy((unsigned short*) (ADDR_PALETTE_OBJ + nPaletteBank*PALETTE_BANK_SIZE_BYTES),
                pData,
                PALETTE_BANK_SIZE * sizeof(unsigned short));
    }
}    

//*****************************************************************************
// sprite_initialize
//*****************************************************************************
void sprite_initialize(int nIndex,
                       int nOBJMode,
                       int nMosaic,
                       int nPaletteStyle,
                       int nShape,
                       int nSize,
                       int nHorizontalFlip,
                       int nVerticalFlip,
                       int nX,
                       int nY,
                       int nTile,
                       int nPriority,
                       int nPaletteBank)
{
    unsigned short usRegVal = 0;
    
    // Set the value of attrib 0. No need to mask since the entire register
    // is being set all at once.
    usRegVal |= nY;
    usRegVal |= nOBJMode      << 10;
    usRegVal |= nMosaic       << 12;
    usRegVal |= nPaletteStyle << 13;
    usRegVal |= nShape        << 14;
    
    *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB0_OFFSET)) = usRegVal;
    
    // Set value of attrib 1
    usRegVal = 0;
    
    usRegVal |= nX;
    usRegVal |= nHorizontalFlip << 12;
    usRegVal |= nVerticalFlip   << 13;
    usRegVal |= nSize           << 14;
    
    *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB1_OFFSET)) = usRegVal;
    
    // Set value of attrib 2
    usRegVal = 0;
    
    usRegVal |= nTile;
    usRegVal |= nPriority    << 10;
    usRegVal |= nPaletteBank << 12;
    
    *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB2_OFFSET)) = usRegVal;
}

//*****************************************************************************
// sprite_set_position
//*****************************************************************************
void sprite_set_position(int nIndex,
                         int nX,
                         int nY)
{
    // Update first register (Y value).
    unsigned short usRegVal = *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB0_OFFSET)) & 0xff00;
    usRegVal |= (0x00ff & nY);
    *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB0_OFFSET)) = usRegVal;
    
    // Update second register (X value)
    usRegVal = *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB1_OFFSET)) & 0xfe00;
    usRegVal |= (0x01ff & nX);
    *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB1_OFFSET)) = usRegVal;
}

//*****************************************************************************
// sprite_set_tile
//*****************************************************************************
void sprite_set_tile(int nIndex,
                     int nTile)
 {
    unsigned short usRegVal = *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB2_OFFSET)) & 0xfe00;
    usRegVal |= (0x01ff & nTile);
    *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB2_OFFSET)) = usRegVal;   
 }
 
//*****************************************************************************
// sprite_set_dimensions
//*****************************************************************************
 void sprite_set_dimensions(int nIndex,
                           int nSize,
                           int nShape)
{
    // Set size in OAM attrib 0
    unsigned short usRegVal = *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB0_OFFSET)) & 0x3fff;
    usRegVal |= (0x3fff & (nShape << 14));
    *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB0_OFFSET)) = usRegVal;
    
    usRegVal = *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB1_OFFSET)) & 0x3fff;
    usRegVal |= (0x3fff & (nSize << 14));
    *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB1_OFFSET)) = usRegVal;
}

//*****************************************************************************
// sprite_flip
//*****************************************************************************
void sprite_flip(int nIndex,
                 int nHorizontalFlip,
                 int nVerticalFlip)
{
    unsigned short usRegVal = *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB1_OFFSET)) & 0xcfff;
    usRegVal |= (0x1000 & (nHorizontalFlip << 12)) | (0x2000 & (nVerticalFlip << 13));
    *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB1_OFFSET)) = usRegVal;
}

//*****************************************************************************
// sprite_enable
//*****************************************************************************
void sprite_enable(int nIndex,
                   int nEnable)
{
    // The value of the enable is NOT'd because 0 = enabled on OBJ attrib register.
    unsigned short usRegVal = *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB0_OFFSET)) & 0xfdff;
    usRegVal |= 0x0200 & (~nEnable << 9);
    *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB0_OFFSET)) = usRegVal;
}

//*****************************************************************************
// sprite_set_palette
//*****************************************************************************
void sprite_set_palette(int nIndex,
                        int nPalette)
{
    unsigned short usRegVal = *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB2_OFFSET)) & 0x0fff;
    usRegVal |= 0xF000 & (nPalette << 12);
    *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB2_OFFSET)) = usRegVal;
}

//*****************************************************************************
// clear_all_sprites
//*****************************************************************************
void clear_all_sprites()
{
    // Doing this the slow way, but this will really only be called once
    // at the beginning of a program/level to disable all sprites that 
    // aren't being used.
    int i = 0;
    unsigned short* pSpriteReg = 0;
    
    for (i = 0; i < MAX_SPRITES; i++)
    {
        pSpriteReg = (unsigned short*) (ADDR_OAM + i*OAM_OBJ_SIZE);
        *pSpriteReg = 0x0200;
        ++pSpriteReg;
        *pSpriteReg = 0x0;
        ++pSpriteReg;
        *pSpriteReg = 0x0;
    }
}

//*****************************************************************************
// initialize_background
//*****************************************************************************
void initialize_background(int nBG,
                           int nPriority,
                           int nCharBaseBlock,
                           int nMosaic,
                           int nPaletteType,
                           int nScreenBaseBlock,
                           int nWrap,
                           int nScreenSize)
{
    unsigned short  usRegVal = 0;
    unsigned short* pBGReg   = 0;
    
    // Find the correct register to change
    switch (nBG)
    {
    case BG_0:
        pBGReg = (unsigned short*) BG0CNT;
        break;
    case BG_1:
        pBGReg = (unsigned short*) BG1CNT;
        break;
    case BG_2:
        pBGReg = (unsigned short*) BG2CNT;
        break;
    case BG_3:
        pBGReg = (unsigned short*) BG3CNT;
        break;
    default:
        // Unknown BG index was supplied...
        return;
    }
    
    usRegVal |= nPriority;
    usRegVal |= nCharBaseBlock << 2;
    usRegVal |= nMosaic << 6;
    usRegVal |= nPaletteType << 7;
    usRegVal |= nScreenBaseBlock << 8;
    usRegVal |= nWrap << 13;
    usRegVal |= nScreenSize << 14;
    
    *pBGReg = usRegVal;
}

//*****************************************************************************
// load_tiles
//*****************************************************************************
void load_tiles(int                   nCharBaseBlock,
                int                   nOffset,
                int                   nNumTiles,
                const unsigned short* pSrc)
{
    unsigned short* pDst = (unsigned short*) (ADDR_VRAM + nCharBaseBlock*CHAR_BLOCK_SIZE + TILE_4_SIZE*nOffset);
    
    memcpy(pDst, pSrc, nNumTiles * TILE_4_SIZE);
}

//*****************************************************************************
// load_map
//*****************************************************************************
void load_map(int nScreenBaseBlock,
			  const unsigned short* pSrc)
{
    unsigned short* pDst = (unsigned short*) (ADDR_VRAM + nScreenBaseBlock*SCREEN_BLOCK_SIZE);
    
    memcpy(pDst, pSrc, SCREEN_BLOCK_SIZE);
}

//*****************************************************************************
// random
//*****************************************************************************
int random()
{
    // This RNG math is how rand is implemented in gcc, or so I've been told.
    s_usRandVal = ((s_usRandVal * 104417) + 12345);
    return (s_usRandVal % 32768);
}

//*****************************************************************************
// seed_random
//*****************************************************************************
void seed_random(int nSeed)
{
    if (nSeed == -1)
    {
        // Seed the random number generator with the timer value.
        // This provides the most randomness, but on certain emulators,
        // the timer values can't seem to be read.
        s_usRandVal = *((volatile unsigned short*) REG_TM0D);
        
    }
    else
    {
        s_usRandVal = (unsigned short) nSeed;
    }
}
