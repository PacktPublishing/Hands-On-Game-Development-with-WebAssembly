# Compiling the locked on camera

If you would like to compile the locked on camera for chapter 11, please use the cd command to enter the locked-on-camera directory.

cd locked-on-camera

Then compile the locked on camera with the following emscripten command.

em++ asteroid.cpp camera.cpp collider.cpp emitter.cpp enemy_ship.cpp finite_state_machine.cpp locator.cpp main.cpp particle.cpp player_ship.cpp projectile_pool.cpp projectile.cpp range.cpp render_manager.cpp shield.cpp ship.cpp star.cpp vector.cpp -o camera.html --preload-file sprites -std=c++17 -s USE_WEBGL2=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"] -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"]

The em++ command in the book is missing the locator.cpp file.