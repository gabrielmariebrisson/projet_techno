/**
 * @file game_opti.h
 * @brief Game opti.
 * @details See @ref index for further details.
 * @copyright University of Bordeaux. All rights reserved, 2021.
 *
 **/

#ifndef __GAME_OPTI_H__
#define __GAME_OPTI_H__
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_ext.h"
#include "game_private.h"
#include "queue.h"

/**
 * @name Game opti
 * @{
 */

/**

 **/
void _update_lighted_flags_wrapping(game g, uint i, uint j);

/**

 **/
void _update_lighted_flags_(game g, uint i, uint j);

/**

 */
void game_update_flags_opti(game g);

/**
 */
void game_play_move_opti(game g, uint i, uint j, square s);

/**
 * @}
 */

#endif  // __GAME_OPTI_H__
