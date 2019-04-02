//#include "game.hpp"
#include <emscripten.h>
#include <stdlib.h>

void add_emitter(int looping, char* sprite_file_input, int max_particles,
                 int sprite_width, int sprite_height, int duration,
                 int min_particle_lifetime, int max_particle_lifetime,
                 int emission_time_ms, int emission_time_ms_variance,
                 int burst_time, int burst_particles, float max_velocity,
                 float min_start_velocity_x, float max_start_velocity_x,
                 float min_start_velocity_y, float max_start_velocity_y,
                 float min_acceleration, float max_acceleration ) {

    printf("looping=%s\n", looping ? "true" : "false");
    printf("sprite_file_input=%s\n", sprite_file_input);
    printf("max_particles=%d\n", max_particles);
    printf("sprite_width=%d\n", sprite_width);
    printf("sprite_height=%d\n", sprite_height);
    printf("duration=%d\n", duration);
    printf("min_particle_lifetime=%d\n", min_particle_lifetime);
    printf("max_particle_lifetime=%d\n", max_particle_lifetime);
    printf("emission_time_ms=%d\n", emission_time_ms);
    printf("emission_time_ms_variance=%d\n", emission_time_ms_variance);
    printf("burst_time=%d\n", burst_time);
    printf("burst_particles=%d\n", burst_particles);
    printf("max_velocity=%f\n", max_velocity);
    printf("min_start_velocity_x=%f\n", min_start_velocity_x);
    printf("max_start_velocity_x=%f\n", max_start_velocity_x);
    printf("min_start_velocity_y=%f\n", min_start_velocity_y);
    printf("max_start_velocity_y=%f\n", max_start_velocity_y);
    printf("min_acceleration=%f\n", min_acceleration);
    printf("max_acceleration=%f\n", max_acceleration);
}

void remove_emitter() {
    printf("remove_emitter\n");
}

void update_emitter() {
    printf("update_emitter\n");
}

int main() {
    printf("Hello World\n");
}
