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
//## Loads a full palette into video RAM.
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
//## load_palette_bank
//##
//## Loads a 16-color palette bank into video RAM.
//##
//## nPaletteType should be 0 for the tile palette and 1 for OBJ palette.
//## (see PALETTE_TYPE_BG and PALETTE_TYPE_OBJ in constants.h)
//##
//## pData should point to the palette data array that will be loaded into
//## video memory. Each color should be 2 bytes in form X1R5G5B5 and there 
//## should be 16 colors (so 32 bytes of memory).
//## **************************************************************************    
void load_palette_bank(int                   nPaletteType,
                       int                   nPaletteBank,
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

//## **************************************************************************
//## clear_all_sprites
//##
//## Clears OAM data for all sprites and sets the disabled flag to true.
//## **************************************************************************                        
void clear_all_sprites();

//## **************************************************************************
//## initialize_background
//##
//## Initializes a background. Before calling this, the background should first
//## be enabled in intialize_display.
//##
//## nBG: background index (0-3)
//## nPriority: 0-3 (3 is highest)
//## nCharBaseBlock: 0-3 (units of 16KB). Offset from start of VRAM for tiles
//## nMosaic: 0 = disabled, 1 = enabled
//## nPaletteType: 0 = 16/16, 1 = 256/1
//## nScreenBaseBlock: 0 - 31 in units of 2KB. Where to look in vid mem for map
//## nWrap: 0 = transparent overflow. 1 = wrap overflow (only works on BG2/3)
//## nScreenSize: 0 = 256x256,  1 = 512x256, 2 = 256x512, 3 = 512x512
//## **************************************************************************
void initialize_background(int nBG,
                           int nPriority,
                           int nCharBaseBlock,
                           int nMosaic,
                           int nPaletteType,
                           int nScreenBaseBlock,
                           int nWrap,
                           int nScreenSize);

//## **************************************************************************
//## load_tiles
//##
//## Loads tiles from RAM into VRAM at 0x06000000. These tiles are referenced
//## by BG maps and OBJ sprites. Right now this function only works for
//## 4 bit tiles.
//##
//## nCharBaseBlock - 0-3 for BG tiles. 4-5 for sprite tiles. Each char block
//##   is 0x4000 bytes long, so char block 1 is at 0x6000000, 2 is at 
//##   0x6004000, 3 is at 0x6008000, ... etc. Sprites must use the tiles
//##   in a sprite char block (0x6010000 or 0x6014000). A char block only 
//##   holds 512 4-byte tiles, but you can give up to an index of 1024 when
//##   giving your tilemap information, that is because you can access 
//##   the next charblock above, but BG tiles should not use sprite tiles.
//## nOffset - the offset (in tiles) from the beginning of the charblock where 
//##   the tiles should be inserted.
//## nNumTiles - the number of tiles that will be copied to VRAM.
//## pSrc - a pointer to the contiguous tile data to be copied to VRAM.
//## 
//## **************************************************************************
void load_tiles(int                   nCharBaseBlock,
                int                   nOffset,
                int                   nNumTiles,
                const unsigned short* pSrc);

//## **************************************************************************
//## load_map
//##
//## Loads a tile map from RAM into VRAM at 0x06000000. This function only
//## supports loading a 32x32 tile map. Therefore, the pointer pSrc should
//## point to 2048 bytes total, where each 2 bytes is a screen entry with
//## a tile ID, flip flags, and palette bank index. 
//## nScreenBaseBlock should be between 0 and 31. 
//##
//## Make sure the the maps and tile data do not intersect.
//## Tile CharBlock 0 overlaps Map ScreenBlock 0-7
//## Tile CharBlock 1 overlaps Map ScreenBlock 8-15
//## Tile CharBlock 2 overlaps Map ScreenBlock 16-23
//## Tile CharBlock 3s overlaps Map ScreenBlock 24-31
//## **************************************************************************
void load_map(int nScreenBaseBlock,
			  const unsigned short* pSrc);

// TODO: implement these functions, also finish the fixed point math lib...
//  and also figure out how to implement timers properly using timer registers.
int random();

int seed_random(int nSeed);



#endif