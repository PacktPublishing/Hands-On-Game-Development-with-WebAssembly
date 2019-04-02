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

class PlayerShip {
    private: 
        const char* c_SpriteFile = "sprites/Franchise.png";
        const Uint32 c_MinLaunchTime = 300;
        const int c_Width = 16;
        const int c_Height = 16;

        Uint32 m_LastLaunchTime;
        SDL_Texture *m_SpriteTexture;
    public:
        double m_X;
        double m_Y;
        double m_Rotation;
        double m_DX;
        double m_DY;
        double m_VX;
        double m_VY;

        PlayerShip();

        void RotateLeft();
        void RotateRight();
        void Accelerate();
        void Decelerate();
        void CapVelocity();
        void Move();
        void Render();
};

enum FSM_STUB {
    SHOOT = 0,
    TURN_LEFT = 1,
    TURN_RIGHT = 2,
    ACCELERATE = 3,
    DECELERATE = 4
};

class EnemyShip {
    private: 
        const char* c_SpriteFile = "sprites/BirdOfAnger.png";
        const Uint32 c_MinLaunchTime = 300;
        const int c_Width = 16;
        const int c_Height = 16;
        const int c_AIStateTime = 2000;

        Uint32 m_LastLaunchTime;
        SDL_Texture *m_SpriteTexture;
    public:
        FSM_STUB m_AIState;
        int m_AIStateTTL;
        double m_X;
        double m_Y;
        double m_Rotation;
        double m_DX;
        double m_DY;
        double m_VX;
        double m_VY;

        EnemyShip();

        void RotateLeft();
        void RotateRight();
        void Accelerate();
        void Decelerate();
        void CapVelocity();
        void Move();
        void Render();
        void AIStub();
};


class Projectile {
    private:
        const char* c_SpriteFile = "sprites/Projectile.png";
        const int c_Width = 8;
        const int c_Height = 8;
        SDL_Texture *m_SpriteTexture;

    public:
        bool m_Active;
        const double velocity = 6.0;
        const double alive_time = 2000;
        double m_TTL;
        double m_X;
        double m_Y;
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
};

extern PlayerShip* player;
extern EnemyShip* enemy;
extern ProjectilePool* projectile_pool;

#endif