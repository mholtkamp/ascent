#include <string.h>
#include "gamedata.h"
#include "util.h"
#include "images/img_enemy.h"

// Arrays to keep track of loaded graphics data
static int s_arLoaded[MAX_ENEMY_TYPES_PER_ROOM]           = {0};
static int s_arLoadedTiles[MAX_ENEMY_TYPES_PER_ROOM]      = {0};
static int s_arLoadedTileCounts[MAX_ENEMY_TYPES_PER_ROOM] = {0};
static int s_arLoadedPalettes[MAX_ENEMY_TYPES_PER_ROOM]   = {0};

void enemy_initialize(Enemy* pEnemy,
                      int nType,
                      int nIndex)
{
    int i          = 0;
    int nLoaded    = 0;
    
    int             nTileCount   = 0;
    unsigned short* pTileData    = 0;
    unsigned short* pPaletteData = 0;
    
    int nShape = 0;
    int nSize  = 0;
    
    pEnemy->nType = nType;
    rect_initialize(&(pEnemy->rect));
    pEnemy->nAlive = 1;
    pEnemy->nTime  = 0;
    pEnemy->nOBJ   = ENEMY_OBJ_START_INDEX + nIndex;
    pEnemy->nTile  = 0;
    memset(pEnemy->arData, 0, ENEMY_SCRATCH_DATA_SIZE);
    
    // Do specific enemy initialization
    switch (nType)
    {
    case ENEMY_TYPE_CATERPILLAR:
        pEnemy->rect.fWidth = int_to_fixed(14);
        pEnemy->rect.fHeight = int_to_fixed(14);
        pEnemy->nHealth = 5;
        pEnemy->update = &enemy_update_caterpillar;
        pEnemy->nSpriteOffsetX = 1;
        pEnemy->nSpriteOffsetY = 1;
        
        // Graphics
        nTileCount   = 8;
        pTileData    = (unsigned short*) arEnemyCaterpillarTiles;
        pPaletteData = (unsigned short*) arEnemyCaterpillarPalette;
        nSize        = SIZE_16x16;
        nShape       = SHAPE_16x16;
        break;
    default:
        break;
    }
    
    // Figure out if graphics data needs to be loaded...
    for (i = 0; i < MAX_ENEMY_TYPES_PER_ROOM; i++)
    {
        if(s_arLoaded[i] == pEnemy->nType)
        {
            // Enemy graphics were already loaded.
            nLoaded = 1;
            break;
        }
        else if (s_arLoaded[i] == -1)
        {
            // Already hit an open slot and have not seen pEnemy->nType
            // listed in the array yet... So the graphics need to be loaded 
            // at this next open slot.
            nLoaded = 0;
            
            // But set the static load flag at index i, because it will be loaded
            // later in the function
            s_arLoaded[i] = pEnemy->nType;
            break;
        }
    }
    
    
    if (i >= MAX_ENEMY_TYPES_PER_ROOM)
    {
        print("ERROR. TOO MANY ENEMY TYPES.");
        return;
    }
    else if (nLoaded == 0)
    {
        // Need to load graphics data.
        
        // First get all the relevant indices
        if (i == 0)
        {
            // Special case for first enemy being loaded
            s_arLoadedTiles[i] = ENEMY_TILE_START_INDEX;
            s_arLoadedTileCounts[i] = nTileCount;
            s_arLoadedPalettes[i] = ENEMY_PALETTE_BANK_START;
        }
        else
        {
            // i - 1 is a valid array index. So the generic code below works
            s_arLoadedTiles[i] = s_arLoadedTiles[i-1] + s_arLoadedTileCounts[i-1];
            s_arLoadedTileCounts[i] = nTileCount;
            s_arLoadedPalettes[i] = ENEMY_PALETTE_BANK_START + i;
        }
        
        // Now to actually put data in VRAM
        load_tiles(SPRITE_CBB,
                   s_arLoadedTiles[i],
                   s_arLoadedTileCounts[i],
                   pTileData);
        load_palette_bank(PALETTE_TYPE_OBJ,
                          s_arLoadedPalettes[i],
                          pPaletteData);
    }
    
    // Save the tile index so that the update method can change
    // it for animation. Also save the palette so palette can
    // be swapped in the update function
    pEnemy->nTile    = s_arLoadedTiles[i];
    pEnemy->nPalette = s_arLoadedPalettes[i];
    
    // Lastly, initialize sprite OBJ data with proper data.    
    sprite_initialize(pEnemy->nOBJ,
                      0,  // regular mode. not affine
                      0,  // no mosaic
                      0,  // 16/16 palette
                      nShape,
                      nSize,
                      0, // hori flip controlled in update function
                      0, // vert flip controlled in update function
                      250,
                      0,
                      pEnemy->nTile,
                      1, // priority 1 (behind text)
                      pEnemy->nPalette);
}

void enemy_clear(Enemy*pEnemy,
                 int nIndex)
{
    pEnemy->nType = 0;
    rect_initialize(&(pEnemy->rect));
    pEnemy->nAlive = 0;
    pEnemy->nTime  = 0;
    pEnemy->nOBJ   = ENEMY_OBJ_START_INDEX + nIndex;
    pEnemy->nTile  = 0;
    pEnemy->nSpriteOffsetX = 0;
    pEnemy->nSpriteOffsetY = 0;
    memset(pEnemy->arData, 0, ENEMY_SCRATCH_DATA_SIZE);
}

void enemy_kill(Enemy* pEnemy)
{   
    pEnemy->nAlive = 0;
    sprite_enable(pEnemy->nOBJ, 0);
}

void enemy_update_generic(Enemy* pEnemy,
                          void* pGameData)
{
    // Update update position
    sprite_set_position(pEnemy->nOBJ,
                        fixed_to_int(pEnemy->rect.fX) - pEnemy->nSpriteOffsetX,
                        fixed_to_int(pEnemy->rect.fY) - pEnemy->nSpriteOffsetY);
                        
    // Increment timer
    pEnemy->nTime++;
}

void enemy_update_caterpillar(Enemy* pEnemy,
                              void* pGameData)
{
    static const fixed SPEED   = 2 << FIXED_SHIFT;
    
    fixed fDX = 0;
    fixed fDY = 0;
    int nVert = 0;
    // SCRATCH DATA:
    // (int) arData + 0 = nDirection
    // (int) arData + 4 = nRedirectTime;
    
    int nDirection    = *((int*) pEnemy->arData);
    int nRedirectTime = *((int*) pEnemy->arData + 4);
    
    if (pEnemy->nTime         == 0 ||
        pEnemy->nTime         == nRedirectTime)
    {
        // Reroll redirect time
        // 50 = minimum time to change directions
        // 30 = variance
        nRedirectTime += 50 + random() % 30;
        *((int*) pEnemy->arData + 4) = nRedirectTime;
        
        // Reroll the moving direction
        // There are only 4 possible directions the caterpillar can move in
        nDirection = random() % 4;
        *((int*) pEnemy->arData) = nDirection;
    }
    
    // Move the rect based on direction
    switch (nDirection)
    {
    case DIR_RIGHT:
        fDX = SPEED;
        break;
    case DIR_DOWN:
        fDY = SPEED;
        break;
    case DIR_LEFT:
        fDX = -SPEED;
        break;
    case DIR_UP:
        fDY = -SPEED;
        break;
    default:
        break;
    }
    
    // Move caterpillar
    rect_move_with_bg_collision(&(pEnemy->rect), fDX, fDY);
    
    sprite_flip(pEnemy->nOBJ,
                nDirection == DIR_RIGHT,
                nDirection == DIR_DOWN);
    
    // Vert flag used to get correct sprite
    nVert = (nDirection == DIR_UP) || (nDirection == DIR_DOWN);
    
    sprite_set_tile(pEnemy->nOBJ,
                    pEnemy->nTile + nVert*4);
                
    enemy_update_generic(pEnemy, pGameData);
}

void reset_room_enemy_data(void* pGameData)
{
    // This should be called every time a room is loaded
    // so that the enemy graphics are loaded properly.
    int i = 0;
    
    GameData* pData = (GameData*) pGameData;
    
    for (i = 0; i < MAX_ENEMY_TYPES_PER_ROOM; i++)
    {
        s_arLoaded[i]           = -1;
        s_arLoadedTiles[i]      =  0;
        s_arLoadedTileCounts[i] =  0;
        s_arLoadedPalettes[i]   =  0;
        
        // Kill any living enemies (probably shouldn't happen
        // unless the ability to bomb doors to leave a room before
        // killing all enemies is implemented).
        enemy_kill(&(pData->arEnemies[i]));
    }
}