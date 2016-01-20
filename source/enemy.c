#include "gamedata.h"
#include "util.h"
#include "images/img_enemy.h"

// Arrays to keep track of loaded graphics data
static int s_arLoaded[MAX_ENEMIES_PER_ROOM]           = {0};
static int s_arLoadedTiles[MAX_ENEMIES_PER_ROOM]      = {0};
static int s_arLoadedTileCounts[MAX_ENEMIES_PER_ROOM] = {0};
static int s_arLoadedPalettes[MAX_ENEMIES_PER_ROOM]   = {0};

void enemy_initialize(Enemy* pEnemy,
                      int nType,
                      int nIndex)
{
    int i          = 0;
    int nLoaded    = 0;
    
    int             nTileCount   = 0;
    unsigned short* pTileData    = 0;
    unsigned short* pPaletteData = 0;
    
    pEnemy->nType = nType;
    rect_initialize(&(pEnemy->rect));
    pEnemy->nAlive = 1;
    pEnemy->nTime  = 0;
    pEnemy->fSpeed = int_to_fixed(1);
    pEnemy->nOBJ   = ENEMY_OBJ_START_INDEX + nIndex;
    pEnemy->nTile  = 0;
    
    // Do specific enemy initialization
    switch (nType)
    {
    case ENEMY_TYPE_CATERPILLAR:
        pEnemy->rect.fWidth = int_to_fixed(14);
        pEnemy->rect.fHeight = int_to_fixed(14);
        pEnemy->nHealth = 5;
        pEnemy->update = &enemy_update_caterpillar;
        
        // Graphics
        nTileCount   = 8;
        pTileData    = (unsigned short*) arEnemyCaterpillarTiles;
        pPaletteData = (unsigned short*) arEnemyCaterpillarPalette;
        break;
    default:
        
        break;
    }
    
    // Figure out if graphics data needs to be loaded...
    for (i = 0; i < MAX_ENEMIES_PER_ROOM; i++)
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
            s_arLoaded[i] = 1;
            break;
        }
    }
    
    
    if (i >= MAX_ENEMIES_PER_ROOM)
    {
        print("ERROR. TOO MANY ENEMIES.");
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
    // it for animation.
    pEnemy->nTile = s_arLoadedTiles[i];
    
    // Lastly, set the proper tile indices and palette bank 
    // for the OBJ to lookup
    sprite_set_palette(pEnemy->nOBJ,
                       s_arLoadedPalettes[i]);
    sprite_set_tile(pEnemy->nOBJ,
                    s_arLoadedTiles[i]);
}

void enemy_update_caterpillar(Enemy* pEnemy,
                              void* pGameData)
{
    
}

void reset_room_enemy_data()
{
    // This should be called every time a room is loaded
    // so that the enemy graphics are loaded properly.
    int i = 0;
    
    for (i = 0; i < MAX_ENEMIES_PER_ROOM; i++)
    {
        s_arLoaded[i]           = -1;
        s_arLoadedTiles[i]      =  0;
        s_arLoadedTileCounts[i] =  0;
        s_arLoadedPalettes[i]   =  0;
    }
}