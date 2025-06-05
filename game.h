// ===========================================================================================
// game.h - Game Logic and Data
// ===========================================================================================

#ifndef GAME_H
#define GAME_H

#include <stdbool.h>  // For using bool types


// ===========================================================================================
// Constants for map and entity limits

#define MAP_SIZE_HEIGHT 21
#define MAP_SIZE_WIDTH 84
#define MAX_ZOMBIES 20
#define MAX_AMMO 99

// ===========================================================================================
// Player structure to hold player state
// ===========================================================================================


typedef struct {
    int x, y;            // Current position of the player on the map
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
// Function declarations
// ===========================================================================================

// Initialize the map with default values (walls, empty spaces)
void init_map(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH]);

// Print the current state of the map to the terminal
void print_map(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH]);

// Set up the player's initial state and place them on the map
void init_player(Player* player, char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH]);

// Randomly place zombie entities on the map
void place_entities(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH], Zombie zombies[], int count, char symbol);

// Place collectible items (mugs or ammo) on the map
void place_items(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH], int count, char symbol);

// Place the exit point on the map
void place_exit(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH]);

// Print player's current stats (HP, ammo, kills, etc.)
void print_stats(const Player* player);

// Handle player movement and check for interactions (combat, pickups, exit)
void move_player(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH], Player* player, Zombie zombies[], int* zombie_count, char direction);

// Fire a bullet in the last moved direction; hits zombies if in path
void fire_bullet(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH], Player* player, Zombie zombies[], int* zombie_count);

// Calculate total score based on mugs, kills, and successful escape
int calculate_score(const Player* player);

#endif
