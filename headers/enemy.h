#ifndef ENEMY_H
#define ENEMY_H

#include "rect.h"

typedef struct Enemy
{
    int nType;
    Rect rect;
    void (*update)(struct Enemy* pEnemy, void* pGameData);
    int nAlive;
    int nHealth;
    int nDamage;
    int nTime;
    int nOBJ;
    int nTile;
    int nPalette;
    int nSpriteOffsetX;
    int nSpriteOffsetY;
    int nDamageCounter;
    int arData[ENEMY_SCRATCH_DATA_SIZE/sizeof(int)];
} Enemy;

void enemy_initialize(Enemy* pEnemy,
                      int nType,
                      int nIndex);
                      
void enemy_clear(Enemy* pEnemy,
                 int nIndex);

void enemy_damage(Enemy* pEnemy,
                  int nDamage);
                 
void enemy_kill(Enemy* pEnemy);

void enemy_update_generic(Enemy* pEnemy,
                          void* pGameData);
                          
void enemy_update_caterpillar(Enemy* pEnemy,
                              void* pGameData);

void enemy_update_pixie(Enemy* pEnemy,
                        void* pGameData);
                   
void load_all_enemy_palettes(void* pGameData);

void reset_room_enemy_data(void* pGameData);

#endif