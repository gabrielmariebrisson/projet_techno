/**
 * @file game_test.h
 * @brief All Test Functions.
 * @copyright University of Bordeaux. All rights reserved, 2021.
 **/

#ifndef __GAME_TEST_H__
#define __GAME_TEST_H__

#include "game.h"
#include "game_ext.h"

/* ************************************************************************** */
/*                               CHECK ROUTINES                               */
/* ************************************************************************** */

bool check_game_ext(cgame g, uint nb_rows, uint nb_cols, square* squares, bool wrapping);
bool check_game(game g, square* squares);

/* ************************************************************************** */
/*                                AUX TESTS                                   */
/* ************************************************************************** */

int test_print(void);
int test_default(void);
int test_default_solution(void);

/* ************************************************************************** */
/*                            BASIC TESTS (V1)                                */
/* ************************************************************************** */

int test_new(void);
int test_new_empty(void);
int test_copy(void);
int test_equal(void);
int test_delete(void);
int test_set_square(void);
int test_get_square(void);
int test_get_state(void);
int test_get_flags(void);
int test_is_state(void);
int test_has_flag(void);
int test_play_move(void);
int test_check_move(void);
int test_is_over(void);
int test_restart(void);
int test_update_flags(void);

/* ************************************************************************** */
/*                              EXT TESTS (V2)                                */
/* ************************************************************************** */

int test_new_ext(void);
int test_new_empty_ext(void);
int test_equal_ext(void);
int test_copy_ext(void);
int test_game_1d(void);
int test_game_wrapping_2x2(void);
int test_game_wrapping_3x3(void);
int test_game_wrapping_5x3(void);
int test_game_wrapping_error(void);
int test_game_diag(void);
int test_undo_one(void);
int test_undo_redo_some(void);
int test_undo_redo_all(void);
int test_restart_undo(void);

/* ************************************************************************** */
/*                              TESTS IO (V2)                                 */
/* ************************************************************************** */

int test_game_save(void);
int test_game_load(void);
int test_game_nb_solutions(void);
int test_game_solution(void);
#endif  // __GAME_TEST_H__
