/**
 * @file game_private.c
 * @copyright University of Bordeaux. All rights reserved, 2021.
 **/

#include "game_private.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_ext.h"
#include "queue.h"

/* ************************************************************************** */
/*                             STACK ROUTINES                                 */
/* ************************************************************************** */

void _stack_push_move(queue* q, move m)
{
  assert(q);
  move* pm = malloc(sizeof(move));
  assert(pm);
  *pm = m;
  queue_push_head(q, pm);
}

/* ************************************************************************** */

move _stack_pop_move(queue* q)
{
  assert(q);
  move* pm = queue_pop_head(q);
  assert(pm);
  move m = *pm;
  free(pm);
  return m;
}

/* ************************************************************************** */

bool _stack_is_empty(queue* q)
{
  assert(q);
  return queue_is_empty(q);
}

/* ************************************************************************** */

void _stack_clear(queue* q)
{
  assert(q);
  queue_clear_full(q, free);
  assert(queue_is_empty(q));
}

/* ************************************************************************** */
/*                          GAME PRIVATE ROUTINES                             */
/* ************************************************************************** */

static char image[255] = {
    [S_BLANK] = ' ', [S_BLACK] = '0', '1', '2', '3', '4', [S_BLACKU] = 'w', [S_LIGHTBULB] = '*', [S_MARK] = '-'};

/* ************************************************************************** */

char _square2str(square s)
{
  square state = s & S_MASK;
  square flags = s & F_MASK;
  if ((state == S_BLANK) && (flags & F_LIGHTED)) return '.';
  return image[state];
}

/* ************************************************************************** */

static int value[255] = {[' '] = S_BLANK,  ['0'] = S_BLACK0, ['1'] = S_BLACK1,    ['2'] = S_BLACK2, ['3'] = S_BLACK3,
                         ['4'] = S_BLACK4, ['w'] = S_BLACKU, ['*'] = S_LIGHTBULB, ['-'] = S_MARK};

/* ************************************************************************** */

int _str2square(char c)
{
  unsigned char uc = c;
  return value[uc] == 0 ? -1 : value[uc];
}

/* ************************************************************************** */

bool _check_square(square s)
{
  square state = s & S_MASK;
  square flags = s & F_MASK;
  if (state < S_START || state > S_END) return false;
  if ((flags & ~(F_LIGHTED | F_ERROR)) != 0) return false;
  return true;
}

/* ************************************************************************** */
/*                                 NEIGHBORHOOD                               */
/* ************************************************************************** */

/* Nota Bene: https://en.cppreference.com/w/c/language/array_initialization */
static int i_offset[] = {[HERE] = 0,     [UP] = -1,       [DOWN] = +1,     [LEFT] = 0,      [RIGHT] = 0,
                         [UP_LEFT] = -1, [UP_RIGHT] = -1, [DOWN_LEFT] = 1, [DOWN_RIGHT] = 1};
static int j_offset[] = {[HERE] = 0,     [UP] = 0,       [DOWN] = 0,       [LEFT] = -1,     [RIGHT] = 1,
                         [UP_LEFT] = -1, [UP_RIGHT] = 1, [DOWN_LEFT] = -1, [DOWN_RIGHT] = 1};

/* ************************************************************************** */

bool _inside(cgame g, int i, int j)
{
  assert(g);
  if (game_is_wrapping(g)) {
    i = (i + game_nb_rows(g)) % game_nb_rows(g);
    j = (j + game_nb_cols(g)) % game_nb_cols(g);
  }
  if (i < 0 || j < 0 || i >= (int)g->nb_rows || j >= (int)g->nb_cols) return false;
  return true;
}

/* ************************************************************************** */

bool _inside_neigh(cgame g, int i, int j, direction dir) { return _inside(g, i + i_offset[dir], j + j_offset[dir]); }

/* ************************************************************************** */

bool _next(cgame g, int* pi, int* pj, direction dir)
{
  assert(g);
  assert(pi && pj);
  int i = *pi;
  int j = *pj;
  assert(i >= 0 && j >= 0 && i < (int)g->nb_rows && j < (int)g->nb_cols);

  // move to the next square in a given direction
  i += i_offset[dir];
  j += j_offset[dir];

  if (game_is_wrapping(g)) {
    i = (i + game_nb_rows(g)) % game_nb_rows(g);
    j = (j + game_nb_cols(g)) % game_nb_cols(g);
  }
  if (!_inside(g, i, j)) return false;

  // update square coords
  *pi = i;
  *pj = j;

  return true;
}

/* ************************************************************************** */

bool _test(cgame g, int i, int j, square s, uint m)
{
  assert(g);
  assert(s >= S_START && s < S_END);
  if (game_is_wrapping(g)) {
    i = (i + game_nb_rows(g)) % game_nb_rows(g);
    j = (j + game_nb_cols(g)) % game_nb_cols(g);
  }
  if (!_inside(g, i, j)) return false;
  return ((SQUARE(g, i, j) & m) == s);
}

/* ************************************************************************** */

bool _test_neigh(cgame g, int i, int j, square s, uint m, direction dir)
{
  return _test(g, i + i_offset[dir], j + j_offset[dir], s, m);
}

/* ************************************************************************** */

bool _neigh(cgame g, uint i, uint j, square s, uint m, bool diag)
{
  assert(g);
  assert(s >= S_START && s < S_END);

  // orthogonally
  if (_test_neigh(g, i, j, s, m, UP) || _test_neigh(g, i, j, s, m, DOWN) || _test_neigh(g, i, j, s, m, LEFT) ||
      _test_neigh(g, i, j, s, m, RIGHT))
    return true;

  // diagonally
  return diag && (_test_neigh(g, i, j, s, m, UP_LEFT) || _test_neigh(g, i, j, s, m, UP_RIGHT) ||
                  _test_neigh(g, i, j, s, m, DOWN_LEFT) || _test_neigh(g, i, j, s, m, DOWN_RIGHT));
}

/* ************************************************************************** */

uint _neigh_size(cgame g, uint i, uint j, bool diag)
{
  assert(g);

  return _neigh_count(g, i, j, 0, 0, diag);
}

/* ************************************************************************** */

uint _neigh_count(cgame g, uint i, uint j, square s, uint m, bool diag)
{
  assert(g);

  // orthogonally
  uint count = _test_neigh(g, i, j, s, m, UP) + _test_neigh(g, i, j, s, m, DOWN) + _test_neigh(g, i, j, s, m, LEFT) +
               _test_neigh(g, i, j, s, m, RIGHT);

  // diagonally
  if (diag) {
    count += _test_neigh(g, i, j, s, m, UP_LEFT) + _test_neigh(g, i, j, s, m, UP_RIGHT) +
             _test_neigh(g, i, j, s, m, DOWN_LEFT) + _test_neigh(g, i, j, s, m, DOWN_RIGHT);
  }

  return count;
}

/* ************************************************************************** */
