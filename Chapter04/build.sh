#!/bin/bash

################################################
# If you have a bash shell you can run this
# script.  If not you can copy and paste the 
# line below to the command line.
################################################

emcc hello_sdl.c --emrun --preload-file font -s USE_SDL=2 -s USE_SDL_TTF=2 -o hello_sdl.html

# This chapter has more than one build

emcc sprite_move.c --preload-file sprites -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"] -o sprite_move.html