/**
 * @file game_text.c
 * @brief Game Interface in Text Mode.
 * @copyright University of Bordeaux. All rights reserved, 2021.
 **/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

/* ************************************************************************** */

static void game_print_errors(game g)
{
  for (uint i = 0; i < game_nb_rows(g); i++) {
    for (uint j = 0; j < game_nb_cols(g); j++) {
      if (game_has_error(g, i, j)) {
        if (game_is_lightbulb(g, i, j)) printf("Error at light bulb (%d,%d)\n", i, j);
        if (game_is_black(g, i, j)) printf("Error at black wall (%d,%d)\n", i, j);
      }
    }
  }
}

/* ************************************************************************** */

static bool game_step(game g)
{
  printf("> ? [h for help]\n");
  // <action> [<row> <col>]
  char c = '?';
  int r = scanf(" %c", &c);  // don't forget the space ' ' before %c
  if (r == EOF || r < 0) {
    return false;
  }
  if (r != 1) {
    printf("Error: invalid user input!\n");
    return true;  // but continue to play
  }

  if (c == 'h') {  // help
    printf("> action: help\n");
    printf("- press 'l <i> <j>' to put a light bulb at square (i,j)\n");
    printf("- press 'm <i> <j>' to put a mark at square (i,j)\n");
    printf("- press 'b <i> <j>' to blank square (i,j)\n");
    printf("- press 'r' to restart\n");
    printf("- press 'z' to undo\n");
    printf("- press 'y' to redo\n");
    printf("- press 'q' to quit\n");
    printf("- press 'w <name file>' to save\n");
    // printf("- press 's' to solve the default game\n");
  } else if (c == 'z') {  // undo
    printf("> action: undo\n");
    game_undo(g);
    return true;
  } else if (c == 'y') {  // redo
    printf("> action: redo\n");
    game_redo(g);
    return true;
  } else if (c == 'r') {  // restart
    printf("> action: restart\n");
    game_restart(g);
    return true;
  } else if (c == 'w') {  // save
    printf("> action: save the game\n");
    char filename[10];
    int gh = scanf("%s", filename);
    gh++;
    game_save(g, filename);
    return true;
  } else if (c == 'q') {  // quit
    printf("> action: quit\n");
    return false;                                 // exit
  } else if (c == 'l' || c == 'm' || c == 'b') {  // play move
    uint i, j;
    int ret = scanf(" %u %u", &i, &j);
    if (ret != 2) {
      printf("Error: invalid user input!\n");
      return true;
    }
    printf("> action: play move '%c' into square (%d,%d)\n", c, i, j);
    square s;
    if (c == 'l') s = S_LIGHTBULB;
    if (c == 'b') s = S_BLANK;
    if (c == 'm') s = S_MARK;
    bool check = game_check_move(g, i, j, s);
    if (!check) {
      printf("Error: illegal move on square (%d,%d)!\n", i, j);
      return true;
    }
    game_play_move(g, i, j, s);
    return true;  // continue to play
  } else {
    printf("Error: invalid user input!\n");
    return true;
  }

  return true;  // continue...
}

/* ************************************************************************** */

int main(int argc, char* argv[])
{
  game g = NULL;
  if (argc == 2)
    g = game_load(argv[1]);

  else {
    g = game_default();
    assert(g);
  }

  game_print(g);
  bool win = game_is_over(g);
  bool cont = true;
  while (!win && cont) {
    cont = game_step(g);
    win = game_is_over(g);
    if (cont) game_print(g);
    game_print_errors(g);
  }
  if (win)
    printf("Congratulation, you win :-)\n");
  else
    printf("What a shame, you gave up :-(\n");
  game_delete(g);

  return EXIT_SUCCESS;
}
