#ifndef __GAME_H__
#define __GAME_H__

#define TWO_PI 6.2831853

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string>
#include <vector>
#include <cstdlib>

extern SDL_Renderer *renderer;

//extern Uint32 ms_per_frame;
extern Uint32 last_time;
extern Uint32 current_time;
extern Uint32 diff_time;
extern float delta_time;
extern float get_random_float( float min, float max );

class Point {
    public:
        float x;
        float y;
        Point();
        Point( float X, float Y );
        Point operator=(const Point& p);
        void Rotate( float radians );
};

class Particle {
    public:
        bool m_active;
        bool m_alpha_fade;
        bool m_color_mod;

        bool m_align_rotation;
        float m_rotation;

        Uint8 m_start_red;
        Uint8 m_start_green;
        Uint8 m_start_blue;
        
        Uint8 m_end_red;
        Uint8 m_end_green;
        Uint8 m_end_blue;
        
        Uint8 m_current_red;
        Uint8 m_current_green;
        Uint8 m_current_blue;

        SDL_Texture *m_sprite_texture;
        int m_ttl;

        Uint32 m_life_time;
        Uint32 m_animation_frames;
        Uint32 m_current_frame;

        Uint32 m_next_frame_ms;

        float m_acceleration;
        float m_alpha;

        float m_width;
        float m_height;

        float m_start_scale;
        float m_end_scale;
        float m_current_scale;

        Point m_position;
        Point m_velocity;
        SDL_Rect m_dest = {.x = 0, .y = 0, .w = 0, .h = 0 };
        SDL_Rect m_src = {.x = 0, .y = 0, .w = 0, .h = 0 };

        Particle( SDL_Texture *sprite, Uint32 life_time, float acceleration,
                    bool alpha_fade, int width, int height, bool align_rotation,
                    Uint32 start_color,
                    Uint32 end_color,
                    Uint32 animation_frames );

        void Update( Uint32 life_time, float acceleration,
                    bool alpha_fade, bool align_rotation,
                    Uint32 start_color, Uint32 end_color,
                    Uint32 m_animation_frames );

        void Spawn( float x, float y, float velocity_x, float velocity_y,
                    float start_scale, float end_scale, float rotation );
        void Move();
        void Render();
};

class Emitter {
    public:
        bool m_loop;
        bool m_align_rotation;
        bool m_active;
        bool m_has_burst;
        SDL_Texture *m_sprite_texture;
        std::vector<Particle*> m_particle_pool; 
        int m_sprite_width;
        int m_sprite_height;
        int m_ttl;
        Uint32 m_max_particles;
        Uint32 m_emission_rate;
        Uint32 m_emission_time_ms;

        Uint32 m_start_color;
        Uint32 m_end_color;

        Uint32 m_burst_particles;
        Uint32 m_emit_loop_ms;
        Uint32 m_animation_frames;

        int m_next_emission;
        float m_max_angle;
        float m_min_angle;
        float m_radius;
        float m_min_starting_velocity;
        float m_max_starting_velocity;

        float m_min_start_scale;
        float m_max_start_scale;
        float m_min_end_scale;
        float m_max_end_scale;
        float m_min_start_rotation;
        float m_max_start_rotation;
        float m_burst_time_pct;

        Point m_position;


        Emitter(char* sprite_file, int max_particles, float min_angle, float max_angle,
                    Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    float min_starting_velocity, float max_starting_velocity,
                    Uint32 emission_rate, int x_pos, int y_pos, float radius,
                    /* new parameters here */
                    float min_start_scale, float max_start_scale,
                    float min_end_scale, float max_end_scale,
                    Uint32 start_color, Uint32 end_color,
                    float burst_time_pct, Uint32 burst_particles, 
                    bool loop, bool align_rotation, 
                    Uint32 emit_time_ms, Uint32 animation_frames );

        void Update(int max_particles, float min_angle, float max_angle,
                    Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    float min_starting_velocity, float max_starting_velocity,
                    Uint32 emission_rate, int x_pos, int y_pos, float radius,
                    /* new parameters here */
                    float min_start_scale, float max_start_scale,
                    float min_end_scale, float max_end_scale,
                    Uint32 start_color, Uint32 end_color,
                    float burst_time_pct, Uint32 burst_particles, 
                    bool loop, bool align_rotation, Uint32 emit_time_ms, 
                    Uint32 animation_frames );

        void Move();
        Particle* GetFreeParticle();
};

#endif