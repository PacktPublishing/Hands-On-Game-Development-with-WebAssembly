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

extern Uint32 last_time;
extern Uint32 current_time;
extern Uint32 diff_time;
extern float delta_time;

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
        SDL_Texture *m_sprite_texture;
        int m_ttl;
        Uint32 m_life_time;
        float m_acceleration;
        float m_alpha;
        Point m_position;
        Point m_velocity;
        SDL_Rect m_dest = {.x = 0, .y = 0, .w = 0, .h = 0 };

        Particle( SDL_Texture *sprite, Uint32 life_time, float acceleration,
                     bool alpha_fade, int width, int height );
        void Update( Uint32 life_time, float acceleration,
                     bool alpha_fade );
        void Spawn( float x, float y, float velocity_x, float velocity_y, float alpha );
        void Move();
        void Render();
};

class Emitter {
    public:
        SDL_Texture *m_sprite_texture;
        std::vector<Particle*> m_particle_pool; 
        int m_sprite_width;
        int m_sprite_height;
        Uint32 m_max_particles;
        Uint32 m_emission_rate;
        Uint32 m_emission_time_ms;
        int m_next_emission;
        float m_max_angle;
        float m_min_angle;
        float m_radius;
        float m_min_starting_velocity;
        float m_max_starting_velocity;
        Point m_position;

        Emitter(char* sprite_file, int max_particles, float min_angle, float max_angle,
                    Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    float min_starting_velocity, float max_starting_velocity,
                    Uint32 emission_rate, int x_pos, int y_pos, float radius );

        void Update(int max_particles, float min_angle, float max_angle,
                    Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    float min_starting_velocity, float max_starting_velocity,
                    Uint32 emission_rate, int x_pos, int y_pos, float radius );

        void Move();
        Particle* GetFreeParticle();
};

#endif