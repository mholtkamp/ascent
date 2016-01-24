#include <string.h>
#include "game.h"
#include "util.h"
#include "images/img_rooms.h"
#include "images/img_hero.h"
#include "images/img_bullet.h"
#include "images/img_drops.h"


#define DIR_RIGHT 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_UP 3
#define NUM_DIRECTIONS 4

void game_initialize(GameData* pData)
{
    int i = 0;
    
    // Load palette for room backgrounds
    load_palette_bank(PALETTE_TYPE_BG, 0, arRoomPalette);
    
    // Load tiles for room backgrounds
    load_tiles(BG_CBB, // CBB 0
               0, // 0 offset
               32, // 32 tiles in BG
               arRoomTiles);
               
    // Load tiles for drops
    for (i = 0; i < NUM_DROP_TYPES; i++)
    {
        // Load tiles in BG tile space (CBB 0) so that
        // the tiles can be used in the UI
        load_tiles(TEXT_CBB, // CBB 1
                   arDropTileIndices[i], // place to start loading tiles
                   1,
                   arDropTiles[i]);
        
        // Load the same tiles into sprite tile space (CBB 4) so that
        // the drops can be rendered as OBJs on the room floor when dropped by
        // enemies / chests.
        load_tiles(SPRITE_CBB,
                   arDropTileIndices[i],
                   1,
                   arDropTiles[i]);
                   
        // Load the drop color palette
        load_palette_bank(PALETTE_TYPE_BG,
                          DROP_PALETTE_BANK,
                          arDropPalette);
        
        load_palette_bank(PALETTE_TYPE_OBJ,
                          DROP_PALETTE_BANK,
                          arDropPalette);
    }
    
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
    
     game_draw_hud(pData);
     
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
    
    // Check for gameover
    if (pData->hero.nHealth <= 0)
    {
        pData->nGameState = STATE_GAME_OVER;
        memset((unsigned short*) ADDR_ROOM_SBB, 0, SCREEN_BLOCK_SIZE);
        clear_all_sprites();
        clear_text();
        text("GAME OVER", 10, 9);
    }
}

void game_draw_hud(GameData* pData)
{
    int i = 0;
    Hero* pHero = &(pData->hero);
    
    // The HUD (for now) is drawing on the text SBB.
    // Might move to BG2 if this is a problem
    unsigned short* pMap = (unsigned short*) (0x06000000 + TEXT_SCREENBLOCK * 0x0800);
    
    // Draw hearts
    for (i = 1; i < HERO_MAX_HEALTH; i++)
    {
        if (i <= pHero->nHealth)
        {
            // Draw heart
            pMap[0 + i] = (DROP_PALETTE_BANK << 12) | arDropTileIndices[DROP_TYPE_HEART];
        }
        else
        {
            // Draw space
            pMap[0 + i] = 0;
        }
    }
    
    // Draw Gems
    pMap[24] = (DROP_PALETTE_BANK << 12) | arDropTileIndices[DROP_TYPE_GEM];
    pMap[25] = 0xf000 | ('X' - ' ');
    pMap[26] = 0xf000 | ((pHero->nGems/10 + 0x30) - ' ');
    pMap[27] = 0xf000 | ((pHero->nGems%10 + 0x30) - ' ');
}

void game_load_floor(GameData* pData)
{
    clear_text();
    
    // Set hero to center of room
    pData->hero.rect.fX = int_to_fixed(HERO_START_X);
    pData->hero.rect.fY = int_to_fixed(HERO_START_Y);
    
    // Enable hero OBJ sprite since it was
    hero_enable_sprite(&(pData->hero));
    
    // Generate the floor
    _game_generate_floor(pData);
    
    // recursively call _game_load_room to load the proper room
    _game_load_room(pData);
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
    
    // Attach Extra rooms
    
    // Add the stairwell last
    _game_attach_room(pData,
                      ROOM_STAIRS,
                      nNumRooms,
                      arRoomListX,
                      arRoomListY);
    
    // Set the current room in the gamedata
    pData->nRoomX = arRoomListX[0];
    pData->nRoomY = arRoomListY[0];
}

void _game_attach_room(GameData* pData,
                       char  cRoomNum,
                       int   nNumRooms,
                       int* arRoomListX,
                       int* arRoomListY)
{
    int i = random() % nNumRooms;
    int nDir = random() % 4;
    int nRoomCount = 0;
    int nX = 0;
    int nY = 0;
    int nDistX = 0;
    int nDistY = 0;
    int nMaxDist = 0;
    
    // First, check if stairs are being added.
    if (cRoomNum == ROOM_STAIRS)
    {
        // We want to add stairs as far away from starting room as possible.
        for (i = 1; i < nNumRooms; i++)
        {
            // Find out X and Y distance in units of rooms
            nDistX = arRoomListX[i] - arRoomListX[0];
            nDistY = arRoomListY[i] - arRoomListY[0];
            
            // Get absolute values
            if (nDistX < 0)
                nDistX *= -1;
            if (nDistY < 0)
                nDistY *= -1;
            
            // If this room is farthest away from starting room,
            // then set the new max dist and save the X and Y coords
            if (nDistX + nDistY > nMaxDist)
            {
                nMaxDist = nDistX + nDistY;
                nX = arRoomListX[i];
                nY = arRoomListY[i];
            }
        }
        
        // Set the farthest room to ROOM_STAIRS (overwriting what it was)
        pData->arFloor[nX][nY] = ROOM_STAIRS;
        
        return;
    }
    
    while(nRoomCount < nNumRooms)
    {
        nX = arRoomListX[i];
        nY = arRoomListY[i];
        
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
        
        if (nX >= 0           &&
            nX < FLOOR_WIDTH  &&
            nY >= 0           &&
            nY < FLOOR_HEIGHT &&
            pData->arFloor[nX][nY] == 0)
         {
            pData->arFloor[nX][nY] = cRoomNum;
            break;
         }
         
         nRoomCount++;
         i++;
         
         // Wrap the room index back to 0 if over # of rooms.
         if (i >= nRoomCount)
         {
            i = 0;
         }
    }
    
    // Failed to attach room
    if (nRoomCount == nNumRooms)
    {
        // Just set the last room in the list to the stairs room
        pData->arFloor[arRoomListX[nNumRooms - 1]][arRoomListY[nNumRooms - 1]] = cRoomNum;
    }
}

void _game_load_room(GameData* pData)
{
    int i = 0;
    char arFloorNum[2] = {0,0};
    int nRand = 0;
    const short* pEnemyMap = 0;
    
    // Get the room number/ID.
    int nRoomNumber = (int) pData->arFloor[pData->nRoomX][pData->nRoomY];
    
    // Check if the room is actually the stairwell
    // to the next floor.
    if (nRoomNumber == ROOM_STAIRS)
    {
        // Room is stairwell so incrememnt level counter
        pData->nLevel++;
        
        if (pData->nLevel <= NUM_FLOORS)
        {
            // Set the game state to hover on this transition state
            pData->nGameState = STATE_FLOOR_TRANS;
            
            memset((unsigned short*) ADDR_ROOM_SBB, 0, SCREEN_BLOCK_SIZE);
            clear_all_sprites();
            clear_text();
            text("FLOOR", 10, 9);
            arFloorNum[0] = ((char) pData->nLevel) + 0x30;
            text(arFloorNum, 16, 9);
        }
        else
        {
            // Set the game state to hover on this transition state
            pData->nGameState = STATE_WIN;
            
            memset((unsigned short*) ADDR_ROOM_SBB, 0, SCREEN_BLOCK_SIZE);
            clear_all_sprites();
            clear_text();
            text("YOU WIN!", 11, 4);
            text("THANKS FOR PLAYING", 6, 7);
            text("SOURCE AVAILABLE AT:", 5, 13);
            text("GITHUB.COM/MHOLTKAMP/ASCENT",2, 15);
        }

        
        // Exit function
        return;
    }
    
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
            enemy_initialize(&(pData->arEnemies[i]),
                             pEnemyMap[i*3],
                             i);
            
            // And then set the position
            pData->arEnemies[i].rect.fX = int_to_fixed((int) pEnemyMap[i*3+1]);
            pData->arEnemies[i].rect.fY = int_to_fixed((int) pEnemyMap[i*3+2]);
        }
        
        i++;
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
    int nTileIndex = OPEN_DOOR_TILE_INDEX;
    
    if (pData->nRoomX - 1 >= 0 &&
        pData->arFloor[pData->nRoomX - 1][pData->nRoomY] != 0)
    {
        if(pData->arFloor[pData->nRoomX - 1][pData->nRoomY] == ROOM_STAIRS)
            nTileIndex = STAIRS_TILE_INDEX;
        else
            nTileIndex = OPEN_DOOR_TILE_INDEX;
        
        // Open left doorway
        pMap[8  * SCREEN_BLOCK_MAP_WIDTH] = nTileIndex;
        pMap[9  * SCREEN_BLOCK_MAP_WIDTH] = nTileIndex;
        pMap[10 * SCREEN_BLOCK_MAP_WIDTH] = nTileIndex;
        pMap[11 * SCREEN_BLOCK_MAP_WIDTH] = nTileIndex;
    }
    
    if (pData->nRoomX + 1 < FLOOR_WIDTH &&
        pData->arFloor[pData->nRoomX + 1][pData->nRoomY] != 0)
    {
        if(pData->arFloor[pData->nRoomX + 1][pData->nRoomY] == ROOM_STAIRS)
            nTileIndex = STAIRS_TILE_INDEX;
        else
            nTileIndex = OPEN_DOOR_TILE_INDEX;
    
        // Open right doorway
        pMap[29 + 8 * SCREEN_BLOCK_MAP_WIDTH] = nTileIndex;
        pMap[29 + 9 * SCREEN_BLOCK_MAP_WIDTH] = nTileIndex;
        pMap[29 + 10 * SCREEN_BLOCK_MAP_WIDTH] = nTileIndex;
        pMap[29 + 11 * SCREEN_BLOCK_MAP_WIDTH] = nTileIndex;
    }
    
    if (pData->nRoomY - 1 >= 0 &&
        pData->arFloor[pData->nRoomX][pData->nRoomY - 1] != 0)
    {
        if(pData->arFloor[pData->nRoomX][pData->nRoomY - 1] == ROOM_STAIRS)
            nTileIndex = STAIRS_TILE_INDEX;
        else
            nTileIndex = OPEN_DOOR_TILE_INDEX;
        
        // Open top doorway
        pMap[13] = nTileIndex;
        pMap[14] = nTileIndex;
        pMap[15] = nTileIndex;
        pMap[16] = nTileIndex;
    }
    if (pData->nRoomY + 1 < FLOOR_HEIGHT &&
        pData->arFloor[pData->nRoomX][pData->nRoomY + 1] != 0)
    {
        if(pData->arFloor[pData->nRoomX][pData->nRoomY + 1] == ROOM_STAIRS)
            nTileIndex = STAIRS_TILE_INDEX;
        else
            nTileIndex = OPEN_DOOR_TILE_INDEX;
        
        // Open bottom doorway
        pMap[13 + 19 * SCREEN_BLOCK_MAP_WIDTH] = nTileIndex;
        pMap[14 + 19 * SCREEN_BLOCK_MAP_WIDTH] = nTileIndex;
        pMap[15 + 19 * SCREEN_BLOCK_MAP_WIDTH] = nTileIndex;
        pMap[16 + 19 * SCREEN_BLOCK_MAP_WIDTH] = nTileIndex;
    }
    
    pData->arFloor[pData->nRoomX][pData->nRoomY] |= ROOM_CLEARED_FLAG;
}