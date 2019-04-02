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

#define DEG_TO_RAD(deg) ((float)deg/180.0)*3.14159
#define RAD_TO_DEG(rad) ((float)rad*180.0)/3.14159

//#define SPRITE_FILE "sprites/Franchise.png"
#define PI 3.14159
#define TWO_PI 6.28318

#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 600

#define LEVEL_WIDTH CANVAS_WIDTH*4
#define LEVEL_HEIGHT CANVAS_HEIGHT*4

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
extern bool f_key_down;

extern float delta_time;
extern int diff_time;

extern float get_random_float( float min, float max );

class Ship;
class Particle;
class Emitter;
class Collider;
class Asteroid;
class Star;
class PlayerShip;
class EnemyShip;
class Projectile;
class ProjectilePool;
class FiniteStateMachine;
class Camera;
class RenderManager;
class Locator;

enum FSM_STATE {
    APPROACH = 0,
    ATTACK = 1,
    FLEE = 2,
    WANDER = 3
};

class Vector2D {
    public:
        float x;
        float y;
        Vector2D();
        Vector2D( float X, float Y );
        void Rotate( float radians );
        void Normalize();

        float MagSQ();
        float Magnitude();
        Vector2D Project( Vector2D &onto );
        float Dot(Vector2D &vec);
        float FindAngle();

        Vector2D operator=(const Vector2D &vec);
        Vector2D operator*(const float &scalar);
        void operator+=(const Vector2D &vec);
        void operator-=(const Vector2D &vec);
        void operator*=(const float &scalar);
        void operator/=(const float &scalar);
        Vector2D operator-(const Vector2D &vec);
        Vector2D operator+(const Vector2D &vec);
};

class Locator {
    public:
        bool m_Active = false;
        bool m_LastActive = false;
        SDL_Texture *m_SpriteTexture;
        SDL_Rect m_dest = {.x = 0, .y = 0, .w = 32, .h = 32 };

        Vector2D m_Position;
        int m_ColorFlux;
        float m_Rotation;

        Locator();
        void SetActive();
        void Move();
        void Render();
};

class Camera {
    public:
        Vector2D m_Position;
        Vector2D m_DesiredPosition;

        float m_HalfWidth;
        float m_HalfHeight;
        
        Camera( float width, float height );

        void Move();

};

class RenderManager {
    public:
        const int c_BackgroundWidth = 800;
        const int c_BackgroundHeight = 600;

        SDL_Texture *m_BackgroundTexture;
        SDL_Rect m_BackgroundDest = {.x = 0, .y = 0, .w = c_BackgroundWidth, .h = c_BackgroundHeight };

        RenderManager();

        void RenderBackground();

        void Render( SDL_Texture *tex, SDL_Rect *src, SDL_Rect *dest, float rad_rotation = 0.0,
                        int alpha = 255, int red = 255, int green = 255, int blue = 255 );

        void RenderUI( SDL_Texture *tex, SDL_Rect *src, SDL_Rect *dest, float rad_rotation = 0.0,
                        int alpha = 255, int red = 255, int green = 255, int blue = 255 );
};

class Range {
    public:
        float min;
        float max;

        Range();
        Range( float min_val, float max_val );
        void operator+=(const Range& range);
        Range operator+(const Range& range);
        Range operator=(const Range& range);

        bool Overlap( Range &other );
        void Sort();
        void Extend( float ex );
        float Clamp( float value );
};

class Particle {
    public:
        bool m_Active;
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

        Vector2D m_Position;
        Vector2D m_Velocity;
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
        bool m_Active;
        bool m_has_burst;
        SDL_Texture *m_sprite_texture;
        std::vector<Particle*> m_particle_pool; 
        int m_sprite_width;
        int m_sprite_height;
        int m_ttl;

        int m_x_adjustment = 0;
        int m_y_adjustment = 0;

        Uint32 m_max_particles;
        Uint32 m_emission_rate;
        Uint32 m_emission_time_ms;

        Uint32 m_start_color;
        Uint32 m_end_color;

        Uint32 m_burst_particles;
        Uint32 m_emit_loop_ms;
        Uint32 m_animation_frames;

        int m_next_emission;

        float* m_parent_rotation;
        
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

        float* m_parent_rotation_ptr;
        float* m_parent_x_ptr;
        float* m_parent_y_ptr;

        Vector2D m_position;


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

        void Run();  
};

class Collider {
    public:
        bool m_Active;
        float* m_ParentRotation;
        float* m_ParentX;
        float* m_ParentY;
        Vector2D m_TempPoint;
        bool CCHitTest( Collider* collider );

        Vector2D m_Position;

        float m_Radius;
        float m_SteeringRadius;
        float m_SteeringRadiusSQ;

        void SetParentInformation( float* rotation, float* x, float* y );

        Collider(float radius);

        bool HitTest( Collider *collider );
        bool SteeringLineTest( Vector2D &p1, Vector2D &p2 );
        bool SteeringRectTest( Vector2D &start_point, Vector2D &end_point );
        void WrapPosition();
};

class Shield : public Collider {
    public:
    int m_ttl;
    int m_NextFrame;
    Uint32 m_CurrentFrame;
    Ship* m_Ship;
    SDL_Texture *m_SpriteTexture;

    SDL_Rect m_src =  {.x = 0, .y = 0, .w = 32, .h = 32 };
    SDL_Rect m_dest = {.x = 0, .y = 0, .w = 32, .h = 32 };

    Shield( Ship* ship, const char* sprite_file );

    void Move();
    void Render();
    bool Activate();
    void Deactivate();
};

class Asteroid : public Collider {
    public:
        SDL_Texture *m_SpriteTexture;
        SDL_Rect m_src = {.x = 0, .y = 0, .w = 16, .h = 16 };
        SDL_Rect m_dest = {.x = 0, .y = 0, .w = 0, .h = 0 };

        Uint32 m_CurrentFrame = 0;
        int m_NextFrameTime;
        float m_Rotation;

        Vector2D m_Direction;
        Vector2D m_Velocity;
        
        Emitter* m_Explode;
        Emitter* m_Chunks;

        Asteroid( float x,  float y, 
                  float velocity, 
                  float rotation );

        void Move();
        void Render();
        void Explode();
};

class Star : public Collider {
    public:
        SDL_Texture *m_SpriteTexture;
        SDL_Rect m_src = {.x = 0, .y = 0, .w = 64, .h = 64 };
        SDL_Rect m_dest = {.x = 0, .y = 0, .w = 64, .h = 64 };

        std::vector<Emitter*> m_FlareList;

        Uint32 m_CurrentFrame = 0;
        int m_NextFrameTime;

        Star();
                    
        void Move();
        void Render();
};


class Ship : public Collider {
    public:
        const float c_Acceleration = 10.0f;
        const float c_MaxVelocity = 100.0f;
        const int c_AliveTime = 2000;
        const Uint32 c_MinLaunchTime = 300;

        bool m_Accelerating = false;
        Uint32 m_LastLaunchTime;
        const int c_Width = 32;
        const int c_Height = 32;
        SDL_Texture *m_SpriteTexture;
        SDL_Rect src = {.x = 0, .y = 0, .w = 32, .h = 32 };

        Emitter* m_Explode;
        Emitter* m_Exhaust;
        Shield* m_Shield;
        std::vector<Collider*> m_Colliders; 

        Uint32 m_CurrentFrame = 0;
        int m_NextFrameTime;
        float m_Rotation;

        Vector2D m_Direction;
        Vector2D m_Velocity;

        void RotateLeft();
        void RotateRight();
        void Accelerate();
        void Decelerate();
        void CapVelocity();
        void Shoot(); 
        virtual void Move() = 0;
        Ship();
        void Render();
        bool CompoundHitTest( Collider* collider );
};

class PlayerShip: public Ship {
    private: 
        const char* c_SpriteFile = "sprites/FranchiseExp.png";
        const char* c_ShieldSpriteFile = "/sprites/shield-franchise.png";

    public:

        PlayerShip();
        void Move();
};

class EnemyShip: public Ship {
    public:
        const char* c_SpriteFile = "/sprites/BirdOfAngerExp.png";
        const char* c_ShieldSpriteFile = "/sprites/shield-bird.png";
        const int c_AIStateTime = 2000;

        int m_AIStateTTL;

        FiniteStateMachine* m_FSM;

        EnemyShip();
        void Move();
};

class FiniteStateMachine {
    public:
        const float c_AttackDistSq = 40000.0;
        const float c_FleeDistSq = 2500.0;
        const int c_MinRandomTurnMS = 100;
        const int c_RandTurnMS = 3000;
        const int c_ShieldDist = 20;
        const int c_AvoidDist = 80;
        const int c_StarAvoidDistSQ = 20000;
        const int c_ObstacleAvoidForce = 150;
        const int c_StarAvoidForce = 120;

        FSM_STATE m_CurrentState;
        EnemyShip* m_Ship;
        bool m_HasLOS;
        bool m_LastTurnLeft;
        int m_SameTurnPct;
        int m_NextTurnMS;
        int m_CheckCycle;
        float m_DesiredRotation;
        float m_PlayerDistSQ;

        FiniteStateMachine(EnemyShip* ship);
        
        void SeekState(Vector2D &seek_point);
        void FleeState(Vector2D &flee_point);
        void WanderState();  
        void AttackState();

        void AvoidForce();
        bool ShieldCheck();
        bool LOSCheck(); 

        Vector2D PredictPosition();

        float GetPlayerDistSq();

        void Move();
};


class Projectile: public Collider {
    public:
        const char* c_SpriteFile = "sprites/ProjectileExp.png";
        const int c_Width = 16;
        const int c_Height = 16;
        SDL_Texture *m_SpriteTexture;
        SDL_Rect src = {.x = 0, .y = 0, .w = 16, .h = 16 };

        Uint32 m_CurrentFrame = 0;
        int m_NextFrameTime;
        const float c_Velocity = 300.0;
        const float c_AliveTime = 2000;
        float m_TTL;

        Vector2D m_Velocity;

        Projectile();
        void Move();
        void Render();
        void Launch(Vector2D &position, Vector2D &direction);
};

class ProjectilePool {
    public:
        std::vector<Projectile*> m_ProjectileList;
        ProjectilePool();
        ~ProjectilePool();

        void MoveProjectiles();
        void RenderProjectiles();
        Projectile* GetFreeProjectile();
        bool CollisionCheck();
};

extern PlayerShip* player;
extern EnemyShip* enemy;
extern Camera* camera;
extern RenderManager* render_manager;
extern Locator* locator;

extern ProjectilePool* projectile_pool;
extern std::vector<Asteroid*> asteroid_list; 

extern Star* star;

#endif