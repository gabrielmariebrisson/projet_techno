/**
 * @file game_private.h
 * @brief Private Game Functions.
 * @copyright University of Bordeaux. All rights reserved, 2021.
 **/

#ifndef __GAME_PRIVATE_H__
#define __GAME_PRIVATE_H__

#include <stdbool.h>

#include "game.h"
#include "queue.h"

/* ************************************************************************** */
/*                                CONSTANTS                                   */
/* ************************************************************************** */

/** first state value in square enum */
#define S_START S_BLANK

/** last state value in square enum */
#define S_END S_BLACKU

/* ************************************************************************** */
/*                             DATA TYPES                                     */
/* ************************************************************************** */

/**
 * @brief Game structure.
 * @details This is an opaque data type.
 */
struct game_s {
  uint nb_rows;      /**< number of rows in the game */
  uint nb_cols;      /**< number of columns in the game */
  square* squares;   /**< the grid of squares */
  bool wrapping;     /**< the wrapping option */
  queue* undo_stack; /**< stack to undo moves */
  queue* redo_stack; /**< stack to redo moves */
};

/**
 * @brief Move structure.
 * @details This structure is used to save the game history.
 */
struct move_s {
  uint i, j;
  square old, new;
};

typedef struct move_s move;

typedef enum { HERE, UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT } direction;

/* ************************************************************************** */
/*                                MACRO                                       */
/* ************************************************************************** */

#define INDEX(g, i, j) ((i) * (g->nb_cols) + (j))
#define SQUARE(g, i, j) ((g)->squares[(INDEX(g, i, j))])
#define STATE(g, i, j) (SQUARE(g, i, j) & S_MASK)
#define FLAGS(g, i, j) (SQUARE(g, i, j) & F_MASK)
#define MAX(x, y) ((x > (y)) ? (x) : (y))

/* ************************************************************************** */
/*                             STACK ROUTINES                                 */
/* ************************************************************************** */

/** push a move in the stack */
void _stack_push_move(queue* q, move m);

/** pop a move from the stack */
move _stack_pop_move(queue* q);

/** test if the stack is empty */
bool _stack_is_empty(queue* q);

/** clear all the stack */
void _stack_clear(queue* q);

/* ************************************************************************** */
/*                          GAME PRIVATE ROUTINES                             */
/* ************************************************************************** */

/**
 * @brief convert a square into a character
 *
 * @param s a square
 * @return the corresponding character, '?' otherwise
 */
char _square2str(square s);

/**
 * @brief convert a character into square
 *
 * @param c character
 * @return the corresponding square value, -1 otherwise
 */
int _str2square(char c);

/**
 * @brief check square value
 *
 * @param s a square
 * @return true, if the square has a correct value
 */

bool _check_square(square s);

/* ************************************************************************** */
/*                                 NEIGHBORHOOD                               */
/* ************************************************************************** */

/**
 * @brief test if a given square is inside the board
 *
 * @param g the game
 * @param i row index
 * @param j column index
 * @return true if inside, false otherwise
 */
bool _inside(cgame g, int i, int j);

/**
 * @brief compute the next square coordinates in a given direction
 *
 * @details If the next coordinates are out of board (return false), the
 * coordinates of current square (*pi,*pj) are not updated. If the wrapping
 * option is enabled, this function always returns true.
 *
 * @param g the game
 * @param pi address of current row index to be updated
 * @param pj address of current column index to be updated
 * @param dir the direction to consider
 * @return true if the next cooardinate are inside the board, false otherwise
 */
bool _next(cgame g, int* pi, int* pj, direction dir);

/**
 * @brief test if the neighbour of a given square is inside the board
 *
 * @param g the game
 * @param i row index
 * @param j column index
 * @param dir the direction in which to consider the neighbour
 * @return true if inside, false otherwise
 */
bool _inside_neigh(cgame g, int i, int j, direction dir);

/**
 * @brief test if a square has a given value
 *
 * @param g the game
 * @param i row index
 * @param j column index
 * @param s the square value to be compared
 * @param m a mask square to be applied before comparison
 * @return true if equal, false otherwise
 */
bool _test(cgame g, int i, int j, square s, uint mask);

/**
 * @brief test if the neighbour square has a given value
 *
 * @param g the game
 * @param i row index
 * @param j column index
 * @param s the square value to be compared
 * @param m a mask square to be applied before comparison
 * @param dir the direction in which to consider the neighbour
 * @return true if equal, false otherwise
 */
bool _test_neigh(cgame g, int i, int j, square s, uint m, direction dir);

/**
 * @brief test if a square can be found in the neighbourhood of another
 *
 * @param g the game
 * @param i row index
 * @param j column index
 * @param s the square value to look for
 * @param m a mask square to be applied before comparison
 * @param diag enable diagonal adjacency
 * @return true if found, false otherwise
 */
bool _neigh(cgame g, uint i, uint j, square s, uint m, bool diag);

/**
 * @brief get the neighbourhood size
 *
 * @param g the game
 * @param i row index
 * @param j column index
 * @param diag enable diagonal adjacency
 *
 * @return the neighbourhood size
 */
uint _neigh_size(cgame g, uint i, uint j, bool diag);

/**
 * @brief get the number of squares with a certain value in the neighbourhood of
 * a given square
 *
 * @param g the game
 * @param i row index
 * @param j column index
 * @param s the square value to look for
 * @param m a mask square to be applied before comparison
 * @param diag enable diagonal adjacency
 * @return the number of squares found
 */
uint _neigh_count(cgame g, uint i, uint j, square s, uint m, bool diag);

#endif  // __GAME_PRIVATE_H__
