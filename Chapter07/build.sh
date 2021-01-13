#!/bin/bash

################################################
# If you have a bash shell you can run this
# script.  If not you can copy and paste the 
# line below to the command line.
################################################

em++ main.cpp collider.cpp ship.cpp enemy_ship.cpp player_ship.cpp projectile.cpp projectile_pool.cpp -std=c++17 --preload-file sprites -s USE_WEBGL2=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"] -o collider.html



