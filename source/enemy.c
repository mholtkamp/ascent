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
    pEnemy->nDamageCounter = 0;
    pEnemy->nDamage = 1;
    memset(pEnemy->arData, 0, ENEMY_SCRATCH_DATA_SIZE);
    
    // Do specific enemy initialization
    switch (nType)
    {
    case ENEMY_TYPE_CATERPILLAR:
        pEnemy->rect.fWidth = int_to_fixed(14);
        pEnemy->rect.fHeight = int_to_fixed(14);
        pEnemy->nHealth = 5;
        pEnemy->nDamage = 1;
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
    case ENEMY_TYPE_PIXIE:
        pEnemy->rect.fWidth  = int_to_fixed(8);
        pEnemy->rect.fHeight = int_to_fixed(8);
        pEnemy->nHealth = 8;
        pEnemy->nDamage = 1;
        pEnemy->update = &enemy_update_pixie;
        pEnemy->nSpriteOffsetX = 4;
        pEnemy->nSpriteOffsetY = 4;
        
        // Graphics
        nTileCount   = 8;
        pTileData    = (unsigned short*) arEnemyPixieTiles;
        pPaletteData = (unsigned short*) arEnemyPixiePalette;
        nSize        = SIZE_16x16;
        nShape       = SHAPE_16x16;
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

void enemy_clear(Enemy* pEnemy,
                 int nIndex)
{
    pEnemy->nType  = 0;
    rect_initialize(&(pEnemy->rect));
    pEnemy->nAlive = 0;
    pEnemy->nTime  = 0;
    pEnemy->nOBJ   = ENEMY_OBJ_START_INDEX + nIndex;
    pEnemy->nTile  = 0;
    pEnemy->nSpriteOffsetX = 0;
    pEnemy->nSpriteOffsetY = 0;
    pEnemy->nDamageCounter = 0;
    pEnemy->nDamage = 1;
    memset(pEnemy->arData, 0, ENEMY_SCRATCH_DATA_SIZE);
}

void enemy_damage(Enemy* pEnemy,
                  int nDamage)
{
    pEnemy->nHealth -= nDamage;
    
    if (pEnemy->nHealth <= 0)
    {
        enemy_kill(pEnemy);
    }
    else
    {
        pEnemy->nDamageCounter = ENEMY_DAMAGE_COUNTER_START;
    }
}

void enemy_kill(Enemy* pEnemy)
{   
    pEnemy->nAlive = 0;
    sprite_enable(pEnemy->nOBJ, 0);
}

void enemy_update_generic(Enemy* pEnemy,
                          void* pGameData)
{
    GameData* pData = (GameData*) pGameData;
    
    // Update damage counter
    if (pEnemy->nDamageCounter > 0)
    {
        pEnemy->nDamageCounter--;
        
        if (pEnemy->nDamageCounter == 0)
        {
            // Stop displaying alternate damage palette
            sprite_set_palette(pEnemy->nOBJ, pEnemy->nPalette);
        }
        else
        {
            sprite_set_palette(pEnemy->nOBJ, DAMAGE_PALETTE_BANK);
        }
    }
    // Update update position
    sprite_set_position(pEnemy->nOBJ,
                        fixed_to_int(pEnemy->rect.fX) - pEnemy->nSpriteOffsetX,
                        fixed_to_int(pEnemy->rect.fY) - pEnemy->nSpriteOffsetY);
                        
    // Check collision with hero
    if (rect_overlap(&(pEnemy->rect), &(pData->hero.rect)))
    {
        hero_damage(&(pData->hero), pEnemy->nDamage);
    }
                        
    // Increment timer
    pEnemy->nTime++;
}

void enemy_update_caterpillar(Enemy* pEnemy,
                              void* pGameData)
{
    // SCRATCH DATA:
    // (int) arData + 0 = nDirection
    // (int) arData + 1 = nRedirectTime;
    
    static const fixed SPEED   = 1 << FIXED_SHIFT;
    
    fixed fDX = 0;
    fixed fDY = 0;
    int nVert = 0;
    int nCollided = 0;
    
    int nDirection    = *(pEnemy->arData);
    int nRedirectTime = *(pEnemy->arData + 1);
    
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
    nCollided = rect_move_with_bg_collision(&(pEnemy->rect), fDX, fDY);
    
    sprite_flip(pEnemy->nOBJ,
                nDirection == DIR_RIGHT,
                nDirection == DIR_DOWN);
    
    // Vert flag used to get correct sprite
    nVert = (nDirection == DIR_UP) || (nDirection == DIR_DOWN);
    
    sprite_set_tile(pEnemy->nOBJ,
                    pEnemy->nTile + nVert*4);
                
    enemy_update_generic(pEnemy, pGameData);
    
    // If caterpillar collided with BG, reroll the direction
    if (nCollided != 0)
    {
        nDirection = random() % 4;
        *((int*) pEnemy->arData) = nDirection;
    }
}

void enemy_update_pixie(Enemy* pEnemy,
                        void* pGameData)
{
    // SCRATCH DATA:
    // (int)   arData + 0 = nShootTime;
    // (int)   arData + 1 = nRedirectTime;
    // (fixed) arData + 2 = fMoveTargetX;
    // (fixed) arData + 3 = fMoveTargetY;
    
    static fixed SPEED = 0x00008000;
    GameData* pData = (GameData*) pGameData;
    fixed fDX = 0;
    fixed fDY = 0;
    
    // Read scratch data
    int   nShootTime    = *(pEnemy->arData);
    int   nRedirectTime = *(pEnemy->arData + 1);
    fixed fMoveTargetX  = (fixed) *(pEnemy->arData + 2);
    fixed fMoveTargetY  = (fixed) *(pEnemy->arData + 3);
    
    
    // If it is time to redirect, then find a new target location
    if (pEnemy->nTime == 0 ||
        pEnemy->nTime == nRedirectTime)
    {
        nRedirectTime += 60 + random() % 50;
        
        fMoveTargetX = pData->hero.rect.fX + int_to_fixed(-40 + random() % 80);
        fMoveTargetY = pData->hero.rect.fY + int_to_fixed(-40 + random() % 80);
    }
    
    // Move towards target location
    fDX = fMoveTargetX - pEnemy->rect.fX;
    fDY = fMoveTargetY - pEnemy->rect.fY;
    
    if (fDX > int_to_fixed(1))
    {
        pEnemy->rect.fX += SPEED;
    }
    else if (fDX < int_to_fixed(-1))
    {
        pEnemy->rect.fX -= SPEED;
    }
    
    if (fDY > int_to_fixed(1))
    {
        pEnemy->rect.fY += SPEED;
    }
    else if (fDY < int_to_fixed(-1))
    {
        pEnemy->rect.fY -= SPEED;
    }
    
    sprite_set_tile(pEnemy->nOBJ,
                    pEnemy->nTile + 4*((pEnemy->nTime & 0x10) != 0));
    
    // Write back scratch data
    *(pEnemy->arData)              = nShootTime;
    *(pEnemy->arData + 1)          = nRedirectTime;
    *((fixed*) pEnemy->arData + 2) = fMoveTargetX;
    *((fixed*) pEnemy->arData + 3) = fMoveTargetY;
    
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