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

// ===========================================================================================
// Helper function to find a random empty space on the map

static void get_random_empty_location(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH], int* x, int* y) {
    do {
        *x = rand() % MAP_SIZE_WIDTH;
        *y = rand() % MAP_SIZE_HEIGHT;
    } while (map[*y][*x] != ' ');
}

// ===========================================================================================


void init_map(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH]) {
    for (int y = 0; y < MAP_SIZE_HEIGHT; y++) {
        for (int x = 0; x < MAP_SIZE_WIDTH; x++) {
            if (x == 0 || x == MAP_SIZE_WIDTH - 1 || y == 0 || y == MAP_SIZE_HEIGHT - 1) {
                map[y][x] = '#';  // Wall
            } else {
                map[y][x] = ' ';  // Empty space
            }
        }
    }
	printf("\n\n");
}

// ===========================================================================================
// Prints Map out onto the screen - this will address for now spacing on top and on the left side to use empty space to depth the playing field


void print_map(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH]) {

    printf("\n\n\n\n");
    for (int y = 0; y < MAP_SIZE_HEIGHT; y++) {
		printf("    ");
        for (int x = 0; x < MAP_SIZE_WIDTH; x++) {
			print_color_char(map[y][x]);             // Prints tiles -- see tools.h for use
        }
        putchar('\n');
    }
	printf("\n\n\n\n");
}

// ===========================================================================================

void init_player(Player* player, char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH]) {
    int x, y;
    get_random_empty_location(map, &x, &y);
    player->x = x;
    player->y = y;
    player->hp = 10;
    player->ammo = 0;
    player->mugs = 0;
    player->kills = 0;
    player->exited = false;
    player->last_dir = 'w';
    map[y][x] = '@';
}

// ===========================================================================================

void place_entities(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH], Zombie zombies[], int count, char symbol) {
    for (int i = 0; i < count; i++) {
        int x, y;
        get_random_empty_location(map, &x, &y);
        zombies[i].x = x;
        zombies[i].y = y;
        zombies[i].hp = 5;
        zombies[i].alive = true;
        map[y][x] = symbol;
    }
}

// ===========================================================================================

void place_items(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH], int count, char symbol) {
    for (int i = 0; i < count; i++) {
        int x, y;
        get_random_empty_location(map, &x, &y);
        map[y][x] = symbol;
    }
}

// ===========================================================================================

void place_exit(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH]) {
    int x, y;
    get_random_empty_location(map, &x, &y);
    map[y][x] = '>';
}

// ===========================================================================================

void print_stats(const Player* player) {
    printf("HP: %d | Ammo: %d | Ales: %d | Kills: %d\n", player->hp, player->ammo, player->mugs, player->kills);
}

// ===========================================================================================

void move_player(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH], Player* player, Zombie zombies[], int* zombie_count, char direction) {
    int dx = 0, dy = 0;
    switch (direction) {
        case 'w': dy = -1; break;
        case 's': dy = 1; break;
        case 'a': dx = -1; break;
        case 'd': dx = 1; break;
        default: return;
    }

    int new_x = player->x + dx;
    int new_y = player->y + dy;
    char target = map[new_y][new_x];

    if (target == '#' || target == '@') return;  // Wall or self

    // Handle object interactions
    if (target == 'U') {
        player->hp += 2;
        if (player->hp > 10) player->hp = 10;
        player->mugs++;
        play_sound("gulp");
    } else if (target == 'o') {
        player->ammo++;
        play_sound("reload");
    } else if (target == 'Z') {
        for (int i = 0; i < *zombie_count; i++) {
            if (zombies[i].alive && zombies[i].x == new_x && zombies[i].y == new_y) {
                int player_roll = rand() % 20 + 1;
                int zombie_roll = rand() % 20 + 1;
                if (player_roll > zombie_roll) {
                    int dmg = (player_roll == 20) ? (rand() % 6 + 1) * 2 : rand() % 6 + 1;
                    zombies[i].hp -= dmg;
                    play_sound("slash");
                    if (zombies[i].hp <= 0) {
                        zombies[i].alive = false;
                        map[new_y][new_x] = ' ';
                        player->kills++;
                    }
                } else if (player_roll < zombie_roll) {
                    int dmg = (zombie_roll == 20) ? (rand() % 6 + 1) * 2 : rand() % 6 + 1;
                    player->hp -= dmg;
                    play_sound("hurt");
                } else {
                    player->hp -= 2;
                    zombies[i].hp -= 2;
                }
                return;
            }
        }
    } else if (target == '>') {
        player->exited = true;
        return;
    }

    // Move the player
    map[player->y][player->x] = ' ';
    player->x = new_x;
    player->y = new_y;
    map[new_y][new_x] = '@';
}

// ===========================================================================================

void fire_bullet(char map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH], Player* player, Zombie zombies[], int* zombie_count) {
    if (player->ammo <= 0) return;

    int dx = 0, dy = 0;
    switch (player->last_dir) {
        case 'w': dy = -1; break;
        case 's': dy = 1; break;
        case 'a': dx = -1; break;
        case 'd': dx = 1; break;
        default: return;
    }

    int bx = player->x + dx;
    int by = player->y + dy;
    for (int i = 0; i < 10; i++) {
        if (map[by][bx] == '#' || map[by][bx] == '>') return;
        if (map[by][bx] == 'Z') {
            for (int j = 0; j < *zombie_count; j++) {
                if (zombies[j].alive && zombies[j].x == bx && zombies[j].y == by) {
                    zombies[j].hp -= 5;
                    if (zombies[j].hp <= 0) {
                        zombies[j].alive = false;
                        map[by][bx] = ' ';
                        player->kills++;
                    }
                    player->ammo--;
                    play_sound("pew");
                    return;
                }
            }
        }
        bx += dx;
        by += dy;
    }
    player->ammo--;
}

// ===========================================================================================

int calculate_score(const Player* player) {
    return player->mugs + (player->kills * 2) + (player->exited ? 10 : 0);
}
