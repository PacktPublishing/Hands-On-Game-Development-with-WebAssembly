all : 
	em++ main.cpp enemy_ship.cpp player_ship.cpp projectile.cpp projectile_pool.cpp -std=c++17 --preload-file sprites -s USE_WEBGL2=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"] -o game_objects.html

clean : 
	rm game_objects.html
