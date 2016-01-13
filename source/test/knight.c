#include "knight.h"

void knight_initialize(Knight* pKnight)
{
    if (pHero != 0)
    {
        pHero->nHealth = 3;
        pHero->nGems   = 3;
        pHero->nDamage = 3;
        pHero->nSpeed  = 3;
        
        rect_initialize(&(pHero->rect));
    }  
}