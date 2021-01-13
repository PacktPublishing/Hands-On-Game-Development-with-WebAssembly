#!/bin/bash

################################################
# If you have a bash shell you can run this
# script.  If not you can copy and paste the 
# line below to the command line.
################################################

em++ asteroid.cpp collider.cpp emitter.cpp enemy_ship.cpp finite_state_machine.cpp main.cpp particle.cpp player_ship.cpp projectile_pool.cpp projectile.cpp range.cpp shield.cpp ship.cpp star.cpp vector.cpp -o ai.html --preload-file sprites -std=c++17 -s USE_WEBGL2=1 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"]
