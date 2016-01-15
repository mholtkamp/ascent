#include "rect.h"
#include "constants.h"
#include "util.h"

#define COLLISION_BUFFER 0x00000010

void rect_initialize(Rect* pRect)
{
    if (pRect != 0)
    {
        pRect->fX = 0;
        pRect->fY = 0;
        pRect->fWidth = int_to_fixed(DEFAULT_RECT_WIDTH);
        pRect->fHeight = int_to_fixed(DEFAULT_RECT_HEIGHT);
    }
}

void rect_move_with_bg_collision(Rect* pRect,
                                 fixed fDX,
                                 fixed fDY)
{
    const unsigned short* pMap = (const unsigned short*) ADDR_ROOM_SBB;
    int nCollided  = 0;
    int nTileStart = 0;
    int nTileEnd   = 0;
    int nTile      = 0;
    
    // Perform x movement first
    if (fDX > 0)
    {
        // X is moving right, so check only right-most tile overlaps
        nTileStart = fixed_to_int(pRect->fX + pRect->fWidth + fDX)/PIXELS_PER_TILE + 
                    (fixed_to_int(pRect->fY)/PIXELS_PER_TILE) * SCREEN_BLOCK_MAP_WIDTH;
        nTileEnd   = fixed_to_int(pRect->fX + pRect->fWidth + fDX)/PIXELS_PER_TILE +
                    (fixed_to_int(pRect->fY + pRect->fHeight)/PIXELS_PER_TILE)*SCREEN_BLOCK_MAP_WIDTH;
        
        // Iterate through interesecting tiles
        for(nTile = nTileStart; nTile <= nTileEnd; nTile += SCREEN_BLOCK_MAP_WIDTH)
        {
            if(pMap[nTile] & BG_COLLISION_BIT)
            {
                // Set collision flag
                nCollided = 1;
                
                // Snap rect to the edge where it collided. The shifting by 3 is used instead of mult/div by 8 for performance.
                pRect->fX = int_to_fixed((fixed_to_int(pRect->fX + pRect->fWidth + fDX) >> 3) << 3) - pRect->fWidth - COLLISION_BUFFER;
                break;
            }
        }
    }
    else if (fDX < 0)
    {
        // X is moving left, so check only left-most tile overlaps
        nTileStart = fixed_to_int(pRect->fX + fDX)/PIXELS_PER_TILE + 
                    (fixed_to_int(pRect->fY)/PIXELS_PER_TILE)*SCREEN_BLOCK_MAP_WIDTH;
        nTileEnd   = fixed_to_int(pRect->fX + fDX)/PIXELS_PER_TILE + 
                    (fixed_to_int(pRect->fY + pRect->fHeight)/PIXELS_PER_TILE)*SCREEN_BLOCK_MAP_WIDTH;
        
        // Iterate through interesecting tiles
        for(nTile = nTileStart; nTile <= nTileEnd; nTile += SCREEN_BLOCK_MAP_WIDTH)
        {
            if(pMap[nTile] & BG_COLLISION_BIT)
            {
                // Set collision flag
                nCollided = 1;
                
                // Snap rect to the edge where it collided. The shifting by 3 is used instead of mult/div by 8 for performance.
                pRect->fX = int_to_fixed(((fixed_to_int(pRect->fX + fDX) >> 3) + 1) << 3) + COLLISION_BUFFER;
                break;
            }
        }
    }
    
    if (nCollided == 0)
    {
        pRect->fX += fDX;
    } 
    
    // Reset collision flag for vertical movement
    nCollided = 0;
    
    if (fDY > 0)
    {
        // Y is moving down, so check only bottom-most tile overlaps
        nTileStart = fixed_to_int(pRect->fX)/PIXELS_PER_TILE + 
                    (fixed_to_int(pRect->fY + pRect->fHeight + fDY)/PIXELS_PER_TILE)*SCREEN_BLOCK_MAP_WIDTH;
        nTileEnd   = fixed_to_int(pRect->fX + pRect->fWidth)/PIXELS_PER_TILE + 
                    (fixed_to_int(pRect->fY + pRect->fHeight + fDY)/PIXELS_PER_TILE)*SCREEN_BLOCK_MAP_WIDTH;
            
        // Iterate through interesecting tiles
        for(nTile = nTileStart; nTile <= nTileEnd; nTile++)
        {
            if(pMap[nTile] & BG_COLLISION_BIT)
            {
                // Set collision flag
                nCollided = 1;
                
                // Snap rect to the edge where it collided. The shifting by 3 is used instead of mult/div by 8 for performance.
                pRect->fY = int_to_fixed((fixed_to_int(pRect->fY + pRect->fHeight + fDY) >> 3) << 3) - pRect->fHeight - COLLISION_BUFFER;
                break;
            }
        }
    }
    else if (fDY < 0)
    {
        // Y is moving down, so check only top-most tile overlaps
        nTileStart = fixed_to_int(pRect->fX)/PIXELS_PER_TILE + 
                    (fixed_to_int(pRect->fY + fDY)/PIXELS_PER_TILE)*SCREEN_BLOCK_MAP_WIDTH;
        nTileEnd   = fixed_to_int(pRect->fX + pRect->fWidth)/PIXELS_PER_TILE + 
                    (fixed_to_int(pRect->fY + fDY)/PIXELS_PER_TILE)*SCREEN_BLOCK_MAP_WIDTH;
            
        // Iterate through interesecting tiles
        for(nTile = nTileStart; nTile <= nTileEnd; nTile++)
        {
            if(pMap[nTile] & BG_COLLISION_BIT)
            {
                // Set collision flag
                nCollided = 1;
                
                // Snap rect to the edge where it collided. The shifting by 3 is used instead of mult/div by 8 for performance.
                pRect->fY = int_to_fixed(((fixed_to_int(pRect->fY + fDY) >> 3) + 1) << 3) + COLLISION_BUFFER;
                break;
            }
        }
    }
    
    if (nCollided == 0)
    {
        pRect->fY += fDY;
    } 
}