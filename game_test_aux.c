/**
 * @file game_test_aux.c
 * @brief Game Tests Aux.
 * @copyright University of Bordeaux. All rights reserved, 2021.
 *
 **/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_examples.h"
#include "game_ext.h"
#include "game_test.h"

/* ************************************************************************** */
/*                                AUX TESTS                                   */
/* ************************************************************************** */

int test_print(void)
{
  game g = game_default();
  game_print(g);  // just print it...
  game_delete(g);
  return EXIT_SUCCESS;
}

/* ************************************************************************** */

int test_default(void)
{
  game g0 = game_default();
  bool test0 = check_game(g0, default_squares);
  game_delete(g0);
  if (test0) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_default_solution(void)
{
  game g = game_default_solution();
  bool test0 = check_game(g, solution_squares);
  game_delete(g);
  if (test0) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */
