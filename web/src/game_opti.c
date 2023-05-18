/**
 * @file game_private.c
 * @copyright University of Bordeaux. All rights reserved, 2021.
 **/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_tools.h"
#include "queue.h"

/* ************************************************************************** */
/*                               UPDATE FLAGS                                 */
/* ************************************************************************** */

static void _update_lighted_flags_wrapping(game g, uint i, uint j)
{
  int i1 = i, j1 = j;
  game_set_square(g, i1, j1, (F_LIGHTED + S_LIGHTBULB));  //
  bool haut = false, bas = false, gauche = false, droite = false;
  for (uint i2 = i1 + 1; i2 < g->nb_rows; i2++) {
    if (game_is_black(g, i2, j1)) {
      bas = true;
      break;
    } else if (game_is_lightbulb(g, i2, j1)) {
      bas = true;
      game_set_square(g, i1, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      game_set_square(g, i2, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      break;
    } else if (game_is_blank(g, i2, j1)) {
      game_set_square(g, i2, j1, (S_BLANK + F_LIGHTED));
    } else if (game_is_marked(g, i2, j1)) {
      game_set_square(g, i2, j1, (S_MARK + F_LIGHTED));
    }
  }
  if (!bas) {
    for (int i2 = 0; i2 < i1; i2++) {
      if (game_is_black(g, i2, j1)) {
        break;
      } else if (game_is_lightbulb(g, i2, j1)) {
        game_set_square(g, i1, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
        game_set_square(g, i2, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
        break;
      } else if (game_is_blank(g, i2, j1)) {
        game_set_square(g, i2, j1, (S_BLANK + F_LIGHTED));
      } else if (game_is_marked(g, i2, j1)) {
        game_set_square(g, i2, j1, (S_MARK + F_LIGHTED));
      }
    }
  }

  for (int i2 = i1 - 1; i2 >= 0; i2--) {
    if (game_is_black(g, i2, j1)) {
      haut = true;
      break;
    } else if (game_is_lightbulb(g, i2, j1)) {
      haut = true;
      game_set_square(g, i1, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      game_set_square(g, i2, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      break;
    } else if (game_is_blank(g, i2, j1)) {
      game_set_square(g, i2, j1, (S_BLANK + F_LIGHTED));
    } else if (game_is_marked(g, i2, j1)) {
      game_set_square(g, i2, j1, (S_MARK + F_LIGHTED));
    }
  }
  if (!haut) {
    for (int i2 = g->nb_rows - 1; i2 > i1; i2--) {
      if (game_is_black(g, i2, j1)) {
        break;
      } else if (game_is_lightbulb(g, i2, j1)) {
        game_set_square(g, i1, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
        game_set_square(g, i2, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
        break;
      } else if (game_is_blank(g, i2, j1)) {
        game_set_square(g, i2, j1, (S_BLANK + F_LIGHTED));
      } else if (game_is_marked(g, i2, j1)) {
        game_set_square(g, i2, j1, (S_MARK + F_LIGHTED));
      }
    }
  }

  for (uint j2 = j1 + 1; j2 < g->nb_cols; j2++) {
    if (game_is_black(g, i1, j2)) {
      droite = true;
      break;
    } else if (game_is_lightbulb(g, i1, j2)) {
      game_set_square(g, i1, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      game_set_square(g, i1, j2, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      droite = true;
      break;
    } else if (game_is_blank(g, i1, j2)) {
      game_set_square(g, i1, j2, (S_BLANK + F_LIGHTED));
    } else if (game_is_marked(g, i1, j1)) {
      game_set_square(g, i1, j2, (S_MARK + F_LIGHTED));
    }
  }
  if (!droite) {
    for (int j2 = 0; j2 < j1; j2++) {
      if (game_is_black(g, i1, j2)) {
        break;
      } else if (game_is_lightbulb(g, i1, j2)) {
        game_set_square(g, i1, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
        game_set_square(g, i1, j2, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
        break;
      } else if (game_is_blank(g, i1, j2)) {
        game_set_square(g, i1, j2, (S_BLANK + F_LIGHTED));
      } else if (game_is_marked(g, i1, j1)) {
        game_set_square(g, i1, j2, (S_MARK + F_LIGHTED));
      }
    }
  }

  for (int j2 = j1 - 1; j2 >= 0; j2--) {
    if (game_is_black(g, i1, j2)) {
      gauche = true;
      break;
    } else if (game_is_lightbulb(g, i1, j2)) {
      gauche = true;
      game_set_square(g, i1, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      game_set_square(g, i1, j2, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      break;
    } else if (game_is_blank(g, i1, j2)) {
      game_set_square(g, i1, j2, (S_BLANK + F_LIGHTED));
    } else if (game_is_marked(g, i1, j1)) {
      game_set_square(g, i1, j2, (S_MARK + F_LIGHTED));
    }
  }
  if (!gauche) {
    for (int j2 = g->nb_cols - 1; j2 > j1; j2--) {
      if (game_is_black(g, i1, j2)) {
        break;
      } else if (game_is_lightbulb(g, i1, j2)) {
        game_set_square(g, i1, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
        game_set_square(g, i1, j2, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
        break;
      } else if (game_is_blank(g, i1, j2)) {
        game_set_square(g, i1, j2, (S_BLANK + F_LIGHTED));
      } else if (game_is_marked(g, i1, j1)) {
        game_set_square(g, i1, j2, (S_MARK + F_LIGHTED));
      }
    }
  }
}

/* ************************************************************************** */

static void _update_lighted_flags_(game g, uint i, uint j)
{
  uint i1 = i, j1 = j;
  game_set_square(g, i1, j1, (F_LIGHTED + S_LIGHTBULB));
  for (uint i2 = i1 + 1; i2 < g->nb_rows; i2++) {
    if (game_is_black(g, i2, j1)) {
      break;
    } else if (game_is_lightbulb(g, i2, j1)) {
      game_set_square(g, i1, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      game_set_square(g, i2, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      break;
    } else if (game_is_blank(g, i2, j1)) {
      game_set_square(g, i2, j1, (S_BLANK + F_LIGHTED));
    } else if (game_is_marked(g, i2, j1)) {
      game_set_square(g, i2, j1, (S_MARK + F_LIGHTED));
    }
  }
  for (int i2 = i1 - 1; i2 >= 0; i2--) {
    if (game_is_black(g, i2, j1)) {
      break;
    } else if (game_is_lightbulb(g, i2, j1)) {
      game_set_square(g, i1, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      game_set_square(g, i2, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      break;
    } else if (game_is_blank(g, i2, j1)) {
      game_set_square(g, i2, j1, (S_BLANK + F_LIGHTED));
    } else if (game_is_marked(g, i2, j1)) {
      game_set_square(g, i2, j1, (S_MARK + F_LIGHTED));
    }
  }
  for (uint j2 = j1 + 1; j2 < g->nb_cols; j2++) {
    if (game_is_black(g, i1, j2)) {
      break;
    } else if (game_is_lightbulb(g, i1, j2)) {
      game_set_square(g, i1, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      game_set_square(g, i1, j2, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      break;
    } else if (game_is_blank(g, i1, j2)) {
      game_set_square(g, i1, j2, (S_BLANK + F_LIGHTED));
    } else if (game_is_marked(g, i1, j1)) {
      game_set_square(g, i1, j2, (S_MARK + F_LIGHTED));
    }
  }
  for (int j2 = j1 - 1; j2 >= 0; j2--) {
    if (game_is_black(g, i1, j2)) {
      break;
    } else if (game_is_lightbulb(g, i1, j2)) {
      game_set_square(g, i1, j1, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      game_set_square(g, i1, j2, (S_LIGHTBULB + F_ERROR + F_LIGHTED));
      break;
    } else if (game_is_blank(g, i1, j2)) {
      game_set_square(g, i1, j2, (S_BLANK + F_LIGHTED));
    } else if (game_is_marked(g, i1, j1)) {
      game_set_square(g, i1, j2, (S_MARK + F_LIGHTED));
    }
  }
}

/* ************************************************************************** */

void game_update_flags_opti(game g)
{
  // 0) reset all flags
  for (uint i = 0; i < g->nb_rows; i++)
    for (uint j = 0; j < g->nb_cols; j++) SQUARE(g, i, j) = STATE(g, i, j);

  // 1) update lighted flag
  if (g->wrapping) {
    for (uint i = 0; i < g->nb_rows; i++) {
      for (uint j = 0; j < g->nb_cols; j++) {
        if (STATE(g, i, j) == S_LIGHTBULB) _update_lighted_flags_wrapping(g, i, j);
      }
    }
    for (uint i1 = 0; i1 < g->nb_rows; i1++) {
      for (uint j1 = 0; j1 < g->nb_cols; j1++) {  // Parcours du tableau
        if (game_is_black(g, i1, j1) && game_get_black_number(g, i1, j1) != -1) {
          // Si c'est un carré noir
          int countB = 0;  // Cpt du nombre de lightbulbs
          int countL = 0;  // Cpt du nombre de blank non lighted.
          /*
          On verifie autour du carré en comptant le nombre de lightbulbs et de blanks
          Si le carré est à son extrémité, on voit de l'autre coté.
          */

          if (i1 != 0) {  // UP
            if (game_is_lightbulb(g, i1 - 1, j1)) {
              countB++;
            }
            if (game_is_blank(g, i1 - 1, j1) && !game_is_lighted(g, i1 - 1, j1)) {
              countL++;
            }
          } else if (i1 == 0) {
            int x = (g->nb_rows) - 1;
            if (game_is_lightbulb(g, x, j1)) {
              countB++;
            }
            if (!game_is_lighted(g, x, j1) && game_is_blank(g, x, j1)) {
              countL++;
            }
          }

          if (i1 != g->nb_rows - 1) {  // DOWN
            if (game_is_lightbulb(g, i1 + 1, j1)) {
              countB++;
            }
            if (game_is_blank(g, i1 + 1, j1) && !game_is_lighted(g, i1 + 1, j1)) {
              countL++;
            }
          } else if (i1 == g->nb_rows - 1) {
            if (game_is_lightbulb(g, 0, j1)) {
              countB++;
            }
            if (game_is_blank(g, 0, j1) && !game_is_lighted(g, 0, j1)) {
              countL++;
            }
          }

          if (j1 != 0) {  // LEFT
            if (game_is_lightbulb(g, i1, j1 - 1)) {
              countB++;
            }
            if (game_is_blank(g, i1, j1 - 1) && !game_is_lighted(g, i1, j1 - 1)) {
              countL++;
            }
          } else if (j1 == 0) {
            int x = (g->nb_cols - 1);
            if (game_is_lightbulb(g, i1, x)) {
              countB++;
            }
            if (game_is_blank(g, i1, x) && !game_is_lighted(g, i1, x)) {
              countL++;
            }
          }

          if (j1 != g->nb_cols - 1) {  // RIGHT
            if (game_is_lightbulb(g, i1, j1 + 1)) {
              countB++;
            }
            if (game_is_blank(g, i1, j1 + 1) && !game_is_lighted(g, i1, j1 + 1)) {
              countL++;
            }
          } else if (j1 == g->nb_cols - 1) {
            if (game_is_lightbulb(g, i1, 0)) {
              countB++;
            }
            if (!game_is_lighted(g, i1, 0) && game_is_blank(g, i1, 0)) {
              countL++;
            }
          }
          int number = game_get_black_number(g, i1, j1);  // Le nombre du carré noir
          if ((countB > number) || ((countB + countL) < number)) {
            /*
            On place des flags d'erreurs si il y a trop de lampes ou pas assez de lampes
            autour d'un carré noir
            */
            square s = game_get_state(g, i1, j1);
            game_set_square(g, i1, j1, s + F_ERROR);  // Placement du flag erreur.
          }
        }
      }
    }

  } else {
    for (uint i = 0; i < g->nb_rows; i++) {
      for (uint j = 0; j < g->nb_cols; j++) {
        if (STATE(g, i, j) == S_LIGHTBULB) _update_lighted_flags_(g, i, j);
      }
    }

    for (uint i1 = 0; i1 < g->nb_rows; i1++) {
      for (uint j1 = 0; j1 < g->nb_cols; j1++) {
        if (game_is_black(g, i1, j1) && game_get_black_number(g, i1, j1) != -1) {
          // Si c'est un carré noir
          int countB = 0;  // Compte le nombre de lightbulbs
          int countL = 0;  // Compte le nombre de NON LIGHTED BLANKS
          // Et la on compte.
          if (i1 != 0) {  // UP
            if (game_is_lightbulb(g, i1 - 1, j1)) {
              countB++;
            }
            if (game_is_blank(g, i1 - 1, j1) && !game_is_lighted(g, i1 - 1, j1)) {
              countL++;
            }
          }

          if (i1 != g->nb_rows - 1) {  // DOWN
            if (game_is_lightbulb(g, i1 + 1, j1)) {
              countB++;
            }
            if (game_is_blank(g, i1 + 1, j1) && !game_is_lighted(g, i1 + 1, j1)) {
              countL++;
            }
          }

          if (j1 != 0) {  // LEFT
            if (game_is_lightbulb(g, i1, j1 - 1)) {
              countB++;
            }
            if (game_is_blank(g, i1, j1 - 1) && !game_is_lighted(g, i1, j1 - 1)) {
              countL++;
            }
          }

          if (j1 != g->nb_cols - 1) {  // RIGHT
            if (game_is_lightbulb(g, i1, j1 + 1)) {
              countB++;
            }
            if (game_is_blank(g, i1, j1 + 1) && !game_is_lighted(g, i1, j1 + 1)) {
              countL++;
            }
          }

          int number = game_get_black_number(g, i1, j1);  // Récupère le nombre du carré black
          if ((countB > number) || ((countB + countL) < number)) {
            /* On place des flags d'erreurs si il y a trop de lampes ou pas assez de lampes
            autour d'un carré noir
            */
            square s = game_get_state(g, i1, j1);
            game_set_square(g, i1, j1, s + F_ERROR);  // Erreur de Flag
          }
        }
      }
    }
  }
}

/* ************************************************************************** */

void game_play_move_opti(game g, uint i, uint j, square s)
{
  SQUARE(g, i, j) = s;
  game_update_flags_opti(g);
}