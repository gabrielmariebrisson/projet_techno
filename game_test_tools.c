#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_examples.h"
#include "game_ext.h"
#include "game_test.h"
#include "game_tools.h"

/* ************************************************************************** */
/*                               TESTS                                        */
/* ************************************************************************** */

int test_game_save()
{
  game g1 = game_default();
  game_save(g1, "game_default.txt");
  game g1_save = game_load("game_default.txt");
  if (!game_equal(g1, g1_save) || game_nb_cols(g1) != game_nb_cols(g1_save) ||
      game_nb_rows(g1) != game_nb_rows(g1_save) || game_is_wrapping(g1) != game_is_wrapping(g1_save)) {
    game_delete(g1);
    game_delete(g1_save);
    return EXIT_FAILURE;
  }
  game g2 = game_new_empty_ext(3, 10, true);
  game_save(g2, "game_wrapping_3x5.txt");
  game g2_save = game_load("game_wrapping_3x5.txt");
  if (!game_equal(g2, g2_save) || game_nb_cols(g2) != game_nb_cols(g2_save) ||
      game_nb_rows(g2) != game_nb_rows(g2_save) || game_is_wrapping(g2) != game_is_wrapping(g2_save)) {
    game_delete(g2);
    game_delete(g2_save);
    return EXIT_FAILURE;
  }
  game_delete(g1);
  game_delete(g1_save);
  game_delete(g2);
  game_delete(g2_save);
  return EXIT_SUCCESS;
}

/* ************************************************************************** */

int test_game_load()
{
  game g1 = game_default();
  game_save(g1, "game_default.txt");
  game g1_load = game_load("game_default.txt");
  if (!game_equal(g1, g1_load) || game_nb_cols(g1) != game_nb_cols(g1_load) ||
      game_nb_rows(g1) != game_nb_rows(g1_load) || game_is_wrapping(g1) != game_is_wrapping(g1_load)) {
    game_delete(g1);
    game_delete(g1_load);
    return EXIT_FAILURE;
  }
  game g2 = game_new_empty_ext(3, 4, true);
  game_save(g2, "game_wrapping_3x4");
  game g2_load = game_load("game_wrapping_3x4");
  if (!game_equal(g2, g2_load) || game_nb_cols(g2) != game_nb_cols(g2_load) ||
      game_nb_rows(g2) != game_nb_rows(g2_load) || game_is_wrapping(g2) != game_is_wrapping(g2_load)) {
    game_delete(g2);
    game_delete(g2_load);
    return EXIT_FAILURE;
  }
  game g3 = game_default_solution();
  game_save(g3, "game_default_solution.txt");
  game g3_load = game_load("game_default_solution.txt");
  if (!game_equal(g3, g3_load) || game_nb_cols(g3) != game_nb_cols(g3_load) ||
      game_nb_rows(g3) != game_nb_rows(g3_load) || game_is_wrapping(g3) != game_is_wrapping(g3_load)) {
    game_delete(g3);
    game_delete(g3_load);
    return EXIT_FAILURE;
  }
  game_delete(g1);
  game_delete(g1_load);
  game_delete(g2);
  game_delete(g2_load);
  game_delete(g3);
  game_delete(g3_load);
  return EXIT_SUCCESS;
}

/* ************************************************************************** */

int test_game_nb_solutions()
{
  game g1 = game_new_ext(7, 7, default_squares, 0);
  game g2 = game_new_ext(4, 4, ext_4x4_squares, 0);
  game g3 = game_new_ext(3, 10, ext_3x10_squares, 0);
  game g4 = game_new_ext(5, 1, ext_5x1_squares, 0);
  game g5 = game_new_ext(5, 3, ext_5x3w_squares, 0);
  game g6 = game_new_ext(3, 3, ext_3x3w_squares, 0);
  game g7 = game_new_ext(2, 2, ext_2x2w_squares, 0);

  game g1w = game_new_ext(7, 7, default_squares, 1);
  game g2w = game_new_ext(4, 4, ext_4x4_squares, 1);
  game g3w = game_new_ext(3, 10, ext_3x10_squares, 1);
  game g4w = game_new_ext(5, 1, ext_5x1_squares, 1);
  game g5w = game_new_ext(5, 3, ext_5x3w_squares, 1);
  game g6w = game_new_ext(3, 3, ext_3x3w_squares, 1);
  game g7w = game_new_ext(2, 2, ext_2x2w_squares, 1);

  if (game_nb_solutions(g1) != 1 || game_nb_solutions(g1w) != 0 || game_nb_solutions(g2) != 1 ||
      game_nb_solutions(g2w) != 0 || game_nb_solutions(g3) != 1 || game_nb_solutions(g3w) != 4 ||
      game_nb_solutions(g4) != 1 || game_nb_solutions(g4w) != 2 || game_nb_solutions(g5) != 3 ||
      game_nb_solutions(g5w) != 5 || game_nb_solutions(g6) != 0 || game_nb_solutions(g6w) != 1 ||
      game_nb_solutions(g7) != 0 || game_nb_solutions(g7w) != 1) {
    game_delete(g1);
    game_delete(g2);
    game_delete(g3);
    game_delete(g4);
    game_delete(g5);
    game_delete(g6);
    game_delete(g7);
    game_delete(g1w);
    game_delete(g2w);
    game_delete(g3w);
    game_delete(g4w);
    game_delete(g5w);
    game_delete(g6w);
    game_delete(g7w);
    return EXIT_FAILURE;
  }
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  game_delete(g4);
  game_delete(g5);
  game_delete(g6);
  game_delete(g7);
  game_delete(g1w);
  game_delete(g2w);
  game_delete(g3w);
  game_delete(g4w);
  game_delete(g5w);
  game_delete(g6w);
  game_delete(g7w);

  return EXIT_SUCCESS;
}

/* ************************************************************************** */

int test_game_solution()
{
  game g1 = game_new_ext(7, 7, default_squares, 0);
  game g2 = game_new_ext(4, 4, ext_4x4_squares, 0);
  game g3 = game_new_ext(3, 10, ext_3x10_squares, 0);
  game g4 = game_new_ext(5, 1, ext_5x1_squares, 0);
  game g5 = game_new_ext(5, 3, ext_5x3w_squares, 0);
  game g6 = game_new_ext(3, 3, ext_3x3w_squares, 0);
  game g7 = game_new_ext(2, 2, ext_2x2w_squares, 0);

  game g1w = game_new_ext(7, 7, default_squares, 1);
  game g2w = game_new_ext(4, 4, ext_4x4_squares, 1);
  game g3w = game_new_ext(3, 10, ext_3x10_squares, 1);
  game g4w = game_new_ext(5, 1, ext_5x1_squares, 1);
  game g5w = game_new_ext(5, 3, ext_5x3w_squares, 1);
  game g6w = game_new_ext(3, 3, ext_3x3w_squares, 1);
  game g7w = game_new_ext(2, 2, ext_2x2w_squares, 1);

  if (game_solve(g1) != true || game_solve(g2) != true || game_solve(g3) == false || game_solve(g4) != true ||
      game_solve(g5) != true || game_solve(g6) == true || game_solve(g7) == true || game_solve(g1w) == true ||
      game_solve(g2w) == true || game_solve(g3w) != true || game_solve(g4w) != true || game_solve(g5w) != true ||
      game_solve(g6w) != true || game_solve(g7w) != true) {
    game_delete(g1);
    game_delete(g2);
    game_delete(g3);
    game_delete(g4);
    game_delete(g5);
    game_delete(g6);
    game_delete(g7);
    game_delete(g1w);
    game_delete(g2w);
    game_delete(g3w);
    game_delete(g4w);
    game_delete(g5w);
    game_delete(g6w);
    game_delete(g7w);
    return EXIT_FAILURE;
  }
  if (!game_is_over(g1) || !game_is_over(g2) || !game_is_over(g3) || !game_is_over(g4) || !game_is_over(g5) ||
      game_is_over(g6) || game_is_over(g7) || game_is_over(g1w) || game_is_over(g2w) || !game_is_over(g3w) ||
      !game_is_over(g4w) || !game_is_over(g5w) || !game_is_over(g6w) || !game_is_over(g7w)) {
    game_delete(g1);
    game_delete(g2);
    game_delete(g3);
    game_delete(g4);
    game_delete(g5);
    game_delete(g6);
    game_delete(g7);
    game_delete(g1w);
    game_delete(g2w);
    game_delete(g3w);
    game_delete(g4w);
    game_delete(g5w);
    game_delete(g6w);
    game_delete(g7w);
    return EXIT_FAILURE;
  }
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  game_delete(g4);
  game_delete(g5);
  game_delete(g6);
  game_delete(g7);
  game_delete(g1w);
  game_delete(g2w);
  game_delete(g3w);
  game_delete(g4w);
  game_delete(g5w);
  game_delete(g6w);
  game_delete(g7w);
  return EXIT_SUCCESS;
}