// ===========================================================================================
// tools.h - Univwersal tools to help with game coding
// ===========================================================================================

#ifndef TOOLS_H
#define TOOLS_H


#define LAST_MSG_SIZE 255

extern char last_action_msg[LAST_MSG_SIZE];

// ============================
// Function declarations
// ============================

// helper function replaces putchar so it allows us to colorize a single char - fancy
void print_color_char(char c);




// for adding the last action string added to the buffer for displaying  later.
void set_last_action_msg(const char *format, ...);


#endif
