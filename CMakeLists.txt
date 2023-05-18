# Light Up Project
# Copyright University of Bordeaux. All rights reserved, 2021.

cmake_minimum_required(VERSION 3.0)

project("Lightup" C)

# test
include(CTest)
enable_testing()

set(CMAKE_C_FLAGS "-std=c99 -Wall -pthread -Wextra -Wunused-parameter")
set(CMAKE_C_FLAGS_DEBUG "-g --coverage")    # use CMake option: -DCMAKE_BUILD_TYPE=DEBUG
set(CMAKE_C_FLAGS_RELEASE "-O3")   # use CMake option: -DCMAKE_BUILD_TYPE=RELEASE

############################# SRC #############################

# game library
add_library(game game.c game_ext.c game_aux.c game_private.c queue.c game_tools.c game_opti.c)

## use a CMake3 package to solve SDL2 dependencies
find_package(SDL2 REQUIRED)
set(SDL2_ALL_INC ${SDL2_INCLUDE_DIRS})
set(SDL2_ALL_LIBS ${SDL2_LIBRARIES} -lSDL2_ttf -lSDL2_image) # add extra SDL2 libraries


# game text
add_executable(game_text game_text.c)
target_link_libraries(game_text game)

# game tests
add_executable(game_test game_test.c game_test_aux.c game_test_v1.c game_test_v2.c game_examples.c game_test_tools.c  game_opti.c)
target_link_libraries(game_test game)

# game solve
add_executable(game_solve game_solve.c )
target_link_libraries(game_solve game)

## print this 
message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

# game sdl
include_directories(${SDL2_ALL_INC})
add_executable(game_sdl game_sdl.c model.c )
target_link_libraries(game_sdl game ${SDL2_ALL_LIBS} m)

############################# TEST V1 #############################

# Aux Tests(game_aux.h)
add_test(testv1_default ./game_test "default")
add_test(testv1_default_solution ./game_test "default_solution")
add_test(testv1_print ./game_test "print")

# Basic Tests (game.h)
add_test(testv1_new ./game_test "new")
add_test(testv1_new_empty ./game_test "new_empty")
add_test(testv1_copy ./game_test "copy")
add_test(testv1_equal ./game_test "equal")
add_test(testv1_delete ./game_test "delete")
add_test(testv1_get_square ./game_test "get_square")
add_test(testv1_set_square ./game_test "set_square")
add_test(testv1_get_state ./game_test "get_state")
add_test(testv1_get_flags ./game_test "get_flags")
add_test(testv1_is_state ./game_test "is_state")
add_test(testv1_has_flag ./game_test "has_flag")
add_test(testv1_play_move ./game_test "play_move")
add_test(testv1_check_move ./game_test "check_move")
add_test(testv1_restart ./game_test "restart")
add_test(testv1_update_flags ./game_test "update_flags")
add_test(testv1_is_over ./game_test "is_over")

############################# TEST V2 #############################

# Ext Tests (game_ext.h)
add_test(testv2_new_ext ./game_test "new_ext")
add_test(testv2_new_empty_ext ./game_test "new_empty_ext")
add_test(testv2_game_1d ./game_test "game_1d")
add_test(testv2_equal_ext ./game_test "equal_ext")
add_test(testv2_copy_ext ./game_test "copy_ext")
add_test(testv2_game_wrapping_2x2 ./game_test "game_wrapping_2x2")
add_test(testv2_game_wrapping_3x3 ./game_test "game_wrapping_3x3")
add_test(testv2_game_wrapping_5x3 ./game_test "game_wrapping_5x3")
add_test(testv2_game_wrapping_error ./game_test "game_wrapping_error")
add_test(testv2_undo_one ./game_test "undo_one")
add_test(testv2_undo_redo_some ./game_test "undo_redo_some")
add_test(testv2_undo_redo_all ./game_test "undo_redo_all")
add_test(testv2_restart_undo ./game_test "restart_undo")

############################# TESTS TOOLS #############################

add_test(test_tools_save ./game_test "save")
add_test(test_tools_load ./game_test "load")
add_test(test_tools_nb_solutions ./game_test "nb_solutions")
add_test(test_tools_solve ./game_test "solve")
# EOF


## copy useful data files in build dir
foreach(file "background.png" "bouche-incendie.png" "flame.png" "error.png" 
"water.png" "marks.png" 
"Quit.png" "Redo.png" "Restart.png" "Save.png" "Undo.png" "Solve.png"
"wall_0.png" "wall_1.png" "wall_2.png" "wall_3.png" "wall_4.png" "wall_U.png""won.png")
file(COPY ${file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(file)