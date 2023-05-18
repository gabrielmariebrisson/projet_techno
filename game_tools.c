#include "game_tools.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_opti.h"
#include "game_private.h"
#include "queue.h"

typedef struct black_wall_places {
  uint occ;
  uint number;
  uint* t;
} places_w;

static int i_offset[] = {[HERE] = 0,     [UP] = -1,       [DOWN] = +1,     [LEFT] = 0,      [RIGHT] = 0,
                         [UP_LEFT] = -1, [UP_RIGHT] = -1, [DOWN_LEFT] = 1, [DOWN_RIGHT] = 1};
static int j_offset[] = {[HERE] = 0,     [UP] = 0,       [DOWN] = 0,       [LEFT] = -1,     [RIGHT] = 1,
                         [UP_LEFT] = -1, [UP_RIGHT] = 1, [DOWN_LEFT] = -1, [DOWN_RIGHT] = 1};

/* ************************************************************************** */

void check_error(bool okay, char* message_error)
{
  if (!okay) {
    perror(message_error);
    exit(EXIT_FAILURE);
  }
}

square txt_to_square(char s)
{
  switch (s) {
    case 'b':
      return S_BLANK;
    case '*':
      return S_LIGHTBULB;
    case '-':
      return S_MARK;
    case 'w':
      return S_BLACKU;
    case '0':
      return S_BLACK0;
    case '1':
      return S_BLACK1;
    case '2':
      return S_BLACK2;
    case '3':
      return S_BLACK3;
    case '4':
      return S_BLACK4;
    default:
      return F_ERROR;
  }
  return F_ERROR;
}

game game_load(char* filename)
{
  // opening the file
  check_error(filename != NULL, filename);
  FILE* f = fopen(filename, "r");
  check_error(f != NULL, "file name error");

  // data acquisition
  char c_g_rows[10];
  char c_g_cols[10];
  char c_wrapping[5];
  int fss = fscanf(f, "%s %s %s", c_g_rows, c_g_cols, c_wrapping);
  fss++;
  // conversion
  uint g_rows = atoi(c_g_rows);
  uint g_cols = atoi(c_g_cols);
  uint wrapping = atoi(c_wrapping);
  char s[g_cols + 2];  // state table

  // game creation
  game g = game_new_empty_ext(g_rows, g_cols, wrapping);
  for (uint i = 0; i < g_rows; i++) {
    int fd = fscanf(f, "%s", s);
    fd++;
    for (uint j = 0; j < g_cols; j++) {
      game_set_square(g, i, j, txt_to_square(s[j]));
    }
  }
  game_update_flags(g);

  fclose(f);

  return g;
}

void print_val_square(FILE* f, cgame g, uint i, uint j)
{
  if (game_is_blank(g, i, j)) {
    fprintf(f, "b");
  }
  if (game_is_lightbulb(g, i, j)) {
    fprintf(f, "*");
  }
  if (game_is_black(g, i, j)) {
    if (game_get_black_number(g, i, j) == -1) {
      fprintf(f, "w");
    } else {
      fprintf(f, "%d", game_get_black_number(g, i, j));
    }
  }
  if (game_is_marked(g, i, j)) {
    fprintf(f, "-");
  }
}

void game_save(cgame g, char* filename)
{
  // on ouvre le fichier
  check_error(filename != NULL, filename);
  FILE* f = fopen(filename, "w");
  check_error(f != NULL, "file name error");

  uint g_rows = game_nb_rows(g);  // g_rows prends la valeur du nombre de lignes
  if (g_rows == 10) {
    fprintf(f, "10 ");
  } else {
    fprintf(f, "%d ", g_rows);  // on écrit cette valeur dans le fichier
  }

  uint g_cols = game_nb_cols(g);  // g_cols prends la valeur du nombre de colonnes
  if (g_cols == 10) {
    fprintf(f, "10 ");
  } else {
    fprintf(f, "%d ", g_cols);  // on écrit cette valeur dans le fichier
  }

  bool g_wrapping =
      game_is_wrapping(g);  // g_wrapping prends la valeur 0 si c'est le jeu n'est pas un wrapping et 1 si s'en est un
  fprintf(f, "%d\n", g_wrapping);  // on écrit cette valeur dans le fichier

  for (uint i = 0; i < g_rows; i++) {
    for (uint j = 0; j < g_cols; j++) {
      print_val_square(f, g, i, j);
    }
    fprintf(f, "\n");
  }

  fclose(f);
}
bool aux1(game g, int il, bool* done)
{
  if (game_is_over(g)) {
    *done = true;
    return true;
  }
  for (uint i = il; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      if (game_is_blank(g, i, j) && !game_is_lighted(g, i, j) && !game_is_black(g, i, j)) {
        game_play_move(g, i, j, S_LIGHTBULB);
        aux1(g, i, done);
        if (*done) {
          return true;
        }
        game_undo(g);
      }
    }
  }
  return true;
}

bool game_solve(game g)
{
  bool done = false;
  aux1(g, 0, &done);
  return done;
}

void transfer(square* dela, square* ala, uint n)
{
  for (uint i = 0; i < n; i++) {
    ala[i] = dela[i];
  }
}

bool aux2(game g, int il, int jl, int* cpt, square** temp, int tab)
{
  if (game_is_over(g)) {
    *cpt = *cpt + 1;
    return true;
  }
  for (uint c = jl; c < g->nb_cols; c++) {
    if (game_is_blank(g, il, c) && !game_is_lighted(g, il, c)) {
      transfer(g->squares, temp[tab], g->nb_cols * g->nb_rows);
      game_play_move_opti(g, il, c, S_LIGHTBULB);
      aux2(g, il, c, cpt, temp, tab + 1);
      transfer(temp[tab], g->squares, g->nb_cols * g->nb_rows);
    }
  }
  for (uint i = il + 1; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      if (game_is_blank(g, i, j) && !game_is_lighted(g, i, j)) {
        transfer(g->squares, temp[tab], g->nb_cols * g->nb_rows);
        game_play_move_opti(g, i, j, S_LIGHTBULB);
        aux2(g, i, j, cpt, temp, tab + 1);
        transfer(temp[tab], g->squares, g->nb_cols * g->nb_rows);
      }
    }
  }
  return true;
}

int _j_(cgame g, int j)
{
  if (game_is_wrapping(g)) {
    j = (j + game_nb_cols(g)) % game_nb_cols(g);
  }
  return j;
}

int _i_(cgame g, int i)
{
  if (game_is_wrapping(g)) {
    i = (i + game_nb_rows(g)) % game_nb_rows(g);
  }
  return i;
}

int _i_neigh(cgame g, int i, int j, direction dir)
{
  j = j;
  return _i_(g, i + i_offset[dir]);
}

int _j_neigh(cgame g, int i, int j, direction dir)
{
  i = i;
  return _j_(g, j + j_offset[dir]);
}

places_w* make_places(game g, uint i, uint j)
{
  places_w* places = malloc(sizeof(places_w));
  places->occ = 0;
  places->number = _neigh_count(g, i, j, S_BLANK, A_MASK, false);

  places->t = malloc(places->number * 2 * sizeof(uint));
  if (_inside_neigh(g, i, j, UP)) {
    if (_test_neigh(g, i, j, S_BLANK, A_MASK, UP)) {
      places->t[places->occ] = _i_neigh(g, i, j, UP);
      places->t[places->occ + 1] = _j_neigh(g, i, j, UP);
      places->occ = places->occ + 2;
    }
  }
  if (_inside_neigh(g, i, j, DOWN)) {
    if (_test_neigh(g, i, j, S_BLANK, A_MASK, DOWN)) {
      places->t[places->occ] = _i_neigh(g, i, j, DOWN);
      places->t[places->occ + 1] = _j_neigh(g, i, j, DOWN);
      places->occ = places->occ + 2;
    }
  }
  if (_inside_neigh(g, i, j, LEFT)) {
    if (_test_neigh(g, i, j, S_BLANK, A_MASK, LEFT)) {
      places->t[places->occ] = _i_neigh(g, i, j, LEFT);
      places->t[places->occ + 1] = _j_neigh(g, i, j, LEFT);
      places->occ = places->occ + 2;
    }
  }
  if (_inside_neigh(g, i, j, RIGHT)) {
    if (_test_neigh(g, i, j, S_BLANK, A_MASK, RIGHT)) {
      places->t[places->occ] = _i_neigh(g, i, j, RIGHT);
      places->t[places->occ + 1] = _j_neigh(g, i, j, RIGHT);
      places->occ = places->occ + 2;
    }
  }
  return places;
}

void free_places(places_w* p)
{
  free(p->t);
  free(p);
}

void play_everywhere(game g, uint i, uint j, square s)
{
  places_w* p = make_places(g, i, j);
  for (uint i = 0; i < p->occ; i = i + 2) {
    game_set_square(g, p->t[i], p->t[i + 1], s);
  }
  free_places(p);
}

void make_black_possibilites(game g, square** temp, int* cpt)
{  // traiter le cas du nblack == -1

  for (uint i = 0; i < g->nb_rows; i++) {
    for (uint j = 0; j < g->nb_cols; j++) {
      if (game_is_black(g, i, j)) {
        if (game_get_black_number(g, i, j) == 0) {
          play_everywhere(g, i, j, S_BLANK + F_LIGHTED);
        }
        if (game_get_black_number(g, i, j) == 4) {
          play_everywhere(g, i, j, S_LIGHTBULB);
        }
        if (game_get_black_number(g, i, j) == 1 && _neigh_count(g, i, j, S_BLANK, A_MASK, false) == 1) {
          play_everywhere(g, i, j, S_LIGHTBULB);
        }
        if (game_get_black_number(g, i, j) == 2 && _neigh_count(g, i, j, S_BLANK, A_MASK, false) == 2) {
          play_everywhere(g, i, j, S_LIGHTBULB);
        }
        if (game_get_black_number(g, i, j) == 3 && _neigh_count(g, i, j, S_BLANK, A_MASK, false) == 3) {
          play_everywhere(g, i, j, S_LIGHTBULB);
        }
      }
    }
  }
  game_update_flags_opti(g);
  aux2(g, 0, 0, cpt, temp, 0);
}

uint game_nb_solutions(cgame g)
{
  int counter = 0;
  game g1 = game_copy(g);
  int s = g->nb_cols * g->nb_rows;
  square** temp;
  temp = malloc(s * sizeof(square*));
  if (temp == NULL) {
    return EXIT_FAILURE;
  }
  for (int i = 0; i < s; i++) {
    temp[i] = malloc(s * sizeof(square));
    if (temp[i] == NULL) {
      return EXIT_FAILURE;
    }
  }
  make_black_possibilites(g1, temp, &counter);
  game_delete(g1);
  for (int i = 0; i < s; i++) {
    free(temp[i]);
  }
  free(temp);
  return counter;
}

/* ************************************************************************** */

static uint nb_neigh_lightbulbs(cgame g, uint i, uint j)
{
  assert(g);
  int nb_rows = game_nb_rows(g);
  int nb_cols = game_nb_cols(g);
  assert(i < nb_rows);
  assert(j < nb_cols);

  int i_up = i - 1;
  int i_down = i + 1;
  int j_left = j - 1;
  int j_right = j + 1;
  if (game_is_wrapping(g)) {
    i_up = (i_up + nb_rows) % nb_rows;
    i_down = (i_down + nb_rows) % nb_rows;
    j_left = (j_left + nb_cols) % nb_cols;
    j_right = (j_right + nb_cols) % nb_cols;
  }

  uint count = 0;
  if ((i_up >= 0) && game_is_lightbulb(g, i_up, j)) count++;
  if ((i_down < nb_rows) && game_is_lightbulb(g, i_down, j)) count++;
  if ((j_left >= 0) && game_is_lightbulb(g, i, j_left)) count++;
  if ((j_right < nb_cols) && game_is_lightbulb(g, i, j_right)) count++;

  return count;
}

/* ************************************************************************** */

static uint nb_unlit_squares(cgame g)
{
  int nb = 0;
  for (uint i = 0; i < game_nb_rows(g); i++)
    for (uint j = 0; j < game_nb_cols(g); j++)
      if (!game_is_lighted(g, i, j) && game_is_blank(g, i, j)) nb++;
  return nb;
}

/* ************************************************************************** */

/**
 * Create a random game with a given size and number of walls
 *
 * @param nb_rows the number of rows of the game
 * @param nb_cols the number of columns of the game
 * @param wrapping wrapping option
 * @param nb_walls the number of walls to add
 * @param with_solution if true, the game contains the solution, otherwise only walls
 *
 * @return the generated random game
 */

game game_random(uint nb_rows, uint nb_cols, bool wrapping, uint nb_walls, bool with_solution)
{
  assert(nb_walls <= nb_rows * nb_cols);

  // step 0: create an empty game
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping);

  // step 1: add random black walls
  uint k = 0;
  while (k < nb_walls) {
    uint i = rand() % nb_rows;
    uint j = rand() % nb_cols;
    if (!game_is_black(g, i, j)) {
      game_set_square(g, i, j, S_BLACKU);
      k++;
    }
  }
  game_update_flags(g);

  // step 2: add lightbulbs until every squares are lighted
  uint nb_unlit = nb_unlit_squares(g);
  while (nb_unlit != 0) {
    uint random_unlit_num = rand() % nb_unlit;
    uint num = 0;
    for (uint i = 0; i < game_nb_rows(g); i++)
      for (uint j = 0; j < game_nb_cols(g); j++) {
        if (!game_is_lighted(g, i, j) && game_is_blank(g, i, j)) {
          if (num == random_unlit_num) {
            game_set_square(g, i, j, S_LIGHTBULB);
            game_update_flags(g);
          }
          num++;
        }
      }
    nb_unlit = nb_unlit_squares(g);
  }

  // step 3 : set some black wall numbers
  for (uint i = 0; i < game_nb_rows(g); i++)
    for (uint j = 0; j < game_nb_cols(g); j++) {
      if (game_is_black(g, i, j)) {
        if (rand() % 2 == 0) {
          int nb_lightbulbs = nb_neigh_lightbulbs(g, i, j);
          game_set_square(g, i, j, S_BLACK + nb_lightbulbs);
        }
      }
    }

  // check
  assert(game_is_over(g));

  if (!with_solution) game_restart(g);
  return g;
}

/* ************************************************************************** */
