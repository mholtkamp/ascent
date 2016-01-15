#include "bullet.h"
#include "util.h"
#include "gamedata.h"
#include "images/img_bullet.h"

#define BULLET_HERO_PALETTE_BANK 1
#define BULLET_ENEMY_PALETTE_BANK 2

int arBulletSpriteXOffsets[NUM_BULLET_TYPES] = 
{
    2 // Basic
};

int arBulletSpriteYOffsets[NUM_BULLET_TYPES] = 
{
    2 // Basic
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
    pBullet->nOBJ = BULLET_OBJ_START_INDEX + nIndex;
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
    unsigned short* pMap = (unsigned short*) ADDR_ROOM_SBB;
    //GameData* pData = = (GameData*) pGameData;
    int nTile = 0;
    Rect* pRect = &(pBullet->rect);
    
    // Update position
    pBullet->rect.fX += pBullet->fXVel;
    pBullet->rect.fY += pBullet->fYVel;
    
    
    // Check if it overlaps the terrain
    nTile = pMap[fixed_to_int(pRect->fX + (pRect->fWidth >> 1))/PIXELS_PER_TILE + 
                (fixed_to_int(pRect->fY + (pRect->fHeight >> 1))/PIXELS_PER_TILE) * SCREEN_BLOCK_MAP_WIDTH];
    
    // Check if it collided with hard object
    if (nTile & BG_COLLISION_BIT)
    {
        // Bullet is dead now
        bullet_kill(pBullet);
        
        // Our business is done here
        return;
    }
    
    // Check if bullet overlaps a hero
    
    // OR check if bullet overlaps an enemy
    
    // Lastly update sprite
    sprite_set_position(pBullet->nOBJ,
                        fixed_to_int(pBullet->rect.fX) - arBulletSpriteXOffsets[BULLET_TYPE_BASIC],
                        fixed_to_int(pBullet->rect.fY) - arBulletSpriteYOffsets[BULLET_TYPE_BASIC]);
}

void bullet_kill(Bullet* pBullet)
{
    pBullet->nAlive = 0;
    
    // set sprite position to off-screen
    sprite_set_position(pBullet->nOBJ, 250, 0);
    
    // disable sprite so nothing renders.
    sprite_enable(pBullet->nOBJ, 0);
}