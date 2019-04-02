/*
    private:
        SDL_Texture *m_SpriteTexture;
        SDL_Rect src = {.x = 0, .y = 0, .w = 0, .h = 0 };
        SDL_Rect dest;


    public:
        int m_X;
        int m_Y;
        Uint32 m_Width;
        Uint32 m_Height;
        Uint32 m_TTL;
        Uint32 m_LifeTime;
        Uint32 m_FrameCount;
        Uint32 m_CurrentFrame;

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
*/

#include "game.hpp"

Particle::Particle(
        SDL_Texture *sprite_texture,

        Uint32 frame_count,
        Uint32 width,
        Uint32 height,

        Uint32 life_time,
        double max_velocity ) {

    m_FrameCount = frame_count;
    m_CurrentFrame = 0;
    m_SpriteTexture = sprite_texture;
    m_Width = src.w = width;
    m_Height = src.h = height;
    m_MaxVelocity = max_velocity;
    m_LifeTime = life_time;
}

void Particle::Spawn(
        int x,
        int y,

        double starting_rotation,
        double rotation_velocity,
        double rotation_acceleration,

        double alpha,
        double alpha_fade_rate,

        double starting_vx,
        double starting_vy,

        double acceleration ) {

    m_Active = true;
    m_X = x;
    m_Y = y;
    m_TTL = m_LifeTime;

    m_StartingVX = m_VX = starting_vx;
    m_StartingVY = m_VY = starting_vy;
    m_Rotation = starting_rotation;
    m_RotationVelocity = rotation_velocity;
    m_RotationAcceleration = rotation_acceleration;
    m_Alpha = alpha;
    m_AlphaFadeRate = alpha_fade_rate;

    m_Acceleration = acceleration;
    m_NextFrameTime = ms_per_frame;
}


void Particle::Move() {
    if( m_Active == false ) {
        return;
    }

    m_TTL -= diff_time;

    if( m_TTL <= 0 ) {
        m_Active = false;
        return;
    }
    else if( m_FrameCount > 1 ) {
        m_NextFrameTime -= diff_time;

        if( m_NextFrameTime <= 0 ) {
            m_NextFrameTime = ms_per_frame;
            if( m_CurrentFrame >= m_FrameCount ) {
                m_Active = false;
                return;
            }
        }

    }

    float acc_diff = 0.0f;
    float acc_adjusted = 1.0f;

    if( m_Acceleration > 1.0f ) {
        acc_diff = m_Acceleration - 1.0f;
        acc_diff *= delta_time;
        acc_adjusted = 1.0f + acc_diff;
    }
    else if( m_Acceleration < 1.0 ) {
        acc_diff = 1.0f - m_Acceleration;
        acc_diff *= delta_time;
        acc_adjusted = 1.0f - acc_diff;
    }

    m_VX *= acc_adjusted;
    m_VY *= acc_adjusted;

    m_X += m_VX;
    m_Y += m_VY;
}

void Particle::Render() {
    dest.x = (int)m_X;
    dest.y = (int)m_Y;

    src.x = 32 * m_CurrentFrame;

    double degrees = (m_Rotation / PI) * 180.0;

}

void Particle::Stop() {
    
}
