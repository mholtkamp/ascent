#include "util.h"
#include "hero.h"
#include "rect.h"
#include "constants.h"
#include "fixedpoint.h"
#include "images/img_hero.h"

#define HERO_MOVESPEED 1
static int s_nVert = 0;
static fixed s_fMoveSpeed[HERO_MAX_SPEED] = {0, 0, 0, 0,
                                             0, 0, 0, 0};
                                             
void hero_static_initialize(Hero* pHero)
{
    s_fMoveSpeed[0] = float_to_fixed(1.0f);
    s_fMoveSpeed[1] = float_to_fixed(1.4f);
    s_fMoveSpeed[2] = float_to_fixed(1.8f);
    s_fMoveSpeed[3] = float_to_fixed(2.2f);
    s_fMoveSpeed[4] = float_to_fixed(2.6f);
    s_fMoveSpeed[5] = float_to_fixed(2.8f);
    s_fMoveSpeed[6] = float_to_fixed(3.0f);
    s_fMoveSpeed[7] = float_to_fixed(3.2f);
    
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
        pHero->nMaxHealth = HERO_STARTING_HEALTH;
        pHero->nHealth    = HERO_STARTING_HEALTH;
        pHero->nGems      = HERO_STARTING_GEMS;
        pHero->nDamage    = HERO_STARTING_DAMAGE;
        pHero->nSpeed     = HERO_STARTING_SPEED;
        
        rect_initialize(&(pHero->rect));
    }   
}

void hero_update(Hero* pHero)
{
    int nVert = 0;
    
    if (key_down(KEY_RIGHT))
    {
        pHero->rect.fX += s_fMoveSpeed[pHero->nSpeed];
    }
    else if (key_down(KEY_LEFT))
    {
        pHero->rect.fX += -s_fMoveSpeed[pHero->nSpeed];
    }
    
    if (key_down(KEY_UP))
    {
        pHero->rect.fY += -s_fMoveSpeed[pHero->nSpeed];
        nVert = 1;
    }
    else if (key_down(KEY_DOWN))
    {
        pHero->rect.fY += s_fMoveSpeed[pHero->nSpeed];
        nVert = 1;
    }
    
    sprite_set_tile(HERO_SPRITE_INDEX, HERO_VERT_TILE_INDEX * nVert);
    
    sprite_set_position(HERO_SPRITE_INDEX, fixed_to_int(pHero->rect.fX), fixed_to_int(pHero->rect.fY));
}

