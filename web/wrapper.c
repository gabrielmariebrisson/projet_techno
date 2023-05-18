/**
 * @file wrapper.js
 * @brief Game Binding to JavaScript (based on Emscripten & Wasm)
 * @author aurelien.esnard@u-bordeaux.fr
 * @copyright University of Bordeaux. All rights reserved, 2022.
 **/

#include <emscripten.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

/* ******************** Game V1 & V2 API ******************** */

EMSCRIPTEN_KEEPALIVE
game new_default(void) { return game_default(); }

EMSCRIPTEN_KEEPALIVE
void delete (game g) { game_delete(g); }

EMSCRIPTEN_KEEPALIVE
square get_square(cgame g, uint i, uint j) { return game_get_square(g, i, j); }

EMSCRIPTEN_KEEPALIVE
void play_move(game g, uint i, uint j, square s) { game_play_move(g, i, j, s); }

EMSCRIPTEN_KEEPALIVE
bool check_move(cgame g, uint i, uint j, square s) { return game_check_move(g, i, j, s); }

EMSCRIPTEN_KEEPALIVE
void restart(game g) { game_restart(g); }

EMSCRIPTEN_KEEPALIVE
bool is_over(game g) { return game_is_over(g); }

EMSCRIPTEN_KEEPALIVE
uint nb_rows(cgame g) { return game_nb_rows(g); }

EMSCRIPTEN_KEEPALIVE
uint nb_cols(cgame g) { return game_nb_cols(g); }

EMSCRIPTEN_KEEPALIVE
square get_state(cgame g, uint i, uint j) { return game_get_state(g, i, j); }

EMSCRIPTEN_KEEPALIVE
square get_flags(cgame g, uint i, uint j) { return game_get_flags(g, i, j); }

EMSCRIPTEN_KEEPALIVE
bool is_blank(cgame g, uint i, uint j) { return game_is_blank(g, i, j); }

EMSCRIPTEN_KEEPALIVE
bool is_lightbulb(cgame g, uint i, uint j) { return game_is_lightbulb(g, i, j); }

EMSCRIPTEN_KEEPALIVE
bool is_black(cgame g, uint i, uint j) { return game_is_black(g, i, j); }

EMSCRIPTEN_KEEPALIVE
int get_black_number(cgame g, uint i, uint j) { return game_get_black_number(g, i, j); }

EMSCRIPTEN_KEEPALIVE
bool is_marked(cgame g, uint i, uint j) { return game_is_marked(g, i, j); }

EMSCRIPTEN_KEEPALIVE
bool is_lighted(cgame g, uint i, uint j) { return game_is_lighted(g, i, j); }

EMSCRIPTEN_KEEPALIVE
bool has_error(cgame g, uint i, uint j) { return game_has_error(g, i, j); }

/* ******************** Game Tools API ******************** */

EMSCRIPTEN_KEEPALIVE
bool solve(game g) { return game_solve(g); }

EMSCRIPTEN_KEEPALIVE
uint nb_solutions(cgame g) { return game_nb_solutions(g); }

EMSCRIPTEN_KEEPALIVE
void undo(game g) { game_undo(g); }

EMSCRIPTEN_KEEPALIVE
void redo(game g) { game_redo(g); }

EMSCRIPTEN_KEEPALIVE
game new_random(uint nb_rows, uint nb_cols, bool wrapping, uint nb_walls, bool with_solution)
{
  srand(time(NULL)); // radom seed
  return game_random(nb_rows, nb_cols, wrapping, nb_walls, with_solution);
}

// EOF
