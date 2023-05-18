/**
 * @file game.c
 * @copyright University of Bordeaux. All rights reserved, 2021.
 **/

#include "game.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game_ext.h"
#include "game_private.h"
#include "queue.h"

/* ************************************************************************** */
/*                                INTERNAL                                    */
/* ************************************************************************** */

static void _update_lighted_flags(game g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);

  // we just have put a lightbulb at (i,j) and we want to update lighted flags
  square s = STATE(g, i, j);
  assert(s == S_LIGHTBULB);

  /* update square (i,j) */
  SQUARE(g, i, j) |= F_LIGHTED;

  uint startdir = UP;
  uint enddir = RIGHT;
  uint dim = MAX(g->nb_rows, g->nb_cols);

  // update lighted flags in all directions
  for (uint dir = startdir; dir <= enddir; dir++) {
    int ii = i;
    int jj = j;
    for (uint k = 1; k < dim; k++) {
      bool cont = _next(g, &ii, &jj, dir);  // update next coord (ii,jj)
      if (!cont) break;
      if (STATE(g, ii, jj) & S_BLACK) break;
      SQUARE(g, ii, jj) |= F_LIGHTED;
    }
  }
}

/* ************************************************************************** */

static bool _check_lightbulb_error(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  // square s = STATE(g, i, j);
  // assert(s == S_LIGHTBULB);

  // update lighted flags in all directions
  direction dirs[] = {UP, DOWN, LEFT, RIGHT};
  for (uint d = 0; d < 4; d++) {
    direction dir = dirs[d];
    int ii = i;
    int jj = j;
    uint dim = 0;
    if (dir == UP || dir == DOWN) dim = g->nb_rows;
    if (dir == LEFT || dir == RIGHT) dim = g->nb_cols;
    for (uint k = 1; k < dim; k++) {
      bool cont = _next(g, &ii, &jj, dir);  // update next coord (ii,jj)
      if (!cont) break;
      if (STATE(g, ii, jj) & S_BLACK) break;
      if (STATE(g, ii, jj) == S_LIGHTBULB) return false;
    }
  }

  return true;
}

/* ************************************************************************** */

static bool _check_blackwall_error(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  square s = STATE(g, i, j);
  assert(s & S_BLACK);
  if (s == S_BLACKU) return true; /* no constraint for unumbered wall */
  int expected = game_get_black_number(g, i, j);
  int nb_lightbulbs = _neigh_count(g, i, j, S_LIGHTBULB, S_MASK, false);
  int nb_blanks = _neigh_count(g, i, j, S_BLANK, A_MASK, false);  // blank state, without lighted flag

  // 1) too many lightbulbs
  if (nb_lightbulbs > expected) return false;

  // 2) not enough blank squares (without lighted flag) to place the expected number of lightbulbs
  if (nb_blanks < (expected - nb_lightbulbs)) return false;

  return true;
}

/* ************************************************************************** */
/*                                 GAME BASIC                                 */
/* ************************************************************************** */

game game_new(square* squares) { return game_new_ext(DEFAULT_SIZE, DEFAULT_SIZE, squares, false); }

/* ************************************************************************** */

game game_new_empty(void) { return game_new_empty_ext(DEFAULT_SIZE, DEFAULT_SIZE, false); }

/* ************************************************************************** */

game game_copy(cgame g)
{
  game gg = game_new_ext(g->nb_rows, g->nb_cols, g->squares, g->wrapping);
  return gg;
}

/* ************************************************************************** */

bool game_equal(cgame g1, cgame g2)
{
  assert(g1 && g2);

  if (g1->nb_rows != g2->nb_rows) return false;
  if (g1->nb_cols != g2->nb_cols) return false;

  for (uint i = 0; i < g1->nb_rows; i++)
    for (uint j = 0; j < g1->nb_cols; j++) {
      if (SQUARE(g1, i, j) != SQUARE(g2, i, j)) return false;
    }

  if (g1->wrapping != g2->wrapping) return false;

  return true;
}

/* ************************************************************************** */

void game_delete(game g)
{
  free(g->squares);
  queue_free_full(g->undo_stack, free);
  queue_free_full(g->redo_stack, free);
  free(g);
}

/* ************************************************************************** */

void game_set_square(game g, uint i, uint j, square s)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  SQUARE(g, i, j) = s;
}

/* ************************************************************************** */

square game_get_square(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  return SQUARE(g, i, j);
}

/* ************************************************************************** */

square game_get_state(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  return STATE(g, i, j);
}

/* ************************************************************************** */

square game_get_flags(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  return FLAGS(g, i, j);
}

/* ************************************************************************** */

void game_update_flags(game g)
{
  assert(g);

  // 0) reset all flags
  for (uint i = 0; i < g->nb_rows; i++)
    for (uint j = 0; j < g->nb_cols; j++) SQUARE(g, i, j) = STATE(g, i, j);

  // 1) update lighted flag
  for (uint i = 0; i < g->nb_rows; i++)
    for (uint j = 0; j < g->nb_cols; j++)
      if (STATE(g, i, j) == S_LIGHTBULB) _update_lighted_flags(g, i, j);

  // 2) update error flag
  for (uint i = 0; i < g->nb_rows; i++)
    for (uint j = 0; j < g->nb_cols; j++) {
      if (game_is_lightbulb(g, i, j) && !_check_lightbulb_error(g, i, j)) SQUARE(g, i, j) |= F_ERROR;
      if (game_is_black(g, i, j) && !_check_blackwall_error(g, i, j)) SQUARE(g, i, j) |= F_ERROR;
    }
}

/* ************************************************************************** */

bool game_check_move(cgame g, uint i, uint j, square s)
{
  assert(g);
  if (i > g->nb_rows - 1) return false;
  if (j > g->nb_cols - 1) return false;
  if (s != S_BLANK && s != S_LIGHTBULB && s != S_MARK) return false;
  square cs = STATE(g, i, j);  // current state at position (i,j)
  if (s & S_BLACK) return false;
  if (cs & S_BLACK) return false;
  return true;  // regular move
}

/* ************************************************************************** */

void game_play_move(game g, uint i, uint j, square s)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  assert(s == S_BLANK || s == S_MARK || s == S_LIGHTBULB);
  bool black = game_is_black(g, i, j);
  assert(!black);
  square cs = STATE(g, i, j);  // save current state
  SQUARE(g, i, j) = s;         // update with new state

  // update flags
  game_update_flags(g);

  // save history
  _stack_clear(g->redo_stack);
  move m = {i, j, cs, s};
  _stack_push_move(g->undo_stack, m);
}

/* ************************************************************************** */

bool game_is_over(cgame g)
{
  assert(g);

  for (uint i = 0; i < g->nb_rows; i++)
    for (uint j = 0; j < g->nb_cols; j++) {
      // 1) check that square is lighted (except if it is a wall)
      if (!game_is_black(g, i, j) && !game_is_lighted(g, i, j)) return false;

      // 2) check if there are no errors
      if (game_has_error(g, i, j)) return false;
    }

  return true;
}

/* ************************************************************************** */

void game_restart(game g)
{
  assert(g);

  for (uint i = 0; i < g->nb_rows; i++)
    for (uint j = 0; j < g->nb_cols; j++) {
      square s = STATE(g, i, j);  // ignore flags
      if (s & S_BLACK)            // keep only wall
        SQUARE(g, i, j) = s;
      else  // blank other
        SQUARE(g, i, j) = S_BLANK;
    }

  // reset history
  _stack_clear(g->undo_stack);
  _stack_clear(g->redo_stack);
}

/* ************************************************************************** */

bool game_is_blank(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  square s = STATE(g, i, j);
  if (s == S_BLANK) return true;
  return false;
}

/* ************************************************************************** */

bool game_is_black(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  square s = STATE(g, i, j);
  if (s & S_BLACK) return true;
  return false;
}

/* ************************************************************************** */

bool game_is_lightbulb(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  square s = STATE(g, i, j);
  if (s == S_LIGHTBULB) return true;
  return false;
}

/* ************************************************************************** */

int game_get_black_number(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  square s = STATE(g, i, j);
  assert(s & S_BLACK);
  if (s == S_BLACKU)
    return -1;
  else
    return (s - S_BLACK);
}

/* ************************************************************************** */

bool game_is_marked(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  square s = STATE(g, i, j);
  if (s == S_MARK) return true;
  return false;
}

/* ************************************************************************** */

bool game_is_lighted(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  square f = FLAGS(g, i, j);
  if (f & F_LIGHTED) return true;
  return false;
}

/* ************************************************************************** */

bool game_has_error(cgame g, uint i, uint j)
{
  assert(g);
  assert(i < g->nb_rows);
  assert(j < g->nb_cols);
  square f = FLAGS(g, i, j);
  if (f & F_ERROR) return true;
  return false;
}

/* ************************************************************************** */
