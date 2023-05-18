// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include "model.h"

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_tools.h"
#include "queue.h"

/* **************************************************************** */

#define FONTSIZE 36
#define BACKGROUND "background.png"
#define FIRE_HYDRANT "bouche-incendie.png"
#define WATER "water.png"
#define MARKS "marks.png"
#define FLAME "flame.png"

#define ERREUR "error.png"
#define QUIT "Quit.png"
#define REDO "Redo.png"
#define UNDO "Undo.png"
#define RESTART "Restart.png"
#define SAVE "Save.png"
#define SOLVE "Solve.png"
#define WALL_U "wall_U.png"
#define WALL_1 "wall_1.png"
#define WALL_2 "wall_2.png"
#define WALL_3 "wall_3.png"
#define WALL_4 "wall_4.png"
#define WALL_0 "wall_0.png"
#define WIN "won.png"
/* **************************************************************** */

#define SIDE_MARGIN 0.08            // SPACE BETWEENT THE THE LEFT EDGE AND THE GRID
#define HEIGHT_MARGIN 0.25          // SPACE BETWEENT THE THE TOP EDGE AND THE GRID
#define PERC_OF_SQUARE 0.0667       // PERCENTAGE OF THE SQUARE IN THE WINDOW
#define DEFAULT_WINDOW 600.0        // DEFAULT WINDOW SIZE
#define HEIGHT_MARGIN_BUTTONS 0.24  // SPACE BETWEEN THE TOP EDGE AND THE 2 FIRST BUTTONS
#define MARGIN_BUTTONS 0.18         // SPACE BETWEEN TWO BUTTONS
#define RESIZE_BUTTONS 1.5          // RESIZE THE BUTTONS
#define RESIZE_SQUIRREL 2.4         // RESIZE THE ANGRY SQUIRREL
#define READJUST_SQUIRREL 0.01
#define SIZE_OF_BUTTON 0.1

/* **************************************************************** */

struct Env_t {
  SDL_Texture* background;
  SDL_Texture* Fire_hydrant;
  SDL_Texture* water;
  SDL_Texture* marks;

  SDL_Texture* flame;
  SDL_Texture* error;

  SDL_Texture* quit;
  SDL_Texture* redo;
  SDL_Texture* undo;
  SDL_Texture* restart;
  SDL_Texture* save;
  SDL_Texture* solve;
  SDL_Texture* wall_U;
  SDL_Texture* wall_0;
  SDL_Texture* wall_1;
  SDL_Texture* wall_2;
  SDL_Texture* wall_3;
  SDL_Texture* wall_4;
  SDL_Texture* win;
  float size_square_x;
  float size_square_y;
  float size_button_x;
  float size_button_y;
  float size_win_x;
  float size_win_y;

  game g;
  FILE* file;
};

/* **************************************************************** */

void place_texture(SDL_Texture* texture, SDL_Rect rect, SDL_Renderer* ren, int i, int j, int h, int w)
{
  SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
  rect.x = i;
  rect.y = j;
  rect.h = rect.h * (h / DEFAULT_WINDOW);
  rect.w = rect.w * (w / DEFAULT_WINDOW);
  SDL_RenderCopy(ren, texture, NULL, &rect);
}

/* **************************************************************** */

Env* init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[])
{
  Env* env = malloc(sizeof(struct Env_t));

  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* PUT YOUR CODE HERE TO INIT TEXTURES, ... */
  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  env->win = IMG_LoadTexture(ren, WIN);
  if (!env->win) ERROR("IMG_LoadTexture: %s\n", WIN);

  env->Fire_hydrant = IMG_LoadTexture(ren, FIRE_HYDRANT);
  if (!env->Fire_hydrant) ERROR("IMG_LoadTexture: %s\n", FIRE_HYDRANT);

  env->wall_U = IMG_LoadTexture(ren, WALL_U);
  if (!env->wall_U) ERROR("IMG_LoadTexture: %s\n", WALL_U);

  env->wall_1 = IMG_LoadTexture(ren, WALL_1);
  if (!env->wall_1) ERROR("IMG_LoadTexture: %s\n", WALL_1);

  env->wall_2 = IMG_LoadTexture(ren, WALL_2);
  if (!env->wall_2) ERROR("IMG_LoadTexture: %s\n", WALL_2);

  env->wall_3 = IMG_LoadTexture(ren, WALL_3);
  if (!env->wall_3) ERROR("IMG_LoadTexture: %s\n", WALL_3);

  env->wall_4 = IMG_LoadTexture(ren, WALL_4);
  if (!env->wall_4) ERROR("IMG_LoadTexture: %s\n", WALL_4);

  env->wall_0 = IMG_LoadTexture(ren, WALL_0);
  if (!env->wall_0) ERROR("IMG_LoadTexture: %s\n", WALL_0);

  env->water = IMG_LoadTexture(ren, WATER);
  if (!env->water) ERROR("IMG_LoadTexture: %s\n", WATER);

  env->marks = IMG_LoadTexture(ren, MARKS);
  if (!env->marks) ERROR("IMG_LoadTexture: %s\n", MARKS);

  env->flame = IMG_LoadTexture(ren, FLAME);
  if (!env->flame) ERROR("IMG_LoadTexture: %s\n", FLAME);

  env->quit = IMG_LoadTexture(ren, QUIT);
  if (!env->quit) ERROR("IMG_LoadTexture: %s\n", QUIT);


  env->redo = IMG_LoadTexture(ren, REDO);
  if (!env->redo) ERROR("IMG_LoadTexture: %s\n", REDO);

  env->undo = IMG_LoadTexture(ren, UNDO);
  if (!env->undo) ERROR("IMG_LoadTexture: %s\n", UNDO);

  env->restart = IMG_LoadTexture(ren, RESTART);
  if (!env->restart) ERROR("IMG_LoadTexture: %s\n", RESTART);

  env->save = IMG_LoadTexture(ren, SAVE);
  if (!env->save) ERROR("IMG_LoadTexture: %s\n", SAVE);

  env->solve = IMG_LoadTexture(ren, SOLVE);
  if (!env->solve) ERROR("IMG_LoadTexture: %s\n", SOLVE);

  env->error = IMG_LoadTexture(ren, ERREUR);
  if (!env->error) ERROR("IMG_LoadTexture: %s\n", ERREUR);
  srand(time(NULL));
  env->g = game_random(7, 7, false, 10, false);
  /* GAME CHOOSE */
  if (argc == 2) {
    env->g = game_load(argv[1]);
  } else {
    env->file = fopen("save.txt", "r");
    /* GAME SAVE */
    if (env->file != NULL) {
      env->g = game_load("save.txt");
    }
    /* GAME DEFAULT */
    else {
      env->g =game_random(7, 7, false, 10, false); 
    }
  }
  return env;
}

/* **************************************************************** */

void render(SDL_Window* win, SDL_Renderer* ren, Env* env)
{
  SDL_Rect rect;
  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  /* render background texture */

  SDL_RenderCopy(ren, env->background, NULL, NULL); /* stretch it */

  /* render a grid with lines */
  SDL_SetRenderDrawColor(ren, 255, 0, 0, SDL_ALPHA_OPAQUE); /* red */

  uint rows = game_nb_rows(env->g);
  uint cols = game_nb_cols(env->g);
  env->size_square_x = PERC_OF_SQUARE * w;
  env->size_square_y = PERC_OF_SQUARE * h;
  env->size_button_x = w / RESIZE_BUTTONS;
  env->size_button_y = h / RESIZE_BUTTONS;
  env->size_win_x = w;
  env->size_win_y = h;

  int size_of_grid = SIDE_MARGIN * w + env->g->nb_cols * PERC_OF_SQUARE * w;
  int height_of_buttons = h * HEIGHT_MARGIN_BUTTONS;
  int horizontal_of_buttons = size_of_grid + w * SIDE_MARGIN;

  place_texture(env->undo, rect, ren, horizontal_of_buttons, height_of_buttons, h / RESIZE_BUTTONS, w / RESIZE_BUTTONS);

  place_texture(env->redo, rect, ren, horizontal_of_buttons + w * MARGIN_BUTTONS, height_of_buttons, h / RESIZE_BUTTONS,
                w / RESIZE_BUTTONS);

  height_of_buttons = height_of_buttons + h * MARGIN_BUTTONS;

  place_texture(env->solve, rect, ren, horizontal_of_buttons, height_of_buttons, h / RESIZE_BUTTONS,
                w / RESIZE_BUTTONS);

  place_texture(env->restart, rect, ren, horizontal_of_buttons + w * MARGIN_BUTTONS, height_of_buttons,
                h / RESIZE_BUTTONS, w / RESIZE_BUTTONS);

  /* quit the game */
  height_of_buttons = height_of_buttons + h * MARGIN_BUTTONS;
  place_texture(env->quit, rect, ren, horizontal_of_buttons, height_of_buttons, h / RESIZE_BUTTONS, w / RESIZE_BUTTONS);

  /* save the state of the game */
  place_texture(env->save, rect, ren, horizontal_of_buttons + w * MARGIN_BUTTONS, height_of_buttons, h / RESIZE_BUTTONS,
                w / RESIZE_BUTTONS);

  // float wi = w, hi = h;
  for (uint i = 0; i <= rows; i++) {
    for (uint j = 0; j <= cols; j++) {
      int ji = j * PERC_OF_SQUARE * h + h * HEIGHT_MARGIN;
      int ii = i * PERC_OF_SQUARE * w + SIDE_MARGIN * w;
      SDL_RenderDrawLine(ren, ii, h * HEIGHT_MARGIN, ii, cols * PERC_OF_SQUARE * h + h * HEIGHT_MARGIN);
      SDL_RenderDrawLine(ren, w * SIDE_MARGIN, ji, rows * PERC_OF_SQUARE * w + w * SIDE_MARGIN, ji);
      if (i != rows && j != cols) {
        if (game_has_error(env->g, i, j)) {
          place_texture(env->error, rect, ren, ii, ji + READJUST_SQUIRREL * w, h / RESIZE_SQUIRREL,
                        w / RESIZE_SQUIRREL);
        } else if (game_is_lighted(env->g, i, j) && !game_is_lightbulb(env->g, i, j)) {
          place_texture(env->water, rect, ren, ii, ji, h, w);
        } else if (!game_is_lighted(env->g, i, j) && game_is_blank(env->g, i, j)) {
          place_texture(env->flame, rect, ren, ii, ji, h, w);
        } else if (game_is_lightbulb(env->g, i, j)) {
          place_texture(env->Fire_hydrant, rect, ren, ii, ji, h, w);
        } else if (game_is_marked(env->g, i, j)) {
          place_texture(env->marks, rect, ren, ii, ji, h, w);
        } else if (game_is_black(env->g, i, j)) {
          int b_n = game_get_black_number(env->g, i, j);
          if (!game_is_over(env->g)) {
            switch (b_n) {
              case 0:
                place_texture(env->wall_0, rect, ren, ii, ji, h, w);
                break;
              case 1:
                place_texture(env->wall_1, rect, ren, ii, ji, h, w);
                break;
              case 2:
                place_texture(env->wall_2, rect, ren, ii, ji, h, w);
                break;
              case 3:
                place_texture(env->wall_3, rect, ren, ii, ji, h, w);
                break;
              case 4:
                place_texture(env->wall_4, rect, ren, ii, ji, h, w);
                break;
              default:
                place_texture(env->wall_U, rect, ren, ii, ji, h, w);
            }
          } else {
            place_texture(env->win, rect, ren, ii, ji + READJUST_SQUIRREL * w, h / RESIZE_SQUIRREL,
                          w / RESIZE_SQUIRREL);
          }
        }
      }
    }
  }
}

/* **************************************************************** */

bool process(SDL_Window* win, SDL_Renderer* ren, Env* env, SDL_Event* e)
{
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* generic events */
  if (e->type == SDL_QUIT) {
    return true;
  }

  SDL_Point mouse;
  SDL_GetMouseState(&mouse.x, &mouse.y);
  float pos_x = (mouse.x - w * SIDE_MARGIN) / env->size_square_x;
  float pos_y = (mouse.y - HEIGHT_MARGIN * h) / env->size_square_y;

  // events mouse
  if (e->type == SDL_MOUSEBUTTONDOWN) {
    int size_of_grid = env->size_square_x * env->g->nb_cols + SIDE_MARGIN * env->size_win_x;
    int height_of_buttons = h * HEIGHT_MARGIN_BUTTONS;
    int horizontal_of_buttons = size_of_grid + w * SIDE_MARGIN;

    if (mouse.y > h * HEIGHT_MARGIN && mouse.y <= h * PERC_OF_SQUARE * game_nb_rows(env->g) + HEIGHT_MARGIN * h &&
        mouse.x > SIDE_MARGIN * env->size_win_x &&
        mouse.x <= size_of_grid) {                     // allows to now that we are clicking inside grid
      switch (game_get_state(env->g, pos_x, pos_y)) {  // change the state in function of the number of the actual state
        case S_BLANK:
          game_play_move(env->g, pos_x, pos_y, S_LIGHTBULB);
          break;

        case S_LIGHTBULB:
          game_play_move(env->g, pos_x, pos_y, S_MARK);

          break;

        case S_MARK:
          game_play_move(env->g, pos_x, pos_y, S_BLANK);
          break;
        default:
          return false;
      }
    }

    else if (mouse.x > horizontal_of_buttons && mouse.x <= (horizontal_of_buttons + w * 0.1)) {
      if (mouse.y > height_of_buttons &&
          mouse.y <=
              height_of_buttons +
                  env->size_win_y * SIZE_OF_BUTTON) {  // allows to know the click is in the first column of buttons
        game_undo(env->g);
      } else if (mouse.y > height_of_buttons + env->size_win_y * MARGIN_BUTTONS &&
                 mouse.y <= height_of_buttons + env->size_win_y * SIZE_OF_BUTTON + env->size_win_y * MARGIN_BUTTONS) {
        game_solve(env->g);

      } else if (mouse.y > height_of_buttons + env->size_win_y * MARGIN_BUTTONS * 2 &&
                 mouse.y <=
                     height_of_buttons + env->size_win_y * SIZE_OF_BUTTON + env->size_win_y * MARGIN_BUTTONS * 2) {
        return true;
      }
    }

    else if ((mouse.x > horizontal_of_buttons + env->size_win_x * MARGIN_BUTTONS) &&
             mouse.x <= (horizontal_of_buttons + env->size_win_x * MARGIN_BUTTONS +
                         env->size_win_x * 0.1)) {  // allows to know the click is in the second column of buttons
      if (mouse.y > height_of_buttons && mouse.y <= height_of_buttons + env->size_win_y * SIZE_OF_BUTTON) {
        game_redo(env->g);
      } else if (mouse.y > height_of_buttons + env->size_win_y * MARGIN_BUTTONS &&
                 mouse.y <= height_of_buttons + env->size_win_y * SIZE_OF_BUTTON + env->size_win_y * MARGIN_BUTTONS) {
        game_restart(env->g);

      } else if (mouse.y > height_of_buttons + env->size_win_y * MARGIN_BUTTONS * 2 &&
                 mouse.y <=
                     height_of_buttons + env->size_win_y * SIZE_OF_BUTTON + env->size_win_y * MARGIN_BUTTONS * 2) {
        game_save(env->g, "save.txt");
      }
    }
  }

  // events keyboards
  else if (e->type == SDL_KEYDOWN) {
    switch (e->key.keysym.sym) {
      case SDLK_z:
        game_undo(env->g);
        break;

      case SDLK_y:
        game_redo(env->g);
        break;

      case SDLK_r:
        game_restart(env->g);
        break;

      case SDLK_w:
        game_save(env->g, "save.txt");
        break;

      case SDLK_s:
        game_solve(env->g);
        break;

      case SDLK_q:
        return true;
        break;
    }

    // allows to now that we are clicking inside grid
    if (mouse.y > env->size_win_y * HEIGHT_MARGIN &&
        mouse.y <= env->size_square_y * game_nb_rows(env->g) + HEIGHT_MARGIN * env->size_win_y &&
        mouse.x > SIDE_MARGIN * env->size_win_x &&
        mouse.x <= env->size_square_x * game_nb_cols(env->g) + SIDE_MARGIN * env->size_win_x) {
      switch (e->key.keysym.sym) {
        case SDLK_l:
          game_play_move(env->g, pos_x, pos_y, S_LIGHTBULB);
          break;

        case SDLK_m:
          game_play_move(env->g, pos_x, pos_y, S_MARK);
          break;

        case SDLK_b:
          game_play_move(env->g, pos_x, pos_y, S_BLANK);
          break;
        default:
          return false;
      }
    }
  }
  return false;
}

/* **************************************************************** */

void clean(SDL_Window* win, SDL_Renderer* ren, Env* env)
{
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */
  SDL_DestroyTexture(env->background);
  SDL_DestroyTexture(env->Fire_hydrant);
  SDL_DestroyTexture(env->water);
  SDL_DestroyTexture(env->flame);
  SDL_DestroyTexture(env->error);
  SDL_DestroyTexture(env->wall_0);
  SDL_DestroyTexture(env->wall_1);
  SDL_DestroyTexture(env->wall_2);
  SDL_DestroyTexture(env->wall_3);
  SDL_DestroyTexture(env->wall_4);
  SDL_DestroyTexture(env->wall_U);
  SDL_DestroyTexture(env->marks);
  SDL_DestroyTexture(env->quit);
  SDL_DestroyTexture(env->redo);
  SDL_DestroyTexture(env->undo);
  SDL_DestroyTexture(env->restart);
  SDL_DestroyTexture(env->save);
  SDL_DestroyTexture(env->solve);
  SDL_DestroyTexture(env->win);
  game_delete(env->g);
  free(env);
}

/* **************************************************************** */
