#include "Rect.h"

typedef struct Knight
{
    int nHealth;
    int nGems;
    int nDamage;
    int nSpeed;
    Rect rect;
} Knight;

void knight_initialize(Knight* pKnight);