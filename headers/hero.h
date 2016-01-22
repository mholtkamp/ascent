#ifndef HERO_H
#define HERO_H

#include "rect.h"

typedef struct Hero
{
    int nMaxHealth;
    int nHealth;
    int nGems;
    int nDamage;
    int nSpeed;
    int nBullet;
    int nAttackDelay;
    int nDamageCounter;
    Rect rect;
} Hero;

void hero_static_initialize(Hero* pHero);

void hero_initialize(Hero* pHero);

void hero_update(Hero* pHero,
                 void* pGameData);

void hero_damage(Hero* pHero,
                 int nDamage);

void _hero_fire_bullet(Hero* pHero,
                       void* pGameData);

#endif