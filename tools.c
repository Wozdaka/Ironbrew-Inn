// ===========================================================================================
// tools.h - Univwersal tools to help with game coding
// ===========================================================================================


#include <stdarg.h>
#include <stdio.h>
#include "tools.h"




// ===========================================================================================
// buffer to hold last action message
// and helper code to populate it with goodness
// ===========================================================================================

char last_action_msg[LAST_MSG_SIZE];

void set_last_action_msg(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(last_action_msg, sizeof(last_action_msg), format, args);
    va_end(args);
}

/* 

notes to myself for ansi codes - when in doubt - ask Kate 

Code		Description

\033[0m		Reset all styles
\033[1m		Bold
\033[30m	Black text
\033[31m	Red text
\033[32m	Green text
\033[33m	Yellow text
\033[34m	Blue text
\033[35m	Magenta text
\033[36m	Cyan text
\033[37m	White text

*/

// ===========================================================================================
// Utility: print_color_char - renders map symbols in color

void print_color_char(char c) {
    switch (c) {
        case '@':
            printf("\033[1;36m@\033[0m");  // Cyan - Player
            break;
        case 'Z':
            printf("\033[1;31mZ\033[0m");  // Red - Zombie
            break;
        case 'U':
            printf("\033[1;33mU\033[0m");  // Yellow - Ale Mug
            break;
        case 'o':
            printf("\033[1;35mo\033[0m");  // Magenta - Ammo
            break;
        case '>':
            printf("\033[1;32m>\033[0m");  // Green - Exit
            break;
        case '#':
            printf("\033[0;37m#\033[0m");  // Light Gray - Wall
            break;
        case ' ':
            putchar(' ');  // Plain empty space
            break;
        default:
            putchar(c);  // Fallback for any other chars
            break;
    }
}
