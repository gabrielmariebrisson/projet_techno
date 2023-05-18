// Random Game Generator
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_ext.h"

/* ************************************************************************** */

static uint nb_neigh_lightbulbs(cgame g, uint i, uint j)
{
  assert(g);
  int nb_rows = game_nb_rows(g);
  int nb_cols = game_nb_cols(g);
  assert(i < nb_rows);
  assert(j < nb_cols);

  int i_up = i - 1;
  int i_down = i + 1;
  int j_left = j - 1;
  int j_right = j + 1;
  if (game_is_wrapping(g)) {
    i_up = (i_up + nb_rows) % nb_rows;
    i_down = (i_down + nb_rows) % nb_rows;
    j_left = (j_left + nb_cols) % nb_cols;
    j_right = (j_right + nb_cols) % nb_cols;
  }

  uint count = 0;
  if ((i_up >= 0) && game_is_lightbulb(g, i_up, j)) count++;
  if ((i_down < nb_rows) && game_is_lightbulb(g, i_down, j)) count++;
  if ((j_left >= 0) && game_is_lightbulb(g, i, j_left)) count++;
  if ((j_right < nb_cols) && game_is_lightbulb(g, i, j_right)) count++;

  return count;
}

/* ************************************************************************** */

static uint nb_unlit_squares(cgame g)
{
  int nb = 0;
  for (uint i = 0; i < game_nb_rows(g); i++)
    for (uint j = 0; j < game_nb_cols(g); j++)
      if (!game_is_lighted(g, i, j) && game_is_blank(g, i, j)) nb++;
  return nb;
}

/* ************************************************************************** */

/**
 * Create a random game with a given size and number of walls
 *
 * @param nb_rows the number of rows of the game
 * @param nb_cols the number of columns of the game
 * @param wrapping wrapping option
 * @param nb_walls the number of walls to add
 * @param with_solution if true, the game contains the solution, otherwise only walls
 *
 * @return the generated random game
 */

game game_random(uint nb_rows, uint nb_cols, bool wrapping, uint nb_walls, bool with_solution)
{
  assert(nb_walls <= nb_rows * nb_cols);

  // step 0: create an empty game
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping);

  // step 1: add random black walls
  uint k = 0;
  while (k < nb_walls) {
    uint i = rand() % nb_rows;
    uint j = rand() % nb_cols;
    if (!game_is_black(g, i, j)) {
      game_set_square(g, i, j, S_BLACKU);
      k++;
    }
  }
  game_update_flags(g);

  // step 2: add lightbulbs until every squares are lighted
  uint nb_unlit = nb_unlit_squares(g);
  while (nb_unlit != 0) {
    uint random_unlit_num = rand() % nb_unlit;
    uint num = 0;
    for (uint i = 0; i < game_nb_rows(g); i++)
      for (uint j = 0; j < game_nb_cols(g); j++) {
        if (!game_is_lighted(g, i, j) && game_is_blank(g, i, j)) {
          if (num == random_unlit_num) {
            game_set_square(g, i, j, S_LIGHTBULB);
            game_update_flags(g);
          }
          num++;
        }
      }
    nb_unlit = nb_unlit_squares(g);
  }

  // step 3 : set some black wall numbers
  for (uint i = 0; i < game_nb_rows(g); i++)
    for (uint j = 0; j < game_nb_cols(g); j++) {
      if (game_is_black(g, i, j)) {
        if (rand() % 2 == 0) {
          int nb_lightbulbs = nb_neigh_lightbulbs(g, i, j);
          game_set_square(g, i, j, S_BLACK + nb_lightbulbs);
        }
      }
    }

  // check
  assert(game_is_over(g));

  if (!with_solution) game_restart(g);
  return g;
}

/* ************************************************************************** */
