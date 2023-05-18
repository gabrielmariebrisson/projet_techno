/**
 * @file game.h
 * @brief Basic Game Functions.
 * @details See @ref index for further details.
 * @copyright University of Bordeaux. All rights reserved, 2021.
 **/

#ifndef __GAME_H__
#define __GAME_H__

#include <stdbool.h>

/**
 * @brief Standard unsigned integer type.
 **/
typedef unsigned int uint;

/**
 * @brief Size of the default game grid.
 **/
#define DEFAULT_SIZE 7

/**
 * @brief Different squares used in the game.
 * @details The value of a square is an integer represented on one byte, with a
 * low-order half-byte that codes for the square state (blank, black wall, light
 * bulb or mark) and a high-order half-byte that codes for the additionnal
 * square flags (lighted or error). While each square has a single state, it can
 * have zero, one or several flags (combined with bitwise operations). For
 * instance, a light bulb can have both the lighted and error flags. In this
 * case, the raw square value is S_LIGHTBULB | F_LIGHTED | F_ERROR, using the
 * bitwise OR operator (|). For more details on bitwise operations, see
 * https://en.wikipedia.org/wiki/Bitwise_operations_in_C.
 **/
typedef enum {
  /* states */
  S_BLANK = 0,        /**< a blank square */
  S_LIGHTBULB = 1,    /**< a light bulb */
  S_MARK = 2,         /**< a marked square (as not-a-light) */
  S_BLACK = 8,        /**< black wall base code */
  S_BLACK0 = S_BLACK, /**< a numbered black wall (with 0 adjacent lights) */
  S_BLACK1,           /**< a numbered black wall (with 1 adjacent light) */
  S_BLACK2,           /**< a numbered black wall (with 2 adjacent lights) */
  S_BLACK3,           /**< a numbered black wall (with 3 adjacent lights) */
  S_BLACK4,           /**< a numbered black wall (with 4 adjacent lights) */
  S_BLACKU,           /**< an unnumbered black wall (any number of adjacent lights) */
  /* flags */
  F_LIGHTED = 16, /**< lighted flag */
  F_ERROR = 32    /**< error flag */
} square;

/** state mask used in square enum */
#define S_MASK 0x0F

/** flag mask used in square enum */
#define F_MASK 0xF0

/** square mask used in square enum */
#define A_MASK 0xFF

/**
 * @brief The structure pointer that stores the game state.
 **/
typedef struct game_s* game;

/**
 * @brief The structure constant pointer that stores the game state.
 * @details That means that it is not possible to modify the game using this
 * pointer.
 **/
typedef const struct game_s* cgame;

/**
 * @brief Creates a new game with default size and initializes it.
 * @param squares an array describing the initial square values (<a
 * href="https://en.wikipedia.org/wiki/row-_and_column-major_order">row-major
 * storage</a>)
 * @pre @p squares must be an initialized array of default size squared.
 * @return the created game
 **/
game game_new(square* squares);

/**
 * @brief Creates a new empty game with defaut size.
 * @details All squares are initialized with blank squares.
 * @return the created game
 **/
game game_new_empty(void);

/**
 * @brief Duplicates a game.
 * @param g the game to copy
 * @return the copy of the game
 * @pre @p g must be a valid pointer toward a game structure.
 **/
game game_copy(cgame g);

/**
 * @brief Tests if two games are equal (same states and same flags).
 * @param g1 the first game
 * @param g2 the second game
 * @return true if the two games are equal, false otherwise
 * @pre @p g1 must be a valid pointer toward a game structure.
 * @pre @p g2 must be a valid pointer toward a game structure.
 **/
bool game_equal(cgame g1, cgame g2);

/**
 * @brief Deletes the game and frees the allocated memory.
 * @param g the game to delete
 * @pre @p g must be a valid pointer toward a game structure.
 **/
void game_delete(game g);

/**
 * @brief Sets the value of a given square.
 * @details This function is useful for initializing the squares of an empty
 * game.
 * @param g the game
 * @param i row index
 * @param j column index
 * @param s the square value
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @pre @p s must be a valid square value.
 **/
void game_set_square(game g, uint i, uint j, square s);

/**
 * @brief Gets the raw value of a given square.
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return the square value
 **/
square game_get_square(cgame g, uint i, uint j);

/**
 * @brief Gets the state of a given square.
 * @details See description of enum square.
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return the square state
 **/
square game_get_state(cgame g, uint i, uint j);

/**
 * @brief Gets the flags of a given square.
 * @details See description of enum square.
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return the square flags
 **/
square game_get_flags(cgame g, uint i, uint j);

/**
 * @brief Test if a given square is blank.
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return true if the square is blank
 **/
bool game_is_blank(cgame g, uint i, uint j);

/**
 * @brief Test if a given square is a lightbulb.
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return true if the square is a lightbulb
 **/
bool game_is_lightbulb(cgame g, uint i, uint j);

/**
 * @brief Test if a given square is black (whether or not it is numbered).
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return true if the square is black
 **/
bool game_is_black(cgame g, uint i, uint j);

/**
 * @brief Get the number of lightbulbs expected against a black wall.
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @pre The square at position (i,j) must be a black wall (either numbered or
 * unumbered).
 * @return the back wall number, or -1 if it is unumbered
 **/
int game_get_black_number(cgame g, uint i, uint j);

/**
 * @brief Test if a given square is marked (as not-a-light).
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return true if the square is marked
 **/
bool game_is_marked(cgame g, uint i, uint j);

/**
 * @brief Test if a given square is lighted.
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return true if the square is lighted
 **/
bool game_is_lighted(cgame g, uint i, uint j);

/**
 * @brief Test if a given square has an error flag
 * @details An error can only occur on an numbered black wall or on a light
 * bulb.
 * @param g the game
 * @param i row index
 * @param j column index
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @return true if the square has an error flag
 **/
bool game_has_error(cgame g, uint i, uint j);

/**
 * @brief Checks if a given move is legal.
 * @details This function checks that it is possible to play a move at a given
 * position in the grid. More precisely, a move is said to be legal: 1) if the
 * coordinates (i,j) are inside the grid, 2) if @p s is either a blank, light
 * bulb or mark square, and 3) if the current square at (i,j) is not a black
 * square.
 * @param g the game
 * @param i row index
 * @param j column index
 * @param s the square value
 * @pre @p g must be a valid pointer toward a game structure.
 * @return false if the move is not legal.
 **/
bool game_check_move(cgame g, uint i, uint j, square s);

/**
 * @brief Plays a move in a given square.
 * @details The grid flags are updated consequently after each move.
 * @param g the game
 * @param i row index
 * @param j column index
 * @param s the square value
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i < game height
 * @pre @p j < game width
 * @pre @p s must be either BLANK, LIGHTBULB or MARK.
 * @pre The game square at position (i,j) must not be a black square.
 **/
void game_play_move(game g, uint i, uint j, square s);

/**
 * @brief Update all grid flags.
 * @details This is a low-level function, which is not intended to be used
 * directly by end-user during the game.
 * @param g the game
 * @pre @p g must be a valid pointer toward a game structure.
 **/
void game_update_flags(game g);

/**
 * @brief Checks if the game is won.
 * @param g the game
 * @details This function checks that all the game rules (decribed on @ref
 * index) are satisfied, ie. that all the blank squares are ligthed, without any
 * errors.
 * @return true if the game ended successfully, false otherwise
 * @pre @p g must be a valid pointer toward a game structure.
 **/
bool game_is_over(cgame g);

/**
 * @brief Restarts a game.
 * @details All the game is reset to its initial state. In particular, all the
 * squares except black walls are reset to blank, and all flags are cleared.
 * @param g the game
 * @pre @p g must be a valid pointer toward a game structure.
 **/
void game_restart(game g);

#endif  // __GAME_H__
