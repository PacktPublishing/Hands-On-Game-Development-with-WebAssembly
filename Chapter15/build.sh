#!/bin/bash

################################################
# If you have a bash shell you can run this
# script.  If not you can copy and paste the 
# line below to the command line.
################################################

emcc webgl-redux.c -o redux.html --preload-file sprites -s USE_WEBGL2=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"]

# This chapter has three builds

emcc glow.c -o glow.html --preload-file sprites -s USE_WEBGL2=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"]

# third build

emcc lighting.c -o lighting.html --preload-file sprites -s USE_SDL=2 -s USE_SDL_IMAGE -s SDL2_IMAGE_FORMATS=["png"]
