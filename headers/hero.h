#ifndef HERO_H
#define HERO_H

#include "rect.h"

#define HERO_HORI_TILE_INDEX 0
#define HERO_VERT_TILE_INDEX 4

#define HERO_STARTING_HEALTH 3
#define HERO_STARTING_GEMS   3
#define HERO_STARTING_DAMAGE 3
#define HERO_STARTING_SPEED  1

#define HERO_MAX_HEALTH 8
#define HERO_MAX_GEMS   99
#define HERO_MAX_DAMAGE 8
#define HERO_MAX_SPEED  8

#define HERO_RECT_WIDTH  14
#define HERO_RECT_HEIGHT 14
#define HERO_SPRITE_X_OFF 1
#define HERO_SPRITE_Y_OFF 1

typedef struct Hero
{
    int nMaxHealth;
    int nHealth;
    int nGems;
    int nDamage;
    int nSpeed;
    int nBullet;
    int nAttackDelay;
    Rect rect;
} Hero;

void hero_static_initialize(Hero* pHero);

void hero_initialize(Hero* pHero);

void hero_update(Hero* pHero,
                 void* pGameData);

void _hero_fire_bullet(Hero* pHero,
                       void* pGameData);

#endif