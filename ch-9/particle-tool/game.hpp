#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdlib.h>


//#define SPRITE_FILE "sprites/Franchise.png"
#define MAX_VELOCITY 2.0

#define PI 3.14159
#define TWO_PI 6.28318

extern void add_emitter();
extern void remove_emitter();
extern void update_emitter();

extern Uint32 last_time;
extern Uint32 last_frame_time;
extern Uint32 current_time;
extern Uint32 ms_per_frame;

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern SDL_Event event;

extern bool left_key_down;
extern bool right_key_down;
extern bool up_key_down;
extern bool down_key_down;
extern bool space_key_down;

extern double delta_time;
extern int diff_time;

class Point {
    public:
        double x;
        double y;
        Point();
        Point( double X, double Y );
        void Rotate( double radians );
        Point operator=(const Point& p);
};

class Particle {
    private:
        SDL_Texture *m_SpriteTexture;
        SDL_Rect src = {.x = 0, .y = 0, .w = 0, .h = 0 };
        SDL_Rect dest;


    public:
        bool m_Active;
        int m_X;
        int m_Y;
        Uint32 m_Width;
        Uint32 m_Height;
        Uint32 m_TTL;
        Uint32 m_LifeTime;
        Uint32 m_FrameCount;
        Uint32 m_CurrentFrame;
        Uint32 m_NextFrameTime;

        double m_VX;
        double m_VY;
        double m_Rotation;
        double m_RotationVelocity;
        double m_RotationAcceleration;
        double m_Alpha;
        double m_AlphaFadeRate;

        double m_StartingVX;
        double m_StartingVY;
        double m_Acceleration;
        double m_MaxVelocity;

        Particle(
            SDL_Texture *sprite_texture,

            Uint32 frame_count,
            Uint32 width,
            Uint32 height,

            Uint32 life_time,
            double max_velocity );

        void Spawn( int x,
                    int y,

                    double starting_rotation,
                    double rotation_velocity,
                    double rotation_acceleration,

                    double alpha,
                    double alpha_fade_rate,

                    double starting_vx,
                    double starting_vy,

                    double acceleration );

        void Move();
        void Render();
        void Stop();
};

enum EMITTER_SHAPE {
    POINT = 0,
    CIRCLE = 1,
    RECT = 2,
    DONUT = 3
};

class Emitter {
    private:
        SDL_Texture *m_SpriteTexture;
        std::vector<Particle*> m_Pool; 
        Point m_TempPoint;
        void EmitParticle();
        Point GetParticlePosition();

        double m_CircleEmitRadius;

        double m_RectEmitWidth;
        double m_RectEmitHeight;

        double m_DonutEmitInnerRadius;
        double m_DonutEmitOuterRadius;

    public:
        bool m_Active;
        bool m_Looping;
        bool m_Flip;
        bool m_Animated;
        EMITTER_SHAPE m_Shape;
        Uint32 m_RunTimeRemaining;
        Uint32 m_NextEmissionTime;
        Uint32 m_Duration;

        Uint32 m_MinParticleLifetime;
        Uint32 m_MaxParticleLifetime;
        Uint32 m_LifetimeDiff;

        Uint32 m_MaxParticles;

        Uint32 m_EmissionTimeMS;
        Uint32 m_EmissionTimeMSVariance;

        Uint32 m_BurstTime;
        Uint32 m_BurstParticles;

        double m_X;
        double m_Y;

        double m_MinStartVelocityX;
        double m_MaxStartVelocityX;

        double m_MinStartVelocityY;
        double m_MaxStartVelocityY;

        double m_MinAcceleration;
        double m_MaxAcceleration;

        double m_MinRotation;
        double m_MaxRotation;
        double m_RotationRange;

        double m_MinRotationVelocity;
        double m_MaxRotationVelocity;
        double m_RotationVelocityRange;

        Emitter(
            Uint32 max_particles,

            bool looping,
            bool flip,
            Uint32 duration,

            Uint32 min_particle_lifetime,
            Uint32 max_particle_lifetime,

            Uint32 emission_rate,
            Uint32 emission_rate_variance,

            Uint32 burst_time,
            Uint32 burst_particles,

            double min_start_velocity_x,
            double max_start_velocity_x,

            double min_start_velocity_y,
            double max_start_velocity_y,

            double min_acceleration,
            double max_acceleration,

            double min_rotation_range,
            double max_rotation_range,

            double min_rotation_velocity,
            double max_rotation_velocity,
            double rotation_acceleration
        );
        void Start();
        void Stop();
        void Move();

        void SetEmitShapePoint();
        void SetEmitShapeCircle( double radius );
        void SetEmitShapeRect( double width, double height );
        void SetEmitShapeDonut( double inner_radius, double outer_radius );
};

class ParticleSystem {
    private:
        bool m_Active;
    public:
        std::vector<Emitter*> m_EmitterList;

        ParticleSystem();
        void Start();
        void Stop();
        void Move();

        void AddEmitter( Emitter* emitter );
};

#endif