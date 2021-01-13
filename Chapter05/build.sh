#!/bin/bash

################################################
# If you have a bash shell you can run this
# script.  If not you can copy and paste the 
# line below to the command line.
################################################

emcc jskey.c -o jskey.html -s NO_EXIT_RUNTIME=1 --shell-file jskey_shell.html -s EXPORTED_FUNCTIONS="['_main', '_press_up', '_press_down', '_press_left', '_press_right', '_release_up', '_release_down', '_release_left', '_release_right']" -s EXTRA_EXPORTED_RUNTIME_METHODS="['cwrap', 'ccall']"

# There are multiple builds in this chapter
emcc keyboard.c -o keyboard.html -s USE_SDL=2

# compile keyboard_move.html
emcc keyboard_move.c -o keyboard_move.html --preload-file sprites -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"]