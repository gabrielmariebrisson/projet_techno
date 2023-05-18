/**
 * @file game_test_v1.c
 * @brief Game Tests V1.
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
/*                            BASIC TESTS (V1)                                */
/* ************************************************************************** */

int test_new(void)
{
  game g = game_new(default_squares);
  bool test0 = check_game(g, default_squares);
  game_delete(g);
  if (test0) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_new_empty(void)
{
  game g = game_new_empty();
  bool test0 = check_game(g, NULL);
  game_delete(g);

  if (test0) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_copy(void)
{
  // game empty
  game g1 = game_new_empty();
  game g2 = game_copy(g1);
  bool test0 = check_game(g2, NULL);
  game_delete(g1);
  game_delete(g2);

  // game default
  game g3 = game_default();
  game g4 = game_copy(g3);
  bool test1 = check_game(g4, default_squares);
  game_delete(g3);
  game_delete(g4);

  // game solution
  game g5 = game_default_solution();
  game g6 = game_copy(g5);
  bool test2 = check_game(g6, solution_squares);
  game_delete(g5);
  game_delete(g6);

  // game other
  game g7 = game_new(other_squares);
  game g8 = game_copy(g7);
  bool test3 = check_game(g8, other_squares);
  game_delete(g7);
  game_delete(g8);

  if (test0 && test1 && test2 && test3) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_equal(void)
{
  game g1 = game_default();
  game g2 = game_default();
  game g3 = game_default();
  game g4 = game_default();

  bool test1 = (game_equal(g1, g2) == true);

  game_play_move(g3, 0, 0, S_LIGHTBULB);
  bool test2 = (game_equal(g1, g3) == false);

  game_set_square(g4, 0, 0, F_LIGHTED);
  bool test3 = (game_equal(g1, g4) == false);

  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  game_delete(g4);

  if (test1 && test2 && test3) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_delete(void)
{
  game g = game_new_empty();
  game_delete(g);  // nothing else to do...
  return EXIT_SUCCESS;
}

/* ************************************************************************** */

int test_set_square(void)
{
  game g = game_default();
  game_set_square(g, 0, 0, S_LIGHTBULB);
  game_set_square(g, 0, 2, S_BLANK);  // overwrite wall
  game_set_square(g, 1, 1, S_MARK);
  game_set_square(g, 2, 2, S_BLACK3 | F_ERROR);
  game_set_square(g, 3, 3, S_LIGHTBULB | F_LIGHTED | F_ERROR);
  square s1 = game_get_square(g, 0, 0);
  square s2 = game_get_square(g, 0, 2);
  square s3 = game_get_square(g, 1, 1);
  square s4 = game_get_square(g, 2, 2);
  square s5 = game_get_square(g, 3, 3);
  square s6 = game_get_square(g, 0, 1);
  bool test1 = (s1 == S_LIGHTBULB);
  bool test2 = (s2 == S_BLANK);
  bool test3 = (s3 == S_MARK);
  bool test4 = (s4 == (S_BLACK3 | F_ERROR));
  bool test5 = (s5 == (S_LIGHTBULB | F_LIGHTED | F_ERROR));
  bool test6 = (s6 == S_BLANK);
  game_delete(g);
  if (test1 && test2 && test3 && test4 && test5 && test6) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_get_square(void)
{
  game g = game_new(other_squares);
  square s1 = game_get_square(g, 0, 0);
  square s2 = game_get_square(g, 0, 1);
  square s3 = game_get_square(g, 0, 2);
  square s4 = game_get_square(g, 2, 6);
  square s5 = game_get_square(g, 6, 6);
  bool test1 = (s1 == (S_LIGHTBULB | F_LIGHTED | F_ERROR));
  bool test2 = (s2 == F_LIGHTED);
  bool test3 = (s3 == S_BLACK1);
  bool test4 = (s4 == (S_BLACK2 | F_ERROR));
  bool test5 = (s5 == S_MARK);
  game_delete(g);
  if (test1 && test2 && test3 && test4 && test5) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_get_state(void)
{
  game g = game_new(other_squares);
  square s1 = game_get_state(g, 0, 0);
  square s2 = game_get_state(g, 0, 1);
  square s3 = game_get_state(g, 0, 2);
  square s4 = game_get_state(g, 2, 6);
  bool test1 = (s1 == S_LIGHTBULB);
  bool test2 = (s2 == S_BLANK);
  bool test3 = (s3 == S_BLACK1);
  bool test4 = (s4 == S_BLACK2);
  game_delete(g);
  if (test1 && test2 && test3 && test4) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_get_flags(void)
{
  game g = game_new(other_squares);
  square s1 = game_get_flags(g, 0, 0);
  square s2 = game_get_flags(g, 0, 1);
  square s3 = game_get_flags(g, 0, 2);
  square s4 = game_get_flags(g, 2, 6);
  bool test1 = (s1 == (F_LIGHTED | F_ERROR));
  bool test2 = (s2 == F_LIGHTED);
  bool test3 = (s3 == 0);
  bool test4 = (s4 == F_ERROR);
  game_delete(g);
  if (test1 && test2 && test3 && test4) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_is_state(void)
{
  game g = game_new(other_squares);
  bool test1 = game_is_lightbulb(g, 0, 0) && !game_is_lightbulb(g, 0, 1);
  bool test2 = game_is_black(g, 0, 2) && game_is_black(g, 2, 5) && !game_is_black(g, 0, 1);
  bool test3 = game_is_blank(g, 0, 1) && !game_is_blank(g, 0, 0);
  bool test4 = game_is_marked(g, 6, 6) && !game_is_marked(g, 0, 0);
  bool test5 = (game_get_black_number(g, 0, 2) == 1) &&  //
               (game_get_black_number(g, 2, 6) == 2) &&  //
               (game_get_black_number(g, 2, 5) == -1);
  game_delete(g);
  if (test1 && test2 && test3 && test4 && test5) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_has_flag(void)
{
  game g = game_new(other_squares);
  bool test1 = (game_is_lighted(g, 0, 0) && game_is_lighted(g, 0, 1) && !game_is_lighted(g, 1, 1));
  bool test2 = (game_has_error(g, 0, 0) && game_has_error(g, 2, 6) && !game_has_error(g, 0, 1));
  game_delete(g);
  if (test1 && test2) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_play_move(void)
{
  // other
  game g0 = game_default();
  game_play_move(g0, 0, 0, S_LIGHTBULB);
  game_play_move(g0, 3, 0, S_LIGHTBULB);
  game_play_move(g0, 6, 6, S_MARK);
  game_play_move(g0, 0, 6, S_LIGHTBULB);
  game_play_move(g0, 0, 6, S_BLANK);
  bool test0 = check_game(g0, other_squares);
  game_delete(g0);

  // solution
  game g1 = game_default();
  game_play_move(g1, 0, 0, S_LIGHTBULB);
  game_play_move(g1, 1, 1, S_LIGHTBULB);
  game_play_move(g1, 2, 2, S_LIGHTBULB);
  game_play_move(g1, 0, 3, S_LIGHTBULB);
  game_play_move(g1, 1, 6, S_LIGHTBULB);
  game_play_move(g1, 3, 6, S_LIGHTBULB);
  game_play_move(g1, 4, 4, S_LIGHTBULB);
  game_play_move(g1, 5, 0, S_LIGHTBULB);
  game_play_move(g1, 5, 5, S_LIGHTBULB);
  game_play_move(g1, 6, 1, S_LIGHTBULB);
  bool test1 = check_game(g1, solution_squares);
  game_delete(g1);

  if (test0 && test1) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

/* This function checks that it is possible to play a move at a given position
in the grid. More precisely, a move is said to be legal: 1) if the coordinates
(i,j) are inside the grid, 2) if s is either a blank, light bulb or mark square,
and 3) if the current square at (i,j) is not a black square. */

int test_check_move(void)
{
  game g0 = game_new(other_squares);

  // some legal moves
  bool test0 = game_check_move(g0, 1, 1, S_LIGHTBULB) &&  //
               game_check_move(g0, 0, 0, S_BLANK) &&      //
               game_check_move(g0, 1, 1, S_MARK);

  // no flags
  bool test1 = !game_check_move(g0, 1, 1, S_LIGHTBULB | F_LIGHTED | F_ERROR) &&  //
               !game_check_move(g0, 0, 0, S_BLANK | F_LIGHTED) &&                //
               !game_check_move(g0, 1, 1, S_MARK | F_ERROR);

  // try to overwrite black wall
  bool test2 = !game_check_move(g0, 0, 2, S_LIGHTBULB) &&  //
               !game_check_move(g0, 2, 5, S_BLANK);

  // invalid coordinates
  bool test3 = !game_check_move(g0, 7, 7, S_LIGHTBULB) &&  //
               !game_check_move(g0, 0, 10, S_BLANK);

  game_delete(g0);
  if (test0 && test1 && test2 && test3) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_is_over(void)
{
  // default game
  game g1 = game_default();
  bool test1 = (game_is_over(g1) == false);
  game_delete(g1);

  // default solution
  game g2 = game_default_solution();
  bool test2 = (game_is_over(g2) == true);
  game_delete(g2);

  // empty game
  game g3 = game_new_empty();
  game_play_move(g3, 0, 0, S_LIGHTBULB);
  game_play_move(g3, 0, 1, S_MARK);
  game_play_move(g3, 1, 1, S_LIGHTBULB);
  game_play_move(g3, 2, 2, S_LIGHTBULB);
  game_play_move(g3, 3, 3, S_LIGHTBULB);
  game_play_move(g3, 4, 4, S_LIGHTBULB);
  game_play_move(g3, 5, 5, S_LIGHTBULB);
  bool test3 = (game_is_over(g3) == false);
  game_play_move(g3, 6, 6, S_LIGHTBULB);
  bool test4 = (game_is_over(g3) == true);
  game_delete(g3);

  // default game (put lightbulb everywhere)
  game g4 = game_default();
  for (uint i = 0; i < DEFAULT_SIZE; i++)
    for (uint j = 0; j < DEFAULT_SIZE; j++)
      if (!game_is_black(g4, i, j)) game_play_move(g4, i, j, S_LIGHTBULB);
  bool test5 = (game_is_over(g4) == false);
  game_delete(g4);

  if (test1 && test2 && test3 && test4 && test5) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_restart(void)
{
  // default solution
  game g1 = game_default_solution();
  game_restart(g1);
  bool test1 = check_game(g1, default_squares);
  game_delete(g1);

  // other
  game g2 = game_new(other_squares);
  game_restart(g2);
  bool test2 = check_game(g2, default_squares);
  game_delete(g2);

  if (test1 && test2) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_update_flags(void)
{
  // test error flags both on lightbulb and black wall
  game g0 = game_default();
  game_set_square(g0, 0, 0, S_LIGHTBULB);
  game_set_square(g0, 3, 0, S_LIGHTBULB);
  game_set_square(g0, 6, 6, S_MARK);
  game_set_square(g0, 0, 6, S_LIGHTBULB);
  game_set_square(g0, 0, 6, S_BLANK);
  game_update_flags(g0);
  bool test0 = check_game(g0, other_squares);
  game_delete(g0);

  // test error flags on lightbulb
  game g1 = game_default();
  game_set_square(g1, 0, 0, S_LIGHTBULB);
  game_set_square(g1, 1, 1, S_LIGHTBULB);
  game_set_square(g1, 2, 2, S_LIGHTBULB);
  game_set_square(g1, 0, 3, S_LIGHTBULB);
  game_set_square(g1, 1, 6, S_LIGHTBULB);
  game_set_square(g1, 3, 6, S_LIGHTBULB);
  game_set_square(g1, 4, 4, S_LIGHTBULB);
  game_set_square(g1, 5, 0, S_LIGHTBULB);
  game_set_square(g1, 5, 5, S_LIGHTBULB);
  game_set_square(g1, 6, 1, S_LIGHTBULB);
  game_update_flags(g1);
  bool test1 = check_game(g1, solution_squares);
  game_delete(g1);

  if (test0 && test1) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */