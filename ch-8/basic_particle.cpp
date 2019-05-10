#include "game.hpp"
#include <emscripten/bind.h>

SDL_Window *window;
SDL_Renderer *renderer;
Emitter* emitter = NULL;

Uint32 last_time = 0;
Uint32 current_time = 0;
Uint32 diff_time = 0;
float delta_time = 0.0f;

extern "C"
EMSCRIPTEN_KEEPALIVE
void add_emitter(char* file_name, int max_particles, float min_angle, float max_angle,
                    Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    float min_starting_velocity, float max_starting_velocity,
                    Uint32 emission_rate, float x_pos, float y_pos, float radius) {
    if( emitter != NULL ) {
        delete emitter;
    }

    emitter = new Emitter(file_name, max_particles, min_angle, max_angle,
                            particle_lifetime, acceleration, alpha_fade,
                            min_starting_velocity, max_starting_velocity,
                            emission_rate, x_pos, y_pos, radius );
}

extern "C"
EMSCRIPTEN_KEEPALIVE
void update_emitter(int max_particles, float min_angle, float max_angle,
                    Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    float min_starting_velocity, float max_starting_velocity,
                    Uint32 emission_rate, float x_pos, float y_pos, float radius ) {

    if( emitter == NULL ) {
        return;
    }

   emitter->Update(max_particles, min_angle, max_angle,
                    particle_lifetime, acceleration, alpha_fade,
                    min_starting_velocity, max_starting_velocity,
                    emission_rate, x_pos, y_pos, radius );
}

void show_emission() {
    current_time = SDL_GetTicks();

    delta_time = (double)(current_time - last_time) / 1000.0;
    diff_time = current_time - last_time;
    last_time = current_time;

    if( emitter == NULL ) {
        return;
    }

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );

    emitter->Move();

    SDL_RenderPresent( renderer );

}

int main() {
    printf("Enter Main\n");

    SDL_Init( SDL_INIT_VIDEO );

    int return_val = SDL_CreateWindowAndRenderer( 800, 600, 0, &window, &renderer );

    SDL_EventState(SDL_TEXTINPUT, SDL_DISABLE);
    SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
    SDL_EventState(SDL_KEYUP, SDL_DISABLE);

    if( return_val != 0 ) {
        printf("Error creating renderer %d: %s\n", return_val, IMG_GetError() );
        return 0;
    }

    last_time = SDL_GetTicks();
    emscripten_set_main_loop(show_emission, 0, 0);
    printf("Exit Main\n");
    return 1;
}
