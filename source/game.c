#include <string.h>
#include "game.h"
#include "util.h"
#include "images/img_rooms.h"
#include "images/img_hero.h"
#include "images/img_bullet.h"


#define DIR_RIGHT 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_UP 3
#define NUM_DIRECTIONS 4

void game_initialize(GameData* pData)
{
    int i = 0;
    
    print("IN-GAME");
    
    // Load palette for room backgrounds
    load_palette_bank(PALETTE_TYPE_BG, 0, arRoomPalette);
    
    // Load tiles for room backgrounds
    load_tiles(0, // CBB 0
               0, // 0 offset
               32, // 32 tiles in BG
               arRoomTiles);
               
    _game_generate_floor(pData);
    
    // Load the starting room
    _game_load_room(pData);
    
    // Initialize the hero
    hero_static_initialize(&(pData->hero));
    
    // Initialize bullet graphics upfront
    bullet_load_graphics();
    
    // Clear enemies
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        enemy_clear(&(pData->arEnemies[i]), i);
    }
}

void game_update(GameData* pData)
{
    static int s_nADown = 0;
    int i = 0;
    Bullet* pBullets = pData->arBullets;
    Enemy* pEnemies = pData->arEnemies;
    
    // Do not update game, if the game is delayed or paused
    if (pData->nDelay > 0)
    {
        pData->nDelay--;
        return;
    }
    
    // Update hero actions
    hero_update(&(pData->hero), pData);
    
    // Update all bullets
    for (i = 0; i < MAX_BULLETS; i++)
    {
        // Loop through bullet array and only call update if alive
        if (pBullets[i].nAlive)
        {
            pBullets[i].update(&(pBullets[i]), pData);
        }
    }
    
    // Update all enemies
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        // Loop through enemy array and call update if alive
        if (pEnemies[i].nAlive)
        {
            pEnemies[i].update(&(pEnemies[i]), pData);
        }
    }
    
    if (key_down(KEY_B) && !s_nADown)
    {
        _game_clear_room(pData);
        s_nADown = 1;
    }
    else if (!key_down(KEY_B))
    {
        s_nADown = 0;
    }
    
    // Check if hero is standing on doorway
    if (pData->arFloor[pData->nRoomX][pData->nRoomY] & ROOM_CLEARED_FLAG)
    {
        if (pData->hero.rect.fX + pData->hero.rect.fWidth > int_to_fixed(234))
        {
            pData->nRoomX++;
            pData->hero.rect.fX = int_to_fixed(9);
            pData->hero.rect.fY = int_to_fixed(72);
            
            _game_load_room(pData);
        }
        else if (pData->hero.rect.fX < int_to_fixed(6))
        {
            pData->nRoomX--;
            pData->hero.rect.fX = int_to_fixed(217);
            pData->hero.rect.fY = int_to_fixed(72);
            
            _game_load_room(pData);
        }
        else if (pData->hero.rect.fY + pData->hero.rect.fHeight > int_to_fixed(164))
        {
            pData->nRoomY++;
            pData->hero.rect.fX = int_to_fixed(113);
            pData->hero.rect.fY = int_to_fixed(9);
            
            _game_load_room(pData);
        }
        else if (pData->hero.rect.fY < int_to_fixed(6))
        {
            pData->nRoomY--;
            pData->hero.rect.fX = int_to_fixed(113);
            pData->hero.rect.fY = int_to_fixed(137);
            
            _game_load_room(pData);
        }
    }
}

void _game_generate_floor(GameData* pData)
{
    // Figure out how many rooms to make based on level
    int nNumRooms  = 0;
    int nRoomCount = 0;
    int nX = 0;
    int nY = 0;
    int nDir          = 0;
    int nMoveAttempts = 0;
    int nRetryRoom    = 0;
    int nPrevX = 0;
    int nPrevY = 0;
    int arRoomListX[LEVEL_5_ROOM_COUNT] = {0};
    int arRoomListY[LEVEL_5_ROOM_COUNT] = {0};
    
    switch(pData->nLevel)
    {
    case 1:
        nNumRooms = LEVEL_1_ROOM_COUNT;
        break;
    case 2:
        nNumRooms = LEVEL_2_ROOM_COUNT;
        break;
    case 3:
        nNumRooms = LEVEL_3_ROOM_COUNT;
        break;
    case 4:
        nNumRooms = LEVEL_4_ROOM_COUNT;
        break;
    case 5:
        nNumRooms = LEVEL_5_ROOM_COUNT;
        break;
    default:
        nNumRooms = LEVEL_5_ROOM_COUNT;
        break;
    }
    
    // Clear the floor array
    memset(pData->arFloor, 0, FLOOR_WIDTH * FLOOR_HEIGHT);
    
    // Pick a random starting location
    nX = random() % FLOOR_WIDTH;
    nY = random() % FLOOR_HEIGHT;
    
    // Make the starting room Room#1 always
    pData->arFloor[nX][nY] = 1;
    
    arRoomListX[0] = nX;
    arRoomListY[0] = nY;
    
    // Iterate until the desired number of rooms have been plotted
    for (nRoomCount = 1; nRoomCount < nNumRooms;)
    {
        // Pick a random direction
        nDir = random() % 4;
        
        // Reset the move attempt counter.
        // If this goes above 4 attempts, do a desperate plot
        // Iterate through 4 directions, plot a random room if an empty room is found.
        for (nMoveAttempts = 0; nMoveAttempts < NUM_DIRECTIONS; nMoveAttempts++)
        {
            // Save a copy of the x/y coords in case room plot fails
            nPrevX = nX;
            nPrevY = nY;
            
            // Based on direction, generate new coordinates
            switch (nDir)
            {
            case DIR_RIGHT:
                ++nX;
                break;
            case DIR_DOWN:
                ++nY;
                break;
            case DIR_LEFT:
                --nX;
                break;
            case DIR_UP:
                --nY;
                break;
            default:
                break;
            }
            
            // Check if the new coords are in-bounds
            // And if the room value is 0
            if (nX >= 0           &&
                nX < FLOOR_WIDTH  &&
                nY >= 0           &&
                nY < FLOOR_HEIGHT &&
                pData->arFloor[nX][nY] == 0)
            {
                // Randomly pick a room to plot.
                // The value of arFloor[nX][nY] should not be 0 (empty room) and
                // should not be 1 (starting room.)
                pData->arFloor[nX][nY] = (random() % (NUM_ROOM_MAPS - 1)) + 2;
                
                arRoomListX[nRoomCount] = nX;
                arRoomListY[nRoomCount] = nY;
                
                // Increase the number of rooms plotted.
                ++nRoomCount;
                
                // Break out of loop, since an adjacent room was successfully found
                break;
            }
            else
            {
                // Reset the X/Y coords and try a new direction
                nX = nPrevX;
                nY = nPrevY;
                ++nDir;
                
                // Loop the direction back to right if it already went past the Up direction
                if (nDir >= NUM_DIRECTIONS)
                {
                    nDir = DIR_RIGHT;
                }
            }
        }
        
        // Check if a room was found or if all attempts were consumed.
        if (nMoveAttempts >= NUM_DIRECTIONS)
        {
            // Since all directions were attempted, go back to the retry room
            // which is initially the first room
            nX = arRoomListX[nRetryRoom];
            nY = arRoomListY[nRetryRoom];
            
            ++nRetryRoom;
        }
    }
    
    // Set the current room in the gamedata
    pData->nRoomX = arRoomListX[0];
    pData->nRoomY = arRoomListY[0];
}

void _game_load_room(GameData* pData)
{
    int i = 0;
    int nRand = 0;
    const short* pEnemyMap = 0;
    
    // Get the room number/ID.
    int nRoomNumber = (int) pData->arFloor[pData->nRoomX][pData->nRoomY];
    
    // Clear static enemy data
    reset_room_enemy_data(pData);
    
    // Load tile map
    load_map(ROOM_SBB, // SBB 31
             arRoomTable[nRoomNumber & (~ROOM_CLEARED_FLAG)]);
             
    // Load enemy map
    nRand = random() % 3;
    pEnemyMap = arEnemyMaps[nRoomNumber & (~ROOM_CLEARED_FLAG)][nRand];
    
    i = 0;
    
    while(1)
    {
        if (pEnemyMap[i*3] == -1)
        {
            // No more enemies to load
            break;
        }
        else
        {
            // Initialize the enemy
            print_int(pEnemyMap[i*3]);
            enemy_initialize(&(pData->arEnemies[i]),
                             pEnemyMap[i*3],
                             i);
            
            // And then set the position
            pData->arEnemies[i].rect.fX = int_to_fixed((int) pEnemyMap[i*3+1]);
            pData->arEnemies[i].rect.fY = int_to_fixed((int) pEnemyMap[i*3+2]);
        }
        
        i++;
        
        //@@ DEBUG
        print("ENEMY ADDED");
    }
    
    unsigned short* pMap = (unsigned short*) ADDR_ROOM_SBB;
    
    // Change tile map based on connecting rooms
    // Hardcoding is kept for simplicity
    if (pData->nRoomX - 1 < 0 ||
        pData->arFloor[pData->nRoomX - 1][pData->nRoomY] == 0)
    {
        // Block off left doorway
        pMap[8  * SCREEN_BLOCK_MAP_WIDTH] = WALL_TILE_INDEX;
        pMap[9  * SCREEN_BLOCK_MAP_WIDTH] = WALL_TILE_INDEX;
        pMap[10 * SCREEN_BLOCK_MAP_WIDTH] = WALL_TILE_INDEX;
        pMap[11 * SCREEN_BLOCK_MAP_WIDTH] = WALL_TILE_INDEX;
    }
    
    if (pData->nRoomX + 1 >= FLOOR_WIDTH ||
        pData->arFloor[pData->nRoomX + 1][pData->nRoomY] == 0)
    {
        pMap[29 + 8 * SCREEN_BLOCK_MAP_WIDTH] = WALL_TILE_INDEX;
        pMap[29 + 9 * SCREEN_BLOCK_MAP_WIDTH] = WALL_TILE_INDEX;
        pMap[29 + 10 * SCREEN_BLOCK_MAP_WIDTH] = WALL_TILE_INDEX;
        pMap[29 + 11 * SCREEN_BLOCK_MAP_WIDTH] = WALL_TILE_INDEX;
    }
    
    if (pData->nRoomY - 1 < 0 ||
        pData->arFloor[pData->nRoomX][pData->nRoomY - 1] == 0)
    {
        pMap[13] = WALL_TILE_INDEX;
        pMap[14] = WALL_TILE_INDEX;
        pMap[15] = WALL_TILE_INDEX;
        pMap[16] = WALL_TILE_INDEX;
    }
    if (pData->nRoomY + 1 >= FLOOR_HEIGHT ||
        pData->arFloor[pData->nRoomX][pData->nRoomY + 1] == 0)
    {
        pMap[13 + 19 * SCREEN_BLOCK_MAP_WIDTH] = WALL_TILE_INDEX;
        pMap[14 + 19 * SCREEN_BLOCK_MAP_WIDTH] = WALL_TILE_INDEX;
        pMap[15 + 19 * SCREEN_BLOCK_MAP_WIDTH] = WALL_TILE_INDEX;
        pMap[16 + 19 * SCREEN_BLOCK_MAP_WIDTH] = WALL_TILE_INDEX;
    }
    
    // Add a delay before game updates
    //pData->nDelay = 5;
    //_game_update_sprites();
}

void _game_clear_room(GameData* pData)
{
    unsigned short* pMap = (unsigned short*) ADDR_ROOM_SBB;

    if (pData->nRoomX - 1 >= 0 &&
        pData->arFloor[pData->nRoomX - 1][pData->nRoomY] != 0)
    {
        // Open left doorway
        pMap[8  * SCREEN_BLOCK_MAP_WIDTH] = OPEN_DOOR_TILE_INDEX;
        pMap[9  * SCREEN_BLOCK_MAP_WIDTH] = OPEN_DOOR_TILE_INDEX;
        pMap[10 * SCREEN_BLOCK_MAP_WIDTH] = OPEN_DOOR_TILE_INDEX;
        pMap[11 * SCREEN_BLOCK_MAP_WIDTH] = OPEN_DOOR_TILE_INDEX;
    }
    
    if (pData->nRoomX + 1 < FLOOR_WIDTH &&
        pData->arFloor[pData->nRoomX + 1][pData->nRoomY] != 0)
    {
        // Open right doorway
        pMap[29 + 8 * SCREEN_BLOCK_MAP_WIDTH] = OPEN_DOOR_TILE_INDEX;
        pMap[29 + 9 * SCREEN_BLOCK_MAP_WIDTH] = OPEN_DOOR_TILE_INDEX;
        pMap[29 + 10 * SCREEN_BLOCK_MAP_WIDTH] = OPEN_DOOR_TILE_INDEX;
        pMap[29 + 11 * SCREEN_BLOCK_MAP_WIDTH] = OPEN_DOOR_TILE_INDEX;
    }
    
    if (pData->nRoomY - 1 >= 0 &&
        pData->arFloor[pData->nRoomX][pData->nRoomY - 1] != 0)
    {
        // Open top doorway
        pMap[13] = OPEN_DOOR_TILE_INDEX;
        pMap[14] = OPEN_DOOR_TILE_INDEX;
        pMap[15] = OPEN_DOOR_TILE_INDEX;
        pMap[16] = OPEN_DOOR_TILE_INDEX;
    }
    if (pData->nRoomY + 1 < FLOOR_HEIGHT &&
        pData->arFloor[pData->nRoomX][pData->nRoomY + 1] != 0)
    {
        // Open bottom doorway
        pMap[13 + 19 * SCREEN_BLOCK_MAP_WIDTH] = OPEN_DOOR_TILE_INDEX;
        pMap[14 + 19 * SCREEN_BLOCK_MAP_WIDTH] = OPEN_DOOR_TILE_INDEX;
        pMap[15 + 19 * SCREEN_BLOCK_MAP_WIDTH] = OPEN_DOOR_TILE_INDEX;
        pMap[16 + 19 * SCREEN_BLOCK_MAP_WIDTH] = OPEN_DOOR_TILE_INDEX;
    }
    
    pData->arFloor[pData->nRoomX][pData->nRoomY] |= ROOM_CLEARED_FLAG;
}