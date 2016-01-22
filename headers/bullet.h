#ifndef BULLET_H
#define BULLET_H

#include "fixedpoint.h"
#include "rect.h"
#include "constants.h"

#define NUM_BULLET_TYPES 1
#define BULLET_TYPE_BASIC 0

#define BULLET_OWNER_HERO 0
#define BULLET_OWNER_ENEMY 1

#define BULLET_OBJ_START_INDEX 100

// Basic bullet constsants
#define BULLET_BASIC_WIDTH 4 << FIXED_SHIFT
#define BULLET_BASIC_HEIGHT 4 << FIXED_SHIFT
#define BULLET_BASIC_ATTACK_DELAY 15
#define BULLET_BASIC_TILE_INDEX 12
#define BULLET_BASIC_LIFETIME 20

extern fixed arBulletSpeed[NUM_BULLET_TYPES];

typedef struct Bullet
{
    Rect rect;
    void (*update)(struct Bullet*, void* pGameData);
    fixed fXVel;
    fixed fYVel;
    int nType;
    int nTime;
    int nOwner;
    int nDamage;
    int nOBJ;
    int nAlive;
    int nDelay;
} Bullet;

void bullet_load_graphics();

void bullet_initialize(Bullet* pBullet,
                       int nType,
                       int nIndex);
                  
void bullet_kill(Bullet* pBullet);

// ***************************
// Individual Update Functions
// ***************************
void _bullet_basic_update(Bullet* pBullet,
                          void* pGameData);
                          
                          
#endif