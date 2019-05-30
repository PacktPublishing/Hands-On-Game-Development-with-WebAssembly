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

//#define SPRITE_FILE "sprites/Franchise.png"
#define MAX_VELOCITY 2.0

#define PI 3.14159
#define TWO_PI 6.28318

extern Uint32 last_time;
extern Uint32 last_frame_time;
extern Uint32 current_time;
extern Uint32 ms_per_frame;

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern SDL_Rect dest;

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
        float x;
        float y;
        Point();
        Point( float X, float Y );
        void Rotate( float radians );
        Point operator=(const Point& p);
};

extern float get_random_float( float min, float max );

class Particle {
    public:
        bool m_active;
        bool m_alpha_fade;
        bool m_color_mod;

        bool m_align_rotation;

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

        float m_rotation;

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

        // added ----------------------------
        int m_x_adjustment = 0;
        int m_y_adjustment = 0;
        // ----------------------------------

        Uint32 m_max_particles;
        Uint32 m_emission_rate;
        Uint32 m_emission_time_ms;

        Uint32 m_start_color;
        Uint32 m_end_color;

        Uint32 m_burst_particles;
        Uint32 m_emit_loop_ms;
        Uint32 m_animation_frames;

        int m_next_emission;

        double* m_parent_rotation;
        
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
        // added ----------------------------
        double* m_parent_rotation_ptr;
        double* m_parent_x_ptr;
        double* m_parent_y_ptr;
        // -----------------------------------

        Point m_position;


        Emitter(char* sprite_file, int max_particles, float min_angle, float max_angle,
                    Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    float min_starting_velocity, float max_starting_velocity,
                    Uint32 emission_rate, int x_pos, int y_pos, float radius,
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
                    float min_start_scale, float max_start_scale,
                    float min_end_scale, float max_end_scale,
                    Uint32 start_color, Uint32 end_color,
                    float burst_time_pct, Uint32 burst_particles, 
                    bool loop, bool align_rotation, Uint32 emit_time_ms, 
                    Uint32 animation_frames );

        void Move();
        Particle* GetFreeParticle();

        void Run();  // added
};

class Collider {
    protected:
        double* m_ParentRotation;
        double* m_ParentX;
        double* m_ParentY;
        Point m_TempPoint;
        bool CCHitTest( Collider* collider );
    public:
        double m_X;
        double m_Y;
        double m_Radius;

        void SetParentInformation( double* rotation, double* x, double* y );

        Collider(double radius);

        bool HitTest( Collider *collider );
};


class Ship : public Collider {
    protected:
        bool m_Accelerating = false;
        Uint32 m_LastLaunchTime;
        const int c_Width = 32;
        const int c_Height = 32;
        SDL_Texture *m_SpriteTexture;
        SDL_Rect src = {.x = 0, .y = 0, .w = 32, .h = 32 };

    public:
        Emitter* m_Explode;
        Emitter* m_Exhaust;
        std::vector<Collider*> m_Colliders; 
        bool m_Alive = true;        
        Uint32 m_CurrentFrame = 0;
        int m_NextFrameTime;
        double m_Rotation;
        double m_DX;
        double m_DY;
        double m_VX;
        double m_VY;

        void RotateLeft();
        void RotateRight();
        void Accelerate();
        void Decelerate();
        void CapVelocity();
        virtual void Move() = 0;
        Ship();
        void Render();
        bool CompoundHitTest( Collider* collider );
};

class PlayerShip: public Ship {
    private: 
        const char* c_SpriteFile = "sprites/FranchiseExp.png";
        const Uint32 c_MinLaunchTime = 300;

    public:

        PlayerShip();
        void Move();
};

enum FSM_STUB {
    SHOOT = 0,
    TURN_LEFT = 1,
    TURN_RIGHT = 2,
    ACCELERATE = 3,
    DECELERATE = 4
};

class EnemyShip: public Ship {
    private: 
        const char* c_SpriteFile = "sprites/BirdOfAngerExp.png";
        const Uint32 c_MinLaunchTime = 300;
        const int c_AIStateTime = 2000;

    public:
        FSM_STUB m_AIState;
        int m_AIStateTTL;

        EnemyShip();
        void AIStub();
        void Move();
};


class Projectile: public Collider {
    private:
        const char* c_SpriteFile = "sprites/ProjectileExp.png";
        const int c_Width = 16;
        const int c_Height = 16;
        SDL_Texture *m_SpriteTexture;
        SDL_Rect src = {.x = 0, .y = 0, .w = 16, .h = 16 };

    public:
        Uint32 m_CurrentFrame = 0;
        int m_NextFrameTime;
        bool m_Active;
        const double velocity = 6.0;
        const double alive_time = 2000;
        double m_TTL;
        double m_VX;
        double m_VY;

        Projectile();
        void Move();
        void Render();
        void Launch(double x, double y, double dx, double dy);
};

class ProjectilePool {
    private:
        std::vector<Projectile*> m_ProjectileList;
    public:
        ProjectilePool();
        ~ProjectilePool();

        void MoveProjectiles();
        void RenderProjectiles();
        Projectile* GetFreeProjectile();
        bool CollisionCheck();
};

extern PlayerShip* player;
extern EnemyShip* enemy;
extern ProjectilePool* projectile_pool;



#endif