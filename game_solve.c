#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_tools.h"

int main(int argc, char* argv[])
{
  game g = game_load(argv[2]);
  if (argc < 3) {
    printf("Error: not enough parameters!\n");
  }
  if ((argv[1])[1] == 's' && game_solve(g)) {
    if (argv[3] != NULL) {
      game_save(g, argv[3]);
      return EXIT_SUCCESS;
    } else {
      game_print(g);
      return EXIT_SUCCESS;
    }
  }
  if ((argv[1])[1] == 'c') {
    if (argv[3] != NULL) {
      FILE* f = fopen(argv[3], "w");
      fprintf(f, "%d\n", game_nb_solutions(g));
      fclose(f);
      return EXIT_SUCCESS;
    } else {
      printf("%d \n", game_nb_solutions(g));
      return EXIT_SUCCESS;
    }
  }
  return EXIT_FAILURE;
}