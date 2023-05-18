/**
 * @file game_aux.c
 * @copyright University of Bordeaux. All rights reserved, 2021.
 **/

#include "game_aux.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_ext.h"
#include "game_private.h"

/* ************************************************************************** */

void game_print(cgame g)
{
  assert(g);
  printf("   ");
  for (uint j = 0; j < game_nb_cols(g); j++) printf("%d", j);
  printf("\n");
  printf("   ");
  for (uint j = 0; j < game_nb_cols(g); j++) printf("-");
  printf("\n");
  for (uint i = 0; i < game_nb_rows(g); i++) {
    printf("%d |", i);
    for (uint j = 0; j < game_nb_cols(g); j++) {
      square s = game_get_square(g, i, j);
      char c = _square2str(s);
      printf("%c", c);
    }
    printf("|\n");
  }
  printf("   ");
  for (uint j = 0; j < game_nb_cols(g); j++) printf("-");
  printf("\n");
}

/* ************************************************************************** */

// https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/lightup.html#7x7:b1f2iB2g1Bi2fBb
game game_default(void)
{
  square squares[] = {
      S_BLANK,  S_BLANK,  S_BLACK1, S_BLANK, S_BLANK,  S_BLANK,  S_BLANK,  /* row 0 */
      S_BLANK,  S_BLANK,  S_BLACK2, S_BLANK, S_BLANK,  S_BLANK,  S_BLANK,  /* row 1 */
      S_BLANK,  S_BLANK,  S_BLANK,  S_BLANK, S_BLANK,  S_BLACKU, S_BLACK2, /* row 2 */
      S_BLANK,  S_BLANK,  S_BLANK,  S_BLANK, S_BLANK,  S_BLANK,  S_BLANK,  /* row 3 */
      S_BLACK1, S_BLACKU, S_BLANK,  S_BLANK, S_BLANK,  S_BLANK,  S_BLANK,  /* row 4 */
      S_BLANK,  S_BLANK,  S_BLANK,  S_BLANK, S_BLACK2, S_BLANK,  S_BLANK,  /* row 5 */
      S_BLANK,  S_BLANK,  S_BLANK,  S_BLANK, S_BLACKU, S_BLANK,  S_BLANK,  /* row 6 */
  };
  return game_new(squares);
}

/* ************************************************************************** */

game game_default_solution(void)
{
  game g = game_default();
  game_play_move(g, 0, 0, S_LIGHTBULB);
  game_play_move(g, 1, 1, S_LIGHTBULB);
  game_play_move(g, 2, 2, S_LIGHTBULB);
  game_play_move(g, 0, 3, S_LIGHTBULB);
  game_play_move(g, 1, 6, S_LIGHTBULB);
  game_play_move(g, 3, 6, S_LIGHTBULB);
  game_play_move(g, 4, 4, S_LIGHTBULB);
  game_play_move(g, 5, 0, S_LIGHTBULB);
  game_play_move(g, 5, 5, S_LIGHTBULB);
  game_play_move(g, 6, 1, S_LIGHTBULB);
  return g;
}

/* ************************************************************************** */
