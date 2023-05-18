/**
 * @file game_test_v2.c
 * @brief Game Tests V2.
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
/*                              EXT TESTS (V2)                                */
/* ************************************************************************** */

int test_new_ext(void)
{
  game g0 = game_new_ext(4, 4, ext_4x4_squares, false);
  bool test0 = check_game_ext(g0, 4, 4, ext_4x4_squares, false);
  game_delete(g0);
  game g1 = game_new_ext(4, 4, sol_4x4_squares, false);
  bool test1 = check_game_ext(g1, 4, 4, sol_4x4_squares, false);
  game_delete(g1);
  game g2 = game_new_ext(3, 10, ext_3x10_squares, false);
  bool test2 = check_game_ext(g2, 3, 10, ext_3x10_squares, false);
  game_delete(g2);
  game g3 = game_new_ext(3, 10, sol_3x10_squares, false);
  bool test3 = check_game_ext(g3, 3, 10, sol_3x10_squares, false);
  game_delete(g3);

  if (test0 && test1 && test2 && test3) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_new_empty_ext(void)
{
  game g0 = game_new_empty_ext(4, 4, false);
  bool test0 = check_game_ext(g0, 4, 4, NULL, false);
  game_delete(g0);

  if (test0) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_equal_ext(void)
{
  game g1 = game_new_ext(3, 10, ext_3x10_squares, false);
  game g2 = game_new_ext(3, 10, ext_3x10_squares, false);
  game g3 = game_new_ext(3, 10, ext_3x10_squares, true);

  // same game
  bool test1 = (game_equal(g1, g2) == true);

  // set a single different square
  game_set_square(g2, 2, 9, S_LIGHTBULB);
  bool test2 = (game_equal(g1, g2) == false);

  // different options
  bool test3 = (game_equal(g1, g3) == false);

  game_delete(g1);
  game_delete(g2);
  game_delete(g3);

  if (test1 && test2 && test3) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_copy_ext(void)
{
  // game empty 3x5w
  game g1 = game_new_empty_ext(3, 5, true);
  game g2 = game_copy(g1);
  bool test0 = check_game_ext(g2, 3, 5, NULL, true);
  game_delete(g1);
  game_delete(g2);

  // game 3x10
  game g3 = game_new_ext(3, 10, sol_3x10_squares, false);
  game g4 = game_copy(g3);
  bool test1 = check_game_ext(g4, 3, 10, sol_3x10_squares, false);
  game_delete(g3);
  game_delete(g4);

  if (test0 && test1) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_game_1d(void)
{
  // try to win this game
  game g0 = game_new_ext(5, 1, ext_5x1_squares, false);
  bool test0 = check_game_ext(g0, 5, 1, ext_5x1_squares, false);
  game_play_move(g0, 0, 0, S_LIGHTBULB);
  bool test1 = (game_is_over(g0) == false);
  game_play_move(g0, 4, 0, S_LIGHTBULB);
  bool test2 = game_is_over(g0);
  game_delete(g0);

  if (test0 && test1 && test2) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_game_wrapping_5x3(void)
{
  // try to win a game with wrapping option
  game g0 = game_new_ext(5, 3, ext_5x3w_squares, true);
  bool test0 = check_game_ext(g0, 5, 3, ext_5x3w_squares, true);
  game_play_move(g0, 1, 1, S_LIGHTBULB);
  game_play_move(g0, 2, 0, S_LIGHTBULB);
  game_play_move(g0, 3, 2, S_LIGHTBULB);
  bool test1 = check_game_ext(g0, 5, 3, sol_5x3w_squares, true);
  bool test2 = game_is_over(g0);
  game_delete(g0);

  if (test0 && test1 && test2) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_game_wrapping_3x3(void)
{
  // try to win a game with wrapping option
  game g0 = game_new_ext(3, 3, ext_3x3w_squares, true);
  bool test0 = check_game_ext(g0, 3, 3, ext_3x3w_squares, true);
  game_play_move(g0, 0, 0, S_LIGHTBULB);
  game_play_move(g0, 2, 2, S_LIGHTBULB);
  bool test1 = check_game_ext(g0, 3, 3, sol_3x3w_squares, true);
  bool test2 = game_is_over(g0);
  game_delete(g0);

  /*
     012
     ---
  0 |*w2|
  1 |.ww|
  2 |..*|
     ---
  */

  if (test0 && test1 && test2) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_game_wrapping_2x2(void)
{
  // check really dummy 2x2 board with wrapping option
  game g0 = game_new_ext(2, 2, ext_2x2w_squares, true);
  bool test0 = check_game_ext(g0, 2, 2, ext_2x2w_squares, true);
  game_play_move(g0, 0, 1, S_LIGHTBULB);
  game_play_move(g0, 1, 0, S_LIGHTBULB);
  bool test1 = check_game_ext(g0, 2, 2, sol_2x2w_squares, true);
  bool test2 = game_is_over(g0);
  game_delete(g0);

  /*
       01
       --
    0 |4*| <- 4 lightbulbs adjacent (and not 2) because of wrapping!
    1 |*.|
       --
  */

  if (test0 && test1 && test2) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_game_wrapping_error(void)
{
  // check error flags on lightbulb with wrapping
  game g1 = game_new_empty_ext(3, 3, true);
  game_set_square(g1, 1, 1, S_BLACKU);
  game_play_move(g1, 1, 0, S_LIGHTBULB);
  bool test3 = game_is_lighted(g1, 1, 0) && game_is_lighted(g1, 1, 2);
  game_play_move(g1, 1, 2, S_LIGHTBULB);
  bool test4 = game_has_error(g1, 1, 0) && game_has_error(g1, 1, 2);
  game_delete(g1);

  /*
     012
     ---
  0 |. .|
  1 |*w*| <- error
  2 |. .|
     ---
  */

  // check error flags on black wall with wrapping
  game g3 = game_new_empty_ext(3, 3, true);
  game_set_square(g3, 0, 1, S_BLACKU);
  game_set_square(g3, 0, 2, S_BLACK2);
  game_set_square(g3, 1, 1, S_BLACKU);
  game_set_square(g3, 1, 2, S_BLACKU);
  game_play_move(g3, 2, 0, S_LIGHTBULB);
  bool test6 = game_has_error(g3, 0, 2);
  game_delete(g3);

  /*
     012
     ---
  0 |.w2| <- all ligthed, but error on black wall '2'
  1 |.ww|
  2 |*..|
     ---
  */

  if (test3 && test4 && test6) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_undo_one(void)
{
  // undo a one move of default solution
  game g0 = game_default();
  game_play_move(g0, 0, 0, S_LIGHTBULB);
  game_play_move(g0, 1, 1, S_LIGHTBULB);
  game_play_move(g0, 2, 2, S_LIGHTBULB);
  game_play_move(g0, 0, 3, S_LIGHTBULB);
  game_play_move(g0, 1, 6, S_LIGHTBULB);
  game_play_move(g0, 3, 6, S_LIGHTBULB);
  game_play_move(g0, 4, 4, S_LIGHTBULB);
  game_play_move(g0, 5, 0, S_LIGHTBULB);
  bool test0 = (game_get_square(g0, 6, 0) == F_LIGHTED);
  game_play_move(g0, 6, 0, S_LIGHTBULB);  // bad move
  bool test1 = (game_get_square(g0, 6, 0) == (S_LIGHTBULB | F_LIGHTED | F_ERROR));
  game_undo(g0);  // undo it
  bool test2 = (game_get_square(g0, 6, 0) == F_LIGHTED);
  game_play_move(g0, 6, 1, S_LIGHTBULB);
  game_play_move(g0, 5, 5, S_LIGHTBULB);
  bool test3 = check_game(g0, solution_squares);
  game_delete(g0);
  if (test0 && test1 && test2 && test3) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_undo_redo_some(void)
{
  // undo & redo some moves
  game g0 = game_default();
  game_play_move(g0, 0, 0, S_LIGHTBULB);
  game_play_move(g0, 1, 1, S_LIGHTBULB);
  game_play_move(g0, 2, 2, S_LIGHTBULB);
  game_play_move(g0, 0, 3, S_LIGHTBULB);
  game_play_move(g0, 1, 6, S_LIGHTBULB);
  game_play_move(g0, 3, 6, S_LIGHTBULB);
  game_play_move(g0, 4, 4, S_LIGHTBULB);
  game_play_move(g0, 5, 5, S_LIGHTBULB);
  game_play_move(g0, 5, 1, S_LIGHTBULB);  // this is a bad move!
  game_undo(g0);                          // undo (5,1) => cancel bad move
  game_undo(g0);                          // undo (5,5) => cancel good move
  game_redo(g0);                          // redo (5,5) => redo good move
  game_play_move(g0, 5, 0, S_LIGHTBULB);
  game_play_move(g0, 6, 1, S_LIGHTBULB);
  bool test0 = check_game(g0, solution_squares);
  game_delete(g0);

  if (test0) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_undo_redo_all(void)
{
  // undo & redo all moves
  game g1 = game_default();
  bool test1 = check_game(g1, default_squares);
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
  bool test2 = check_game(g1, solution_squares);
  for (int k = 0; k < 10; k++) game_undo(g1);
  bool test3 = check_game(g1, default_squares);
  for (int k = 0; k < 10; k++) game_redo(g1);
  bool test4 = check_game(g1, solution_squares);
  game_delete(g1);

  if (test1 && test2 && test3 && test4) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */

int test_restart_undo(void)
{
  game g0 = game_default();
  game_play_move(g0, 0, 0, S_LIGHTBULB);
  game_play_move(g0, 1, 1, S_LIGHTBULB);
  game_play_move(g0, 2, 2, S_LIGHTBULB);
  game_play_move(g0, 2, 2, S_BLANK);
  game_restart(g0);
  game_undo(g0);  // it should do nothing
  bool test0 = check_game(g0, default_squares);
  game_delete(g0);
  if (test0) return EXIT_SUCCESS;
  return EXIT_FAILURE;
}

/* ************************************************************************** */
