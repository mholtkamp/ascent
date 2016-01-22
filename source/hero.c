#include "util.h"
#include "hero.h"
#include "rect.h"
#include "constants.h"
#include "fixedpoint.h"
#include "images/img_hero.h"
#include "gamedata.h"
#include "bullet.h"

#define HERO_MOVESPEED 1

#define HERO_DIR_RIGHT 0x01
#define HERO_DIR_LEFT  0x02
#define HERO_DIR_DOWN  0x04
#define HERO_DIR_UP    0x08

static int s_nDir = 0;
static fixed s_fMoveSpeed[HERO_MAX_SPEED] = {0, 0, 0, 0,
                                             0, 0, 0, 0};
                                             
void hero_static_initialize(Hero* pHero)
{
    // Create move-speed table
    s_fMoveSpeed[0] = float_to_fixed(0.8f);
    s_fMoveSpeed[1] = float_to_fixed(1.0f);
    s_fMoveSpeed[2] = float_to_fixed(1.4f);
    s_fMoveSpeed[3] = float_to_fixed(1.8f);
    s_fMoveSpeed[4] = float_to_fixed(2.2f);
    s_fMoveSpeed[5] = float_to_fixed(2.4f);
    s_fMoveSpeed[6] = float_to_fixed(2.6f);
    s_fMoveSpeed[7] = float_to_fixed(2.8f);
    
    // Load the hero OBJ tiles
    load_tiles(SPRITE_CBB,           // CBB 4
               HERO_HORI_TILE_INDEX, // 0 offset
               4,                    // 4 tiles to load
               arHeroHoriTiles);
    load_tiles(SPRITE_CBB,            // CBB 4
               HERO_VERT_TILE_INDEX,  // Offset is 4 tiles in
               4,                     // 4 tiles to load
               arHeroVertTiles);
    load_tiles(SPRITE_CBB,            // CBB 4
               HERO_DIAG_TILE_INDEX,  // offset is 8 tiles in 
               4,                     // load 4 tiles
               arHeroDiagTiles);
               
    // Load the hero palette into bank 0
    load_palette_bank(PALETTE_TYPE_OBJ, HERO_PALETTE_BANK, arHeroPalette);
    load_palette_bank(PALETTE_TYPE_OBJ, DAMAGE_PALETTE_BANK, arDamagePalette);
    
    sprite_enable(HERO_SPRITE_INDEX, 1);
    
    sprite_initialize(HERO_SPRITE_INDEX,
                      0,
                      0,
                      0,
                      0,
                      1,
                      0,
                      0,
                      120,
                      60,
                      HERO_HORI_TILE_INDEX,
                      1,
                      0);
                      
    pHero->rect.fX = int_to_fixed(120);
    pHero->rect.fY = int_to_fixed(60);
}

void hero_initialize(Hero* pHero)
{
    if (pHero != 0)
    {
        pHero->nMaxHealth   = HERO_STARTING_HEALTH;
        pHero->nHealth      = HERO_STARTING_HEALTH;
        pHero->nGems        = HERO_STARTING_GEMS;
        pHero->nDamage      = HERO_STARTING_DAMAGE;
        pHero->nSpeed       = HERO_STARTING_SPEED;
        pHero->nAttackDelay = BULLET_BASIC_ATTACK_DELAY;
        pHero->nBullet      = BULLET_TYPE_BASIC;
        pHero->nDamageCounter = 0;
        
        pHero->rect.fX      = 0;
        pHero->rect.fY      = 0;
        pHero->rect.fWidth  = int_to_fixed(HERO_RECT_WIDTH);
        pHero->rect.fHeight = int_to_fixed(HERO_RECT_HEIGHT);
    }   
}

void hero_update(Hero* pHero,
                 void* pGameData)
{   
    int nKeyDir  = 0;
    int nTile = 0;
    int nFlipX = 0;
    int nFlipY = 0;
    
    fixed fDX = 0;
    fixed fDY = 0;
    
    // Perform movement first
    if (key_down(KEY_RIGHT))
    {
        fDX = s_fMoveSpeed[pHero->nSpeed];
        nKeyDir |= HERO_DIR_RIGHT;
    }
    if (key_down(KEY_LEFT))
    {
        fDX = -s_fMoveSpeed[pHero->nSpeed];
        nKeyDir |= HERO_DIR_LEFT;
    }
    if (key_down(KEY_UP))
    {
        fDY = -s_fMoveSpeed[pHero->nSpeed];
        nKeyDir |= HERO_DIR_UP;
    }
    if (key_down(KEY_DOWN))
    {
        fDY = s_fMoveSpeed[pHero->nSpeed];
        nKeyDir |= HERO_DIR_DOWN;
    }
    
    if (nKeyDir != 0 &&
        !key_down(KEY_R))
    {
        s_nDir   = nKeyDir;
    }
    
    // Update sprite based on s_nDir
    switch(s_nDir)
    {
    case HERO_DIR_RIGHT:
        nTile  = HERO_HORI_TILE_INDEX;
        break;
    case HERO_DIR_DOWN:
        nTile  = HERO_VERT_TILE_INDEX;
        nFlipY = 1;
        break;
    case HERO_DIR_LEFT:
        nTile  = HERO_HORI_TILE_INDEX;
        nFlipX = 1;
        break;
    case HERO_DIR_UP:
        nTile = HERO_VERT_TILE_INDEX;
        break;
    case HERO_DIR_DOWN | HERO_DIR_RIGHT:
        nTile  = HERO_DIAG_TILE_INDEX;
        nFlipY = 1;
        break;
    case HERO_DIR_DOWN | HERO_DIR_LEFT:
        nTile  = HERO_DIAG_TILE_INDEX;
        nFlipX = 1;
        nFlipY = 1;
        break;
    case HERO_DIR_UP | HERO_DIR_LEFT:
        nTile  = HERO_DIAG_TILE_INDEX;
        nFlipX = 1;
        break;
    case HERO_DIR_UP | HERO_DIR_RIGHT:
        nTile  = HERO_DIAG_TILE_INDEX;
        break;
    default:
        break;
    }
    
    // Only move is L isn't held
    if (!key_down(KEY_L))
    {
        rect_move_with_bg_collision(&(pHero->rect), fDX, fDY);
    }
    
    sprite_set_tile(HERO_SPRITE_INDEX, 
                    nTile);
                    
    sprite_flip(HERO_SPRITE_INDEX, 
                nFlipX,
                nFlipY);
    
    sprite_set_position(HERO_SPRITE_INDEX, 
                        fixed_to_int(pHero->rect.fX) - HERO_SPRITE_X_OFF, 
                        fixed_to_int(pHero->rect.fY) - HERO_SPRITE_Y_OFF);
                        
     // Next, check if a shot is fired
     if (key_down(KEY_A) &&
         pHero->nAttackDelay == 0)
     {
         _hero_fire_bullet(pHero, pGameData);
     }
     else if (pHero->nAttackDelay != 0)
     {
        pHero->nAttackDelay--;
     }
     
     // Update damage counter for flashing + invulnerability
     if (pHero->nDamageCounter > 0)
     {
        pHero->nDamageCounter--;
        
        if (pHero->nDamageCounter == 0)
        {
            sprite_set_palette(HERO_SPRITE_INDEX, HERO_PALETTE_BANK);
        }
        else
        {
            // Flash the palette
            if (pHero->nDamageCounter % 8 >= 4)
            {
                sprite_set_palette(HERO_SPRITE_INDEX, HERO_PALETTE_BANK);
            }
            else
            {
                sprite_set_palette(HERO_SPRITE_INDEX, DAMAGE_PALETTE_BANK);
            }
        }
     }
}

void hero_damage(Hero* pHero,
                 int nDamage)
{
    if (pHero->nDamageCounter == 0)
    {
        pHero->nDamageCounter = HERO_DAMAGE_COUNTER_START;
        pHero->nHealth -= nDamage;
    }
}

void _hero_fire_bullet(Hero* pHero,
                       void* pGameData)
{
    GameData* pData = (GameData*) pGameData;
    
    // Find what bullet index is next to be used
    int nIndex = pData->nBulletIndex;
    pData->nBulletIndex++;
    
    if (pData->nBulletIndex >= MAX_BULLETS)
    {
        pData->nBulletIndex = 0;
    }
    
    Bullet* pBullet = &(pData->arBullets[nIndex]);
    bullet_initialize(pBullet, pHero->nBullet, nIndex);
    pBullet->rect.fX = pHero->rect.fX + ((HERO_RECT_WIDTH/2) << FIXED_SHIFT) - (pBullet->rect.fWidth >> 2);
    pBullet->rect.fY = pHero->rect.fY + ((HERO_RECT_HEIGHT/2) << FIXED_SHIFT) - (pBullet->rect.fHeight >> 2);
    pBullet->nDamage = pHero->nDamage;
    pBullet->nOwner  = BULLET_OWNER_HERO;
    
    // Figure out what direction to shoot it
    switch (s_nDir)
    {
    case HERO_DIR_RIGHT:
        pBullet->fXVel = arBulletSpeed[pHero->nBullet];
        break;
    case HERO_DIR_DOWN:
        pBullet->fYVel = arBulletSpeed[pHero->nBullet];
        break;
    case HERO_DIR_LEFT:
        pBullet->fXVel = -arBulletSpeed[pHero->nBullet];
        break;
    case HERO_DIR_UP:
        pBullet->fYVel = -arBulletSpeed[pHero->nBullet];
        break;
    case HERO_DIR_DOWN | HERO_DIR_RIGHT:
        pBullet->fXVel = arBulletSpeed[pHero->nBullet];
        pBullet->fYVel = arBulletSpeed[pHero->nBullet];
        break;
    case HERO_DIR_DOWN | HERO_DIR_LEFT:
        pBullet->fXVel = -arBulletSpeed[pHero->nBullet];
        pBullet->fYVel = arBulletSpeed[pHero->nBullet];
        break;
     case HERO_DIR_UP | HERO_DIR_LEFT:
        pBullet->fXVel = -arBulletSpeed[pHero->nBullet];
        pBullet->fYVel = -arBulletSpeed[pHero->nBullet];
        break;
     case HERO_DIR_UP | HERO_DIR_RIGHT:
        pBullet->fXVel = arBulletSpeed[pHero->nBullet];
        pBullet->fYVel = -arBulletSpeed[pHero->nBullet];
        break;
    default:
        pBullet->fXVel = arBulletSpeed[pHero->nBullet];
        break;
    }
    
    pHero->nAttackDelay = pBullet->nDelay;
}
