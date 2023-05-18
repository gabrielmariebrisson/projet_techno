/**
 * @file game_test.c
 * @brief Game Tests.
 * @copyright University of Bordeaux. All rights reserved, 2021.
 *
 **/

#include "game.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_ext.h"
#include "game_test.h"

/* ************************************************************************** */
/*                               CHECK ROUTINES                               */
/* ************************************************************************** */

/* if squares == NULL, test if game is empty */
bool check_game_ext(cgame g, uint nb_rows, uint nb_cols, square* squares, bool wrapping)
{
  if (!g) return false;

  // nb rows & cols
  if (game_nb_rows(g) != nb_rows) return false;
  if (game_nb_cols(g) != nb_cols) return false;

  // options
  if (game_is_wrapping(g) != wrapping) return false;

  // check squares
  for (uint i = 0; i < nb_rows; i++)
    for (uint j = 0; j < nb_cols; j++) {
      square s = game_get_square(g, i, j);
      square ss = squares ? squares[i * nb_cols + j] : S_BLANK;
      if (s != ss) return false;
    }

  return true;
}

/* ************************************************************************** */

bool check_game(game g, square* squares) { return check_game_ext(g, DEFAULT_SIZE, DEFAULT_SIZE, squares, false); }

/* ************************************************************************** */
/*                                MAIN ROUTINE                                */
/* ************************************************************************** */

static void usage(int argc, char* argv[])
{
  assert(argc > 0);
  fprintf(stderr, "Usage: %s <testname>\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* ************************************************************************** */

struct test {
  char* name;
  int (*func)(void);
};

/* ************************************************************************** */
struct test tests[] = {  // dummy test

    // basic tests (game v1)
    {"new", test_new},
    {"new_empty", test_new_empty},
    {"copy", test_copy},
    {"equal", test_equal},
    {"delete", test_delete},
    {"set_square", test_set_square},
    {"get_square", test_get_square},
    {"get_state", test_get_state},
    {"get_flags", test_get_flags},
    {"is_state", test_is_state},
    {"has_flag", test_has_flag},
    {"play_move", test_play_move},
    {"check_move", test_check_move},
    {"update_flags", test_update_flags},
    {"is_over", test_is_over},
    {"restart", test_restart},

    // aux tests
    {"default", test_default},
    {"default_solution", test_default_solution},
    {"print", test_print},

    // ext tests (game v2)
    /* rectangular board */
    {"new_ext", test_new_ext},
    {"new_empty_ext", test_new_empty_ext},
    {"equal_ext", test_equal_ext},
    {"copy_ext", test_copy_ext},
    {"game_1d", test_game_1d},

    /* wrapping option */
    {"game_wrapping_2x2", test_game_wrapping_2x2},
    {"game_wrapping_3x3", test_game_wrapping_3x3},
    {"game_wrapping_5x3", test_game_wrapping_5x3},
    {"game_wrapping_error", test_game_wrapping_error},

    /* undo & redo */
    {"undo_one", test_undo_one},
    {"undo_redo_some", test_undo_redo_some},
    {"undo_redo_all", test_undo_redo_all},
    {"restart_undo", test_restart_undo},

    /* save & load */
    {"save", test_game_save},
    {"load", test_game_load},
    {"nb_solutions", test_game_nb_solutions},
    {"solve", test_game_solution},
    // end
    {NULL, NULL}};

/* ************************************************************************** */

int main(int argc, char* argv[])
{
  // run test
  if (argc == 1) usage(argc, argv);
  int ret = -1;
  for (struct test* t = tests; t->name && t->func; t++) {
    if (strcmp(argv[1], t->name) == 0) {
      ret = t->func();
      break;
    }
  }

  // print test result
  if (ret == -1) {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    return EXIT_FAILURE;
  } else if (ret == 0) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/* ************************************************************************** */
