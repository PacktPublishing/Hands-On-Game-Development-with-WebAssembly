#include "game.hpp"
#include <emscripten/bind.h>

SDL_Window *window;
SDL_Renderer *renderer;
Emitter* emitter = NULL;

Uint32 last_time = 0;
Uint32 current_time = 0;
Uint32 diff_time = 0;
Uint32 ms_per_frame = 250;

float delta_time = 0.0f;

float get_random_float( float min, float max ) {
    int int_min = (int)(min * 1000);
    int int_max = (int)(max * 1000);

    if( int_min > int_max ) {
        int temp = int_max;
        int_max = int_min;
        int_min = temp;
    }

    int int_diff = int_max - int_min;
    int int_rand = (int_diff == 0) ? 0 : rand() % int_diff;
    int_rand += int_min;

    return (float)int_rand / 1000.0;
}

extern "C"
EMSCRIPTEN_KEEPALIVE
void add_emitter(char* file_name, int max_particles, float min_angle, float max_angle,
                    Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    float min_starting_velocity, float max_starting_velocity,
                    Uint32 emission_rate, float x_pos, float y_pos, float radius,
                    /* new parameters */
                    float min_start_scale, float max_start_scale,
                    float min_end_scale, float max_end_scale,
                    Uint32 start_color, Uint32 end_color,
                    float burst_time_pct, Uint32 burst_particles, 
                    bool loop, bool align_rotation, Uint32 emit_time_ms, 
                    Uint32 animation_frames ) {
    if( emitter != NULL ) {
        delete emitter;
    }
    printf("add emitter file=%s\n", file_name);
    emitter = new Emitter(file_name, max_particles, min_angle, max_angle,
                            particle_lifetime, acceleration, alpha_fade,
                            min_starting_velocity, max_starting_velocity,
                            emission_rate, x_pos, y_pos, radius,
                            /* new parameters */
                            min_start_scale, max_start_scale,
                            min_end_scale, max_end_scale,
                            start_color, end_color,
                            burst_time_pct, burst_particles, 
                            loop, align_rotation, emit_time_ms,
                            animation_frames
                             );

}

extern "C"
EMSCRIPTEN_KEEPALIVE
void update_emitter(int max_particles, float min_angle, float max_angle,
                    Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    float min_starting_velocity, float max_starting_velocity,
                    Uint32 emission_rate, float x_pos, float y_pos, float radius,
                    /* new parameters */
                    float min_start_scale, float max_start_scale,
                    float min_end_scale, float max_end_scale,
                    Uint32 start_color, Uint32 end_color,
                    float burst_time_pct, Uint32 burst_particles, 
                    bool loop, bool align_rotation, Uint32 emit_time_ms, 
                    Uint32 animation_frames
                     ) {

    if( emitter == NULL ) {
        return;
    }

   emitter->Update(max_particles, min_angle, max_angle,
                    particle_lifetime, acceleration, alpha_fade,
                    min_starting_velocity, max_starting_velocity,
                    emission_rate, x_pos, y_pos, radius,
                    /* new parameters */
                    min_start_scale, max_start_scale,
                    min_end_scale, max_end_scale,
                    start_color, end_color,
                    burst_time_pct, burst_particles, 
                    loop, align_rotation, emit_time_ms,
                    animation_frames
                     );
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
    int return_val = SDL_Init( SDL_INIT_VIDEO );

    if( return_val != 0 ) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    return_val = SDL_CreateWindowAndRenderer( 800, 600, 0, &window, &renderer );

    if( return_val != 0 ) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 1;
    }

    SDL_EventState(SDL_TEXTINPUT, SDL_DISABLE);
    SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
    SDL_EventState(SDL_KEYUP, SDL_DISABLE);

    if( return_val != 0 ) {
        printf("Error creating renderer %d: %s\n", return_val, IMG_GetError() );
        return 0;
    }

    last_time = SDL_GetTicks();
    emscripten_set_main_loop(show_emission, 0, 0);

    return 1;
}
