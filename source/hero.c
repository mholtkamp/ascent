#include "util.h"
#include "hero.h"
#include "rect.h"
#include "constants.h"
#include "fixedpoint.h"
#include "images/img_hero.h"
#include "gamedata.h"

#define HERO_MOVESPEED 1

#define HERO_DIR_RIGHT 0
#define HERO_DIR_DOWN  1
#define HERO_DIR_LEFT  2
#define HERO_DIR_UP    3

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
    load_tiles(4,                    // CBB 4
               HERO_HORI_TILE_INDEX, // 0 offset
               4,                    // 4 tiles to load
               arHeroHoriTiles);
    load_tiles(4,                     // CBB 4
               HERO_VERT_TILE_INDEX,  // Offset is 4 tiles in
               4,                     // 4 tiles to load
               arHeroVertTiles); 
               
    // Load the hero palette into bank 0
    load_palette_bank(PALETTE_TYPE_OBJ, 0, arHeroPalette);
    
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
        
        pHero->rect.fX      = 0;
        pHero->rect.fY      = 0;
        pHero->rect.fWidth  = int_to_fixed(HERO_RECT_WIDTH);
        pHero->rect.fHeight = int_to_fixed(HERO_RECT_HEIGHT);
    }   
}

void hero_update(Hero* pHero,
                 void* pGameData)
{   
    fixed fDX = 0;
    fixed fDY = 0;
    
    // Perform movement first
    if (key_down(KEY_RIGHT))
    {
        fDX = s_fMoveSpeed[pHero->nSpeed];
        s_nDir = HERO_DIR_RIGHT;
    }
    else if (key_down(KEY_LEFT))
    {
        fDX = -s_fMoveSpeed[pHero->nSpeed];
        s_nDir = HERO_DIR_LEFT;
    }
    
    if (key_down(KEY_UP))
    {
        fDY = -s_fMoveSpeed[pHero->nSpeed];
        s_nDir = HERO_DIR_UP;
    }
    else if (key_down(KEY_DOWN))
    {
        fDY = s_fMoveSpeed[pHero->nSpeed];
        s_nDir = HERO_DIR_DOWN;
    }
    
    rect_move_with_bg_collision(&(pHero->rect), fDX, fDY);

    sprite_set_tile(HERO_SPRITE_INDEX, 
                    HERO_VERT_TILE_INDEX * ((s_nDir == HERO_DIR_DOWN) || (s_nDir == HERO_DIR_UP)));
                    
    sprite_flip(HERO_SPRITE_INDEX, 
                s_nDir == HERO_DIR_LEFT,
                s_nDir == HERO_DIR_DOWN);
    
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
        print_int(pHero->nAttackDelay);
        pHero->nAttackDelay--;
     }
}

void _hero_fire_bullet(Hero* pHero,
                       void* pGameData)
{
    print("FIRE");
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
    pBullet->rect.fX = pHero->rect.fX + ((HERO_RECT_WIDTH/2) << FIXED_SHIFT);
    pBullet->rect.fY = pHero->rect.fY + ((HERO_RECT_HEIGHT/2) << FIXED_SHIFT);
    pBullet->fXVel = int_to_fixed(3);
    
    pHero->nAttackDelay = pBullet->nDelay;
}
