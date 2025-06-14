// ===========================================================================================
// game.c - Function Definitions for Ironbrew Inn
// ===========================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "game.h"
#include "sounds.h"
#include "tools.h"



extern World world;
extern Player player;



// ===========================================================================================
// Helper function to find a random empty space on the map
// Static - only use in game.c 
// =========================================================================================== 

static void get_random_empty_location(Tile tiles[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH], int* x, int* y) {
    do {
        *x = rand() % MAP_SIZE_WIDTH;
        *y = rand() % MAP_SIZE_HEIGHT;
    } while (tiles[*y][*x].display != ' ');
}

// ===========================================================================================
// Prints Map out onto the screen - this will address for now spacing on top and on the left side to use empty space to depth the playing field
// ===========================================================================================


void print_map(Tile tiles[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH]) {

    printf("\n\n\n\n");
    for (int y = 0; y < MAP_SIZE_HEIGHT; y++) {
		printf("    ");
        for (int x = 0; x < MAP_SIZE_WIDTH; x++) {
			
			if (IS_SET(tiles[y][x], TILE_VISIBLE) || IS_SET(tiles[y][x], TILE_REVEALED)) {
			   print_color_char(tiles[y][x].display);             // Prints tiles -- see tools.h for use
			} else {
			   print_color_char(TILE_FOG);
			}
			
        }
        putchar('\n');
    }
	printf("n\n");
	printf("%s\n", last_action_msg);
	
}

// ===========================================================================================
// world init_map
// ===========================================================================================


void init_world(void) {
	ZoneID zid = ZONE_INN;
	
	while (zid != ZONE_MAX) {
		
		init_map(zid);  // map layout, populate it with zombies, gear
		place_mobs(zid); // place zombies 
		place_items(zid, MAX_ALE_PER_ZONE, TILE_ALE); // place mugs
		place_items(zid, MAX_AMMO_PER_ZONE, TILE_AMMO); //place ammo
		
		// zone specific setuup - like bosses, exits, special code here
		
		if (zid == ZONE_INN) {
			int x, y;
            
			get_random_empty_location(world.zones[zid].tiles, &x, &y);
			
			world.zones[zid].tiles[y][x].display = TILE_EXIT;             // Place exit for now.
			
			world.current_zone_id = zid;  // our first zone will be the inn - set it here
			
			world.zones[zid].name = "First Floor - Ironbrew Inn";
			
			
		} else if (zid == ZONE_CELLAR) {
			world.zones[zid].name = "Cellar - Ironbrew Inn";
		} else if (zid == ZONE_CAVE1) {
			world.zones[zid].name = "Spooky Cave";
		}
		zid++;
	}
	init_player();
	
}

// ===========================================================================================
// Okay builds the map which is now tiles in 1+ zones in a world
// ===========================================================================================


void init_map(ZoneID zid) {
	
	Tile (*map)[MAP_SIZE_WIDTH] = world.zones[zid].tiles; // tricky 2d map mapping - \o/ 
	
    for (int y = 0; y < MAP_SIZE_HEIGHT; y++) {
        for (int x = 0; x < MAP_SIZE_WIDTH; x++) {
            if (x == 0 || x == MAP_SIZE_WIDTH - 1 || y == 0 || y == MAP_SIZE_HEIGHT - 1) {
                map[y][x].display = TILE_WALL;  // Wall
            } else {
                map[y][x].display = TILE_EMPTY;  // Empty space
            }
			map[y][x].flags = 0;
        }
    }
}



// ===========================================================================================
// ready player 1 - fight!
// ===========================================================================================


void init_player(void) {
    int x, y;
   
	get_random_empty_location(world.zones[world.current_zone_id].tiles, &x, &y);
	
    player.hp = 10;
    player.ammo = 0;
    player.mugs = 0;
    player.kills = 0;
    player.exited = false;
    player.last_dir = 'w';
	
	world.player_x = x;
	world.player_y = y;
	
	world.zones[world.current_zone_id].tiles[y][x].display = TILE_HERO;
	fog_hero_pass();
}

// ===========================================================================================

// ===========================================================================================


void place_mobs(ZoneID zid) {
	
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        int x, y;
     
		get_random_empty_location(world.zones[zid].tiles, &x, &y);
		
		world.zones[zid].tiles[y][x].display = TILE_ZOMBIE; 
		world.zones[zid].tiles[y][x].hp = ZOMBIE_HP;
    }
}

// ===========================================================================================

// ===========================================================================================


void place_items(ZoneID zid, int count, char symbol) {
	
    for (int i = 0; i < count; i++) {
        int x, y;
       
		get_random_empty_location(world.zones[zid].tiles, &x, &y);
		world.zones[zid].tiles[y][x].display = symbol;
    }
}

// ===========================================================================================

// ===========================================================================================

void print_stats(void) {
    printf("HP: %d | Ammo: %d | Ales: %d | Kills: %d\n", player.hp, player.ammo, player.mugs, player.kills);
}

// ===========================================================================================


void move_player(char direction) {
    int dx = 0, dy = 0;
    switch (direction) {
        case MOVE_UP: dy = -1; break;
        case MOVE_DOWN: dy = 1; break;
        case MOVE_LEFT: dx = -1; break;
        case MOVE_RIGHT: dx = 1; break;
        default: return;
    }

    int new_x = world.player_x + dx;
    int new_y = world.player_y + dy;
    char target = world.zones[world.current_zone_id].tiles[new_y][new_x].display;
	Tile (*map)[MAP_SIZE_WIDTH] = world.zones[world.current_zone_id].tiles;

    if (target == TILE_WALL || target == TILE_HERO) return;  // Wall or self

    // Handle object interactions
	
    if (target == TILE_ALE) {
        
		player.hp += ALE_HEAL;
        if (player.hp > MAX_HERO_HP) player.hp = MAX_HERO_HP;
        player.mugs++;
        play_sound("gulp");
		if (player.hp >= MAX_HERO_HP) {
			set_last_action_msg("\n\nYou drink down an ale and back to max hit points!\n\n");
		} else {
			set_last_action_msg("\nYou drink a mug of ale!\n +++ GOOD STUFF +++\n You gain %d hit points.\n\n", ALE_HEAL);
		}
    } else if (target == TILE_AMMO) {
        
		player.ammo++;
		set_last_action_msg("\n\nFortune smiles on you. You relieve the floor of its bullet.\n\n");
        play_sound("reload");
    
	} else if (target == 'Z') {   // Combat -- let's get ready to rumble!!!
	
	    // combat will be refactored in the future during a combat mechanic adjustment
		
	    if (map[new_y][new_x].display == TILE_ZOMBIE) {
			
			int player_roll = rand() % 20 + 1;
            int zombie_roll = rand() % 20 + 1;
			
            if (player_roll >= zombie_roll) {
				
               int dmg = (player_roll == 20) ? (rand() % 6 + 1) * 2 : rand() % 6 + 1;
               
			   map[new_y][new_x].hp -= dmg;
                play_sound("slash");
                if (map[new_y][new_x].hp <= 0) {
                   map[new_y][new_x].display = TILE_EMPTY;
                   player.kills++;
				   set_last_action_msg("\n\nYour roll of (%d) beats the Zombie's (%d) and you land (%d) damage. Killing it!\n\n",
									  player_roll, zombie_roll, dmg);
                } else {
				   set_last_action_msg("\n\nYour roll of (%d) beats the Zombie's (%d) and you land (%d) damage. Zombie is wounded!\n\n",
									  player_roll, zombie_roll, dmg);
				}
            } else {
                int dmg = (zombie_roll == 20) ? (rand() % 6 + 1) * 2 : rand() % 6 + 1;
                
				player.hp -= dmg;
				set_last_action_msg("\n\nYour roll of (%d) comes up short to the Zombie's (%d) and you take (%d) damage. Ouch!\n\n",
									  player_roll, zombie_roll, dmg);
                play_sound("hurt");
            
			} 
            return;
		}
    } else if (target == '>') {         // EXIT - Roll Credits
        player.exited = true;
        return;
    }

    // Move the player
	
    map[world.player_y][world.player_x].display = TILE_EMPTY;
    world.player_x = new_x;
    world.player_y = new_y;
    map[new_y][new_x].display = TILE_HERO;
	fog_hero_pass();
	
}

// ===========================================================================================

void fire_bullet(void) {
    if (player.ammo <= 0) return; // no ammo - no range combat - nuff said

    int dx = 0, dy = 0;
    switch (player.last_dir) {
        case MOVE_UP: dy = -1; break;
        case MOVE_DOWN: dy = 1; break;
        case MOVE_LEFT: dx = -1; break;
        case MOVE_RIGHT: dx = 1; break;
        default: return;
    }

    int bx = world.player_x + dx;
    int by = world.player_y + dy;
	
	Tile (*map)[MAP_SIZE_WIDTH] = world.zones[world.current_zone_id].tiles;
	
	// note - we can refactor this function with a helper to put this into a helper in the future
	
	player.ammo--;
    for (int i = 0; i < 10; i++) {
        if (map[by][bx].display == TILE_WALL) {
			set_last_action_msg("\n\n ***BANG*** You daftly fire off a round and hit the wall. Take that wall!\n\n");
			return;
		}
		if (map[by][bx].display == TILE_EXIT) {
			set_last_action_msg("\n\n ***BANG*** You wound the exit. Yet it still will send you home when ready.\n\n");
			return;
		}
        if (map[by][bx].display == TILE_ZOMBIE) {
			
			map[by][bx].display = TILE_EMPTY;
			map[by][bx].hp = 0;
			play_sound("pew");
			player.kills++;
			
			set_last_action_msg("\n\nYou whip your revolver around ***BANG*** the Zombie falls from your headshot!\n\n");
			return;
        }
        bx += dx;
        by += dy;
    }	
}

// ===========================================================================================

int calculate_score(const Player* player) {
    return player->mugs + (player->kills * 2) + (player->exited ? 10 : 0);
}

// ===========================================================================================


bool is_within_sight(int source_x, int source_y, int sight_distance, int target_x, int target_y) {
    int dx = target_x - source_x;
    int dy = target_y - source_y;
    
    // Use Euclidean distance squared (avoids sqrt for speed)
    int distance_squared = dx * dx + dy * dy;
    int sight_radius_squared = sight_distance * sight_distance;
    
    return (distance_squared <= sight_radius_squared);
}

// ===========================================================================================

void fog_hero_pass(void) {
    Tile (*map)[MAP_SIZE_WIDTH] = world.zones[world.current_zone_id].tiles;

    for (int y = 0; y < MAP_SIZE_HEIGHT; y++) {
        for (int x = 0; x < MAP_SIZE_WIDTH; x++) {

            if (is_within_sight(world.player_x, world.player_y, DEFAULT_SIGHT_DISTANCE, x, y)) {

                ON(map[y][x], TILE_VISIBLE);

                // Permanently reveal walls and exits
                if (map[y][x].display == TILE_WALL || map[y][x].display == TILE_EXIT) {
                    ON(map[y][x], TILE_REVEALED);
                }

            } else {

                OFF(map[y][x], TILE_VISIBLE);

            }
        }
    }
}
