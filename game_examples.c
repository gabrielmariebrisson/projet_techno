/**
 * @file game_examples.c
 * @brief Game Examples.
 * @copyright University of Bordeaux. All rights reserved, 2021.
 *
 **/

#include "game_examples.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_ext.h"

/* ************************************************************************** */
/*                                  MISC                                      */
/* ************************************************************************** */

#define LIGHTEDBULB (S_LIGHTBULB | F_LIGHTED)
#define LIGHTEDBULBERR (S_LIGHTBULB | F_LIGHTED | F_ERROR)
#define BLACK2ERR (S_BLACK2 | F_ERROR)

/* ************************************************************************** */
/*                          BASIC GAMES (V1)                                  */
/* ************************************************************************** */

square default_squares[] = {
    S_BLANK,  S_BLANK,  S_BLACK1, S_BLANK, S_BLANK,  S_BLANK,  S_BLANK,  /* row 0 */
    S_BLANK,  S_BLANK,  S_BLACK2, S_BLANK, S_BLANK,  S_BLANK,  S_BLANK,  /* row 1 */
    S_BLANK,  S_BLANK,  S_BLANK,  S_BLANK, S_BLANK,  S_BLACKU, S_BLACK2, /* row 2 */
    S_BLANK,  S_BLANK,  S_BLANK,  S_BLANK, S_BLANK,  S_BLANK,  S_BLANK,  /* row 3 */
    S_BLACK1, S_BLACKU, S_BLANK,  S_BLANK, S_BLANK,  S_BLANK,  S_BLANK,  /* row 4 */
    S_BLANK,  S_BLANK,  S_BLANK,  S_BLANK, S_BLACK2, S_BLANK,  S_BLANK,  /* row 5 */
    S_BLANK,  S_BLANK,  S_BLANK,  S_BLANK, S_BLACKU, S_BLANK,  S_BLANK,  /* row 6 */
};

/* ************************************************************************** */

square solution_squares[] = {
    LIGHTEDBULB, F_LIGHTED,   S_BLACK1,    LIGHTEDBULB, F_LIGHTED,   F_LIGHTED,   F_LIGHTED,   /* row 0 */
    F_LIGHTED,   LIGHTEDBULB, S_BLACK2,    F_LIGHTED,   F_LIGHTED,   F_LIGHTED,   LIGHTEDBULB, /* row 1 */
    F_LIGHTED,   F_LIGHTED,   LIGHTEDBULB, F_LIGHTED,   F_LIGHTED,   S_BLACKU,    S_BLACK2,    /* row 2 */
    F_LIGHTED,   F_LIGHTED,   F_LIGHTED,   F_LIGHTED,   F_LIGHTED,   F_LIGHTED,   LIGHTEDBULB, /* row 3 */
    S_BLACK1,    S_BLACKU,    F_LIGHTED,   F_LIGHTED,   LIGHTEDBULB, F_LIGHTED,   F_LIGHTED,   /* row 4 */
    LIGHTEDBULB, F_LIGHTED,   F_LIGHTED,   F_LIGHTED,   S_BLACK2,    LIGHTEDBULB, F_LIGHTED,   /* row 5 */
    F_LIGHTED,   LIGHTEDBULB, F_LIGHTED,   F_LIGHTED,   S_BLACKU,    F_LIGHTED,   F_LIGHTED,   /* row 6 */
};

/* ************************************************************************** */

square other_squares[] = {
    LIGHTEDBULBERR, F_LIGHTED, S_BLACK1,  S_BLANK,   S_BLANK,   S_BLANK,   S_BLANK,   /* row 0 */
    F_LIGHTED,      S_BLANK,   S_BLACK2,  S_BLANK,   S_BLANK,   S_BLANK,   S_BLANK,   /* row 1 */
    F_LIGHTED,      S_BLANK,   S_BLANK,   S_BLANK,   S_BLANK,   S_BLACKU,  BLACK2ERR, /* row 2 */
    LIGHTEDBULBERR, F_LIGHTED, F_LIGHTED, F_LIGHTED, F_LIGHTED, F_LIGHTED, F_LIGHTED, /* row 3 */
    S_BLACK1,       S_BLACKU,  S_BLANK,   S_BLANK,   S_BLANK,   S_BLANK,   S_BLANK,   /* row 4 */
    S_BLANK,        S_BLANK,   S_BLANK,   S_BLANK,   S_BLACK2,  S_BLANK,   S_BLANK,   /* row 5 */
    S_BLANK,        S_BLANK,   S_BLANK,   S_BLANK,   S_BLACKU,  S_BLANK,   S_MARK,    /* row 6 */
};

/* ************************************************************************** */
/*                               EXT GAMES (V2)                               */
/* ************************************************************************** */

// https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/lightup.html#4x4:b0bBd2e

/*
   0123
   ----
0 |*.0.|
1 |.w.*|
2 |.*2.|
3 |..*.|
   ----
*/

square ext_4x4_squares[] = {
    S_BLANK, S_BLANK,  S_BLACK0, S_BLANK, /* row 0 */
    S_BLANK, S_BLACKU, S_BLANK,  S_BLANK, /* row 1 */
    S_BLANK, S_BLANK,  S_BLACK2, S_BLANK, /* row 2 */
    S_BLANK, S_BLANK,  S_BLANK,  S_BLANK, /* row 3 */
};

/* ************************************************************************** */

square sol_4x4_squares[] = {
    LIGHTEDBULB, F_LIGHTED,   S_BLACK0,    F_LIGHTED,   /* row 0 */
    F_LIGHTED,   S_BLACKU,    F_LIGHTED,   LIGHTEDBULB, /* row 1 */
    F_LIGHTED,   LIGHTEDBULB, S_BLACK2,    F_LIGHTED,   /* row 2 */
    F_LIGHTED,   F_LIGHTED,   LIGHTEDBULB, F_LIGHTED,   /* row 3 */
};

/* ************************************************************************** */

// https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/lightup.html#10x3:c1d1B1h1B0d0c

square ext_3x10_squares[] = {
    S_BLANK,  S_BLANK,  S_BLANK, S_BLACK1, S_BLANK,  /* row 0 */
    S_BLANK,  S_BLANK,  S_BLANK, S_BLACK1, S_BLACKU, /* row 0 */
    S_BLACK1, S_BLANK,  S_BLANK, S_BLANK,  S_BLANK,  /* row 1 */
    S_BLANK,  S_BLANK,  S_BLANK, S_BLANK,  S_BLACK1, /* row 1 */
    S_BLACKU, S_BLACK0, S_BLANK, S_BLANK,  S_BLANK,  /* row 2 */
    S_BLANK,  S_BLACK0, S_BLANK, S_BLANK,  S_BLANK,  /* row 2 */
};

/* ************************************************************************** */

square sol_3x10_squares[] = {
    LIGHTEDBULB, F_LIGHTED, F_LIGHTED,   S_BLACK1,    F_LIGHTED,   /* row 0 */
    F_LIGHTED,   F_LIGHTED, LIGHTEDBULB, S_BLACK1,    S_BLACKU,    /* row 0 */
    S_BLACK1,    F_LIGHTED, F_LIGHTED,   LIGHTEDBULB, F_LIGHTED,   /* row 1 */
    F_LIGHTED,   F_LIGHTED, F_LIGHTED,   F_LIGHTED,   S_BLACK1,    /* row 1 */
    S_BLACKU,    S_BLACK0,  F_LIGHTED,   F_LIGHTED,   LIGHTEDBULB, /* row 2 */
    F_LIGHTED,   S_BLACK0,  F_LIGHTED,   F_LIGHTED,   LIGHTEDBULB, /* row 2 */
};

/* ************************************************************************** */

square ext_5x1_squares[] = {
    S_BLANK,  /* row 0 */
    S_BLANK,  /* row 1 */
    S_BLACK0, /* row 2 */
    S_BLANK,  /* row 3 */
    S_BLANK,  /* row 4 */

};

/* ************************************************************************** */

/*
     01
     --
  0 |4*| <- 4 lightbulbs adjacent (and not 2) because of wrapping!
  1 |*.|
     --
  */

square ext_2x2w_squares[] = {
    S_BLACK4, S_BLANK, /* row 0 */
    S_BLANK, S_BLANK,  /* row 1 */
};

/* ************************************************************************** */

/* l 0 1 l 1 0 */
square sol_2x2w_squares[] = {
    S_BLACK4, LIGHTEDBULB,  /* row 0 */
    LIGHTEDBULB, F_LIGHTED, /* row 1 */
};

/* ************************************************************************** */

/*
   012
   ---
0 |*w2|
1 |.ww|
2 |..*|
   ---
*/

square ext_3x3w_squares[] = {
    S_BLANK, S_BLACKU, S_BLACK2, /* row 0 */
    S_BLANK, S_BLACKU, S_BLACKU, /* row 1 */
    S_BLANK, S_BLANK,  S_BLANK,  /* row 2 */
};

/* ************************************************************************** */

/* l 0 0 l 2 2 */
square sol_3x3w_squares[] = {
    LIGHTEDBULB, S_BLACKU,  S_BLACK2,    /* row 0 */
    F_LIGHTED,   S_BLACKU,  S_BLACKU,    /* row 1 */
    F_LIGHTED,   F_LIGHTED, LIGHTEDBULB, /* row 2 */
};

/* ************************************************************************** */

/*
   012
   ---
0 |...|
1 |w*1|
2 |*2.|
3 |..*|
4 |...|
   ---
*/

square ext_5x3w_squares[] = {
    S_BLANK,  S_BLANK,  S_BLANK,  /* row 0 */
    S_BLACKU, S_BLANK,  S_BLACK1, /* row 1 */
    S_BLANK,  S_BLACK2, S_BLANK,  /* row 2 */
    S_BLANK,  S_BLANK,  S_BLANK,  /* row 3 */
    S_BLANK,  S_BLANK,  S_BLANK,  /* row 4 */
};

/* ************************************************************************** */

/* l 1 1 l 2 0 l 3 2 */
square sol_5x3w_squares[] = {
    F_LIGHTED,   F_LIGHTED,   F_LIGHTED,   /* row 0 */
    S_BLACKU,    LIGHTEDBULB, S_BLACK1,    /* row 1 */
    LIGHTEDBULB, S_BLACK2,    F_LIGHTED,   /* row 2 */
    F_LIGHTED,   F_LIGHTED,   LIGHTEDBULB, /* row 3 */
    F_LIGHTED,   F_LIGHTED,   F_LIGHTED,   /* row 4 */
};

/* ************************************************************************** */
