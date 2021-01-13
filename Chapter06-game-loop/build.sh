#!/bin/bash

################################################
# If you have a bash shell you can run this
# script.  If not you can copy and paste the 
# line below to the command line.
################################################

emcc game_loop.c -o gameloop.html --preload-file sprites -s NO_EXIT_RUNTIME=1 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"] -s EXTRA_EXPORTED_RUNTIME_METHODS="['cwrap','ccall']" -s USE_SDL=2

