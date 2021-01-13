#!/bin/bash

################################################
# If you have a bash shell you can run this
# script.  If not you can copy and paste the 
# line below to the command line.
################################################

em++ collider.cpp emitter.cpp enemy_ship.cpp particle.cpp player_ship.cpp point.cpp projectile_pool.cpp projectile.cpp ship.cpp main.cpp -o particle_system.html --preload-file sprites -std=c++17 -s USE_WEBGL2=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"] -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"]
