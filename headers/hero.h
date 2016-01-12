#ifndef HERO_H
#define HERO_H

#include "rect.h"

#define HERO_STARTING_HEALTH 3
#define HERO_STARTING_GEMS   3
#define HERO_STARTING_DAMAGE 3
#define HERO_STARTING_SPEED  3

typedef struct Hero
{
    int nHealth;
    int nGems;
    int nDamage;
    int nSpeed;
    Rect rect;
} Hero;

void hero_initialize(Hero* pHero);

#endif