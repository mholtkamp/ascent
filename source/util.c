#include "util.h"
#include "registers.h"
#include "constants.h"
#include "font.h"
#include  <string.h>

static unsigned short s_usKeys = 0xffff;
static unsigned int   s_unRandVal = 0;
static int s_nPrintLine = 1;

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
// poll_keys
//*****************************************************************************
 void poll_keys()
 {
    s_usKeys = ~(*((volatile unsigned short*)REG_KEYINPUT));
 }
 
//*****************************************************************************
// key_down
//*****************************************************************************
int key_down(unsigned short usKey)
{
    return s_usKeys & usKey;
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
    s_unRandVal = ((s_unRandVal * /*1103515245*/104417) + 12345);
    return (int) ((s_unRandVal >> 16) % 32768);
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
        s_unRandVal = *((volatile unsigned short*) REG_TM0D);
    }
    else
    {
        s_unRandVal = (unsigned short) nSeed;
    }
}

//*****************************************************************************
// initialize_font
//*****************************************************************************
void initialize_font()
{
    // Upload data to second charblock
    // Maybe make this configurable as a parameter.
    unsigned short* pDst = (unsigned short*) (ADDR_VRAM + CHAR_BLOCK_SIZE);
    
    // Copy tile data to Charblock 1
    memcpy(pDst, arVakkiFont, VAKKI_SIZE);
    
    // Set white color in last palette (bank #16);
    pDst = (unsigned short*) (ADDR_PALETTE_BG + PALETTE_BANK_SIZE_BYTES*15 + 2);
    
    *pDst = 0x7fff;
    
    // Initialize background 0 for printing/displaying text.
    initialize_background(0, // BG 0
                          0, // Priority? 0
                          1, // Char Base Block 1
                          0, // no mosaic
                          0, // 16/16 palette
                          16, // screen base block 16
                          0,  // Wrap, no? doesnt even work on GB0
                          0); // 0 = 32tx32t
                          
    // Clear the SBB 16 map
    pDst = (unsigned short*) (ADDR_VRAM + 16*SCREEN_BLOCK_SIZE); 
    memset(pDst, 0, SCREEN_BLOCK_SIZE);
}

//*****************************************************************************
// print
//*****************************************************************************
void print(const char* pStr)
{
    // Print out on screen like console. Does not support formatted strings.
    int nStrlen = strlen(pStr);
    int i = 0;
    
    unsigned short* pDst = (unsigned short*) (ADDR_VRAM + TEXT_SCREENBLOCK*SCREEN_BLOCK_SIZE);
    pDst += (SCREEN_ENTRIES_PER_LINE * s_nPrintLine);
    
    for (i = 0; i < nStrlen; i++)
    {
        pDst[i] = 0xf000 + (pStr[i] - ASCII_OFFSET); 
    }
    
    s_nPrintLine++;
    if (s_nPrintLine > MAX_LINES_ON_SCREEN)
    {
        s_nPrintLine = 0;
    }
}

//*****************************************************************************
// print_int
//*****************************************************************************
void print_int(int nInt)
{
    int i = 4;
    char arStr[6] = {' ',' ', ' ', ' ', ' ', 0x0};
    
    if (nInt > 99999)
    {
        arStr[0] = 'E';
        arStr[1] = 'R';
        arStr[2] = 'R';
        arStr[3] = '!';
        
        print(arStr);
        return;
    }
    else if (nInt == 0)
    {
        arStr[4] = '0';
    }
    else if (nInt < 0)
    {
        arStr[0] = 'N';
        arStr[1] = 'E';
        arStr[2] = 'G';
        arStr[3] = '!';
        
        print(arStr);
        return;
    }
    
    while (nInt != 0 &&
           i    >= 0)
    {
        arStr[i] = (nInt % 10) + '0';
        nInt = nInt/10;
        --i;
    }
        
    print(arStr);
}

//*****************************************************************************
// text
//*****************************************************************************
void text(const char* pStr,
          int nX,
          int nY)
{   
    int i       = 0;
    int nStrlen = strlen(pStr);
    
    if (nStrlen <= SCREEN_ENTRIES_PER_LINE &&
        nX >= 0                            &&
        nY >= 0                            &&
        nX < MAX_LINES_ON_SCREEN           &&
        nY < SCREEN_ENTRIES_PER_LINE)
    {
        unsigned short* pDst = (unsigned short*) (ADDR_VRAM + TEXT_SCREENBLOCK * SCREEN_BLOCK_SIZE);
        pDst += SCREEN_ENTRIES_PER_LINE * nY + nX;
        
        for (i = 0; i < nStrlen; i++)
        {
            // 0xf000 sets the palette bank to 15, which is the color white.
            *pDst = 0xf000 + (pStr[i] - ASCII_OFFSET);
            pDst++;
        }
    }
    
}

//*****************************************************************************
// clear_text
//*****************************************************************************
void clear_text()
{
    unsigned short* pDst = (unsigned short*) (ADDR_VRAM + TEXT_SCREENBLOCK * SCREEN_BLOCK_SIZE);
    
    memset(pDst, 0, SCREEN_BLOCK_SIZE);
    
    s_nPrintLine = 0;
}