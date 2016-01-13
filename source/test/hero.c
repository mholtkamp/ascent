#include "hero.h"
#include "rect.h"

void something_initialize(Hero* pHero)
{
    if (pHero != 0)
    {
        pHero->nHealth = HERO_STARTING_HEALTH;
        pHero->nGems   = HERO_STARTING_GEMS;
        pHero->nDamage = HERO_STARTING_DAMAGE;
        pHero->nSpeed  = HERO_STARTING_SPEED;
        
        rect_initialize(&(pHero->rect));
    }   
}

