# @file Makefile
# @brief Build web version of game project.
# @author aurelien.esnard@u-bordeaux.fr
# @copyright University of Bordeaux. All rights reserved, 2022.

ALL: game.js game.wasm

LIBSRC  := $(wildcard src/*.c)
LIBOBJ  := $(LIBSRC:.c=.o)

game.wasm game.js: wrapper.o libgame.a 
	emcc $^ -o $@ -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_RUNTIME_METHODS=ccall,cwrap

%.o: %.c
	emcc -I src -c $< -o $@

libgame.a: $(LIBOBJ)
	emar rcs libgame.a $^

clean:
	rm -f *.o src/*.o game.wasm game.js libgame.a

# EOF
