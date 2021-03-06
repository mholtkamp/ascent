#include "bullet.h"
#include "util.h"
#include "gamedata.h"
#include "images/img_bullet.h"

int arBulletSpriteXOffsets[NUM_BULLET_TYPES] = 
{
    2 // Basic
};

int arBulletSpriteYOffsets[NUM_BULLET_TYPES] = 
{
    2 // Basic
};

fixed arBulletSpeed[NUM_BULLET_TYPES] = 
{
    4 << FIXED_SHIFT
};

void bullet_load_graphics()
{
    load_tiles(4,
               BULLET_BASIC_TILE_INDEX,
               1,
               arBulletBasicTiles);
    load_palette_bank(PALETTE_TYPE_OBJ, BULLET_HERO_PALETTE_BANK, arBulletHeroPalette);
    load_palette_bank(PALETTE_TYPE_OBJ, BULLET_ENEMY_PALETTE_BANK, arBulletEnemyPalette);
}

void bullet_initialize(Bullet* pBullet,
                       int nType,
                       int nIndex)
{
    rect_initialize(&(pBullet->rect));
    pBullet->fXVel   = 0;
    pBullet->fYVel   = 0;
    pBullet->nType   = nType;
    pBullet->nTime   = 0;
    pBullet->nOwner  = BULLET_OWNER_HERO;
    pBullet->nDamage = 1;
    pBullet->nOBJ    = BULLET_OBJ_START_INDEX + nIndex;
    pBullet->nAlive  = 1;
    
    // Pick the correct bullet update function
    switch (nType)
    {
    case BULLET_TYPE_BASIC:
        pBullet->update = &_bullet_basic_update;
        pBullet->rect.fWidth = BULLET_BASIC_WIDTH;
        pBullet->rect.fHeight = BULLET_BASIC_HEIGHT;
        pBullet->nDelay = BULLET_BASIC_ATTACK_DELAY;
        
        // Set starting tile
        sprite_set_tile(pBullet->nOBJ, BULLET_BASIC_TILE_INDEX);
        
        break;
    default:
        pBullet->update       = &_bullet_basic_update;
        pBullet->rect.fWidth  = BULLET_BASIC_WIDTH;
        pBullet->rect.fHeight = BULLET_BASIC_HEIGHT;
        pBullet->nDelay       = BULLET_BASIC_ATTACK_DELAY;
        
        // Set starting tile
        sprite_set_tile(pBullet->nOBJ, BULLET_BASIC_TILE_INDEX);
        
        break;
    }
    
    // Set sprite off-screen
    sprite_set_position(pBullet->nOBJ, 250, 0);
    
    // Set correct palette based on owner (Hero vs Enemy)
    sprite_set_palette(pBullet->nOBJ, pBullet->nOwner + BULLET_HERO_PALETTE_BANK);
    
    // Enable sprite rendering
    sprite_enable(pBullet->nOBJ, 1);
}

void _bullet_basic_update(Bullet* pBullet,
                          void* pGameData)
{
    int i = 0;
    unsigned short* pMap = (unsigned short*) ADDR_ROOM_SBB;
    unsigned short* pTile = 0;
    GameData* pData = (GameData*) pGameData;
    Enemy* pEnemies = pData->arEnemies;
    
    //GameData* pData = = (GameData*) pGameData;
    int nTile = 0;
    Rect* pRect = &(pBullet->rect);
    
    // Update position
    pBullet->rect.fX += pBullet->fXVel;
    pBullet->rect.fY += pBullet->fYVel;
    
    
    // Check if it overlaps the terrain
    // 0x03ff is the portion of screen-entry that contains tile index
    pTile = &pMap[fixed_to_int(pRect->fX + (pRect->fWidth >> 1))/PIXELS_PER_TILE + 
                 (fixed_to_int(pRect->fY + (pRect->fHeight >> 1))/PIXELS_PER_TILE) * SCREEN_BLOCK_MAP_WIDTH];
    nTile = (*pTile) & 0x03ff;
    
    // Check if it collided with a hard object
    // or if lifetime expired
    if (nTile & BG_COLLISION_BIT ||
        pBullet->nTime >= BULLET_BASIC_LIFETIME)
    {
        // Check if bullet hit a mushroom
        if (nTile == MUSHROOM_TILE_INDEX_0 ||
            nTile == MUSHROOM_TILE_INDEX_1 ||
            nTile == MUSHROOM_TILE_INDEX_2 ||
            nTile == MUSHROOM_TILE_INDEX_3)
        {
            *pTile = GROUND_TILE_INDEX;
        }
        
        // Bullet is dead now
        bullet_kill(pBullet);
        
        // Our business is done here
        return;
    }
    
    // Check if bullet overlaps an enemy
    if (pBullet->nOwner == BULLET_OWNER_HERO)
    {
        for (i = 0; i < MAX_ENEMIES; i++)
        {
            if (pEnemies[i].nAlive != 0 &&
                rect_overlap(&(pEnemies[i].rect), &(pBullet->rect)))
            {
                // Perform damage deduction
                enemy_damage(&(pEnemies[i]), pBullet->nDamage);
                bullet_kill(pBullet);
                return;
            }
        }
    }
    // Of if it overlaps the hero
    else
    {
    
    }
    
    // Lastly update sprite
    sprite_set_position(pBullet->nOBJ,
                        fixed_to_int(pBullet->rect.fX) - arBulletSpriteXOffsets[BULLET_TYPE_BASIC],
                        fixed_to_int(pBullet->rect.fY) - arBulletSpriteYOffsets[BULLET_TYPE_BASIC]);

    // Increment timer
    ++(pBullet->nTime);
}

void bullet_kill(Bullet* pBullet)
{
    pBullet->nAlive = 0;
    
    // set sprite position to off-screen
    sprite_set_position(pBullet->nOBJ, 250, 0);
    
    // disable sprite so nothing renders.
    sprite_enable(pBullet->nOBJ, 0);
}