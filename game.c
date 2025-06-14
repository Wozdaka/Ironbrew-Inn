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


World world; //  Make the World - it will have ZONE_MAX zones 

// char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH]; Old World - Purge later --Ken

Player player; // player data - his location will be in the world map - might re-think that --Ken

// Zombie zombies[NUM_ZOMBIES]; - using lots of memory to put zombie data in the zones by tile
// int zombie_count = NUM_ZOMBIES; -- see above

// ===========================================================================================
// Initialize all game elements including player, map, items, and zombies

void init_game() {
	
    srand(time(NULL));  // Seed randomness for varied gameplay
	
	
	// okay we got data in .h we need to change init world here and all the gen code in game.c -- ken

    init_world();	// builds world which will include populating the zones with entities and items --Ken
	

}

// ===========================================================================================
// Core game loop: runs until player dies, escapes, or quits

void game_loop(void) {
    char command;
    int running = 1;

    while (running) {
		
		#ifdef _WIN32
		system("cls");  // Clear screen for clean redraw
		#else
		system("clear");  // Clear screen for clean redraw
		#endif
		
		print_map(world.zones[world.current_zone_id].tiles);
		
        print_stats();      // Show player status bar

        printf("Move (WASD), Fire (R), Quit (Q): ");
		
		// read in the data so we dont have enter
		//
		
		command = _getch();
		command = tolower(command);
		

        // Handle input
		
        switch (command) {
            case MOVE_UP: 
			case MOVE_LEFT: 
			case MOVE_DOWN: 
			case MOVE_RIGHT:
                player.last_dir = command;  // Track last movement direction
                move_player(command);  // Attempt move
                break;
            case MOVE_FIRE:
                fire_bullet();  // Fire ranged attack
                break;
            case MOVE_QUIT:
                running = 0;  // Exit game
                break;
        }

        // End conditions
        if (player.hp <= 0) {
            printf("\n\nYou died! Final Score: %d\n", calculate_score(&player));
            break;
        }

        if (player.exited) {
            printf("\n\nYou escaped the Ironbrew Inn! Final Score: %d\n", calculate_score(&player));
            break;
        }
    }
}

// ===========================================================================================
// Program entry point

int main() {
    init_game();   // Setup game world
    game_loop();   // Run the game loop
	printf("\nPress Enter to exit...");
    getchar();
    return 0;      // Exit cleanly
}
