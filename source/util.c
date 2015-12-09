#include "util.h"
#include "registers.h"
#include "constants.h"
#include  <string.h>

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
    *((unsigned short*) (ADDR_OAM + nIndex * OAM_OBJ_SIZE + OAM_ATTRIB1_OFFSET)) = nX;
}