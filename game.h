// ===========================================================================================
// game.h - Game Logic and Data
// ===========================================================================================

#ifndef GAME_H
#define GAME_H

#include <stdbool.h>  // For using bool types
#include <stdint.h>


#define ENABLE_DEV_COMMANDS   // dev mode

#ifdef ENABLE_DEV_COMMANDS
// #define DEV_ONLY(code) code
#else
// #define DEV_ONLY(code)
#endif



// ===========================================================================================
// Constants for map and entity limits

#define MAP_SIZE_HEIGHT 21
#define MAP_SIZE_WIDTH 84
#define MAX_ZOMBIES 20               // per Zone
#define ZOMBIE_HP 5
#define MAX_AMMO 99                
#define MAX_AMMO_PER_ZONE 10    
#define MAX_ALE_PER_ZONE 10
#define MAX_HERO_HP 10
#define ALE_HEAL 4
#define DEFAULT_SIGHT_DISTANCE 3


#define TILE_WALL '#'
#define TILE_EMPTY ' '
#define TILE_EXIT '>'

#define TILE_AMMO 'o'
#define TILE_ALE 'U'

#define TILE_ZOMBIE 'Z'

#define TILE_FOG '.'

#define TILE_HERO '@'

#define MOVE_UP 'w'
#define MOVE_DOWN 's'
#define MOVE_RIGHT 'd'
#define MOVE_LEFT 'a'

#define MOVE_FIRE 'r'
#define MOVE_QUIT 'q'

#define MOVE_KATE 'k' //future dev command to show the map allocate
#define MOVE_LIFE 'l' // dev command to give life -- we should #ifndef these dev commands to make them easy to turn them on/off_type
#define MOVE_GPS 'g' // dev command to show current location for debugging maps - future commmand


// ===========================================================================================
// TILE BIT MASK

#define TILE_VISIBLE   0x01  // Currently visible this frame
#define TILE_REVEALED  0x02  // Has been seen before (discovered)
#define TILE_PASSABLE  0x04  // Example future: can move through
#define TILE_DOOR      0x08  // Example future: is a door

#define ON(tile, flag)       ((tile).flags |= (flag))    // Turn on
#define OFF(tile, flag)      ((tile).flags &= ~(flag))   // Turn off
#define IS_SET(tile, flag)   (((tile).flags & (flag)) != 0) // Check if on



// ===========================================================================================
// Player structure to hold player state
// ===========================================================================================


typedef struct {
    // int x, y;            // Current position of the player on the map moved to world structure 
    int hp;              // Player's hit points
    int ammo;            // Amount of ammo the player has
    int mugs;            // Number of mugs collected
    int kills;           // Number of zombies killed
    bool exited;         // Flag for whether the player reached the exit
    char last_dir;       // Last direction moved (used for ranged attack direction)
} Player;

// ===========================================================================================
// Zombie structure to track individual enemies
// ===========================================================================================

typedef struct {
    int x, y;            // Position of the zombie
    int hp;              // Hit points of the zombie
    bool alive;          // Status flag to check if zombie is still active
} Zombie;


// ===========================================================================================
// 
// ===========================================================================================

typedef struct {
    char display;    // What the player sees (‘#’, ‘Z’, ‘>’, etc.)
	uint8_t flags;   // our 8 bit mask for each tile
    int hp;			 // monster/boss hp
} Tile;

// ===========================================================================================
// 
// ===========================================================================================

typedef struct {
    Tile tiles[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH];
    const char* name;
} Zone;

// ===========================================================================================
// 
// ===========================================================================================

typedef enum {
    ZONE_INN,
    ZONE_CELLAR,
    ZONE_CAVE1,
    ZONE_MAX
} ZoneID;

// ===========================================================================================
// 
// ===========================================================================================


typedef struct {
    Zone zones[ZONE_MAX];     // All zones loaded/initialized at game start
    ZoneID current_zone_id;   // Where the player is now
    int player_x; 
	int player_y;   
} World;





// ===========================================================================================
// Function declarations
// ===========================================================================================

// Initializes the world - and the world was good 
void init_world(void);

// Initialize the map with default values (walls, empty spaces)
void init_map(ZoneID zid);

// Print the current state of the map to the terminal
void print_map(Tile tiles[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH]);

// Set up the player's initial state and place them on the map
void init_player(void);

// Randomly place zombie entities on the map
void place_mobs(ZoneID zid);

// Place collectible items (mugs or ammo) on the map
void place_items(ZoneID zid, int couunt, char symbol);


// Print player's current stats (HP, ammo, kills, etc.)
void print_stats(void);

// Handle player movement and check for interactions (combat, pickups, exit)
void move_player(char direction);

// Fire a bullet in the last moved direction; hits zombies if in path
void fire_bullet(void);

// Calculate total score based on mugs, kills, and successful escape
int calculate_score(const Player* player);

// Using algebra to calc if 1 spot is within distance of  it
bool is_within_sight(int source_x, int source_y, int sight_distance, int target_x, int target_y);

// checks map to see what is visible to the hero - calls the is_within_sight checks
void fog_hero_pass(void);

#endif
