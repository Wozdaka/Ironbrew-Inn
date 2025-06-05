// ===========================================================================================
// main.c - Ironbrew Inn Core Loop
// ===========================================================================================


#include <stdio.h>          // Standard input/output
#include <stdlib.h>         // For system(), rand(), etc.
#include <conio.h>          // for _getchar() 
#include <time.h>           // For seeding the random number generator
#include <windows.h>        // virtual console for coloring ansi
#include "game.h"           // Game-related functions and structures
#include "sounds.h"         // Placeholder sound system


// ===========================================================================================
// Constants for game setup

#define MAP_SIZE 21
#define NUM_ZOMBIES 10
#define NUM_MUGS 10
#define NUM_AMMO 10

// ===========================================================================================
// Global game state

char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH];
Player player;
Zombie zombies[NUM_ZOMBIES];
int zombie_count = NUM_ZOMBIES;

// ===========================================================================================
// Initialize all game elements including player, map, items, and zombies

void init_game() {
    srand(time(NULL));  // Seed randomness for varied gameplay
    init_map(map);      // Generate map layout
    init_player(&player, map);  // Place player on map
    place_entities(map, zombies, NUM_ZOMBIES, 'Z');  // Spawn zombies
    place_items(map, NUM_MUGS, 'U');  // Place healing ale mugs
    place_items(map, NUM_AMMO, 'o');  // Place ammo pickups
    place_exit(map);    // Set the escape point
}

// ===========================================================================================
// Core game loop: runs until player dies, escapes, or quits

void game_loop() {
    char command;
    int running = 1;

    while (running) {
		
		#ifdef _WIN32
		system("cls");  // Clear screen for clean redraw
		#else
		system("clear");  // Clear screen for clean redraw
		#endif
		
		
        print_map(map);            // Display current game map
        print_stats(&player);      // Show player status bar

        printf("Move (WASD), Fire (R), Quit (Q): ");
		
		// read in the data so we dont have enter
		//
		
		command = _getch();
		command = tolower(command);
		

        // Handle input
        switch (command) {
            case 'w':			case 'a': case 's': case 'd':
                player.last_dir = command;  // Track last movement direction
                move_player(map, &player, zombies, &zombie_count, command);  // Attempt move
                break;
            case 'r':
                fire_bullet(map, &player, zombies, &zombie_count);  // Fire ranged attack
                break;
            case 'q':
                running = 0;  // Exit game
                break;
        }

        // End conditions
        if (player.hp <= 0) {
            printf("\nYou died! Final Score: %d\n", calculate_score(&player));
            break;
        }

        if (player.exited) {
            printf("\nYou escaped the Ironbrew Inn! Final Score: %d\n", calculate_score(&player));
            break;
        }
    }
}

// ===========================================================================================
// Program entry point

int main() {
    init_game();   // Setup game world
    game_loop();   // Run the game loop
    return 0;      // Exit cleanly
}
