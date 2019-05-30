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

class Collider {
    protected:
        double* m_ParentRotation;
        double* m_ParentX;
        double* m_ParentY;
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
        Uint32 m_LastLaunchTime;
        const int c_Width = 32;
        const int c_Height = 32;
        SDL_Texture *m_SpriteTexture;
        SDL_Rect src = {.x = 0, .y = 0, .w = 32, .h = 32 };

    public:
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