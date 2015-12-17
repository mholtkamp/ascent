#ifndef UTIL_H
#define UTIL_H

//## **************************************************************************
//## initialize_display
//##
//## Sets all bits of the display control register except for the window bits.
//## **************************************************************************
void initialize_display(int nVideoMode,
                        int nDisplayFrame,
                        int nHBlankFree,
                        int nSpriteMappingMode,
                        int nForcedBlank,
                        int nBG0Enable,
                        int nBG1Enable,
                        int nBG2Enable,
                        int nBG3Enable,
                        int nOBJEnable);
                       
//## **************************************************************************
//## sync_with_vblank
//##
//## Waits until the start of the next vblank section.
//## **************************************************************************                      
 void sync_with_vblank();
 
//## **************************************************************************
//## key_down
//##
//## Returns 1 if key is currently down, 0 otherwise.
//## usKey should be a key value defined in constants.h (i.e. KEY_A, KEY_B...)
//## **************************************************************************
int key_down(unsigned short usKey);

//## **************************************************************************
//## load_palette
//##
//## Loads a palette into video RAM.
//##
//## nPaletteType should be 0 for the tile palette and 1 for OBJ palette.
//## (see PALETTE_TYPE_BG and PALETTE_TYPE_OBJ in constants.h)
//##
//## pData should point to the palette data array that will be loaded into
//## video memory. Each color should be 2 bytes in form X1R5G5B5 and there 
//## should be 256 colors (so 512 bytes of memory).
//## **************************************************************************    
void load_palette(int                   nPaletteType,
                  const unsigned short* pData);

//## **************************************************************************
//## sprite_initialize
//##
//## Initializes a sprite at nIndex = 0-127.
//## **************************************************************************
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
                       int nPaletteBank);

//## **************************************************************************
//## sprite_set_position
//##
//## Soley updates the position of a sprite.
//## **************************************************************************
void sprite_set_position(int nIndex,
                         int nX,
                         int nY);

//## **************************************************************************
//## sprite_set_tile
//##
//## Soley updates the starting tile of sprite graphic.
//## **************************************************************************
void sprite_set_tile(int nIndex,
                     int nTile);

//## **************************************************************************
//## sprite_set_dimensions
//##
//## Soley updates the size and shape of a sprite.
//## **************************************************************************
void sprite_set_dimensions(int nIndex,
                           int nSize,
                           int nShape);

//## **************************************************************************
//## sprite_flip
//##
//## Flips the sprite, mirroring it along x axis (horizontally) or along the
//## y axis (vertically).
//## **************************************************************************
void sprite_flip(int nIndex,
                 int nHorizontalFlip,
                 int nVerticalFlip);

//## **************************************************************************
//## sprite_enable
//##
//## Enables/disables the sprite rending.
//## **************************************************************************
void sprite_enable(int nIndex,
                   int nEnable);
                   
//## **************************************************************************
//## sprite_set_palette
//##
//## Sets the sprite's palette bank. The nPalette parameter chooses the 
//## 16-color palette bank to use. nPalette should be in range 0-15.
//## **************************************************************************
void sprite_set_palette(int nIndex,
                        int nPalette);


#endif