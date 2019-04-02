#include "game.hpp"

Particle::Particle( SDL_Texture *sprite_texture, Uint32 life_time, float acceleration, 
                    bool alpha_fade, int width, int height ) {
    m_sprite_texture = sprite_texture;
    m_life_time = life_time;
    m_acceleration = acceleration;
    m_alpha_fade = alpha_fade;
    m_dest.w = width;
    m_dest.h = height;
    m_active = false;
}

void Particle::Update( Uint32 life_time, float acceleration, bool alpha_fade ) {
    m_life_time = life_time;
    m_acceleration = acceleration;
    m_alpha_fade = alpha_fade;
    m_active = false;
}

void Particle::Spawn( float x, float y, float velocity_x, float velocity_y, float alpha ) {
    m_position.x = x;
    m_dest.x = (int)m_position.x;
    m_position.y = y;
    m_dest.y = (int)m_position.y;

    m_velocity.x = velocity_x;
    m_velocity.y = velocity_y;

    m_alpha = alpha;
    m_active = true;
    m_ttl = m_life_time;
}

void Particle::Move() {
    
    float acc_adjusted = 1.0f;
    if( m_acceleration < 1.0f ) {
        acc_adjusted = 1.0f - m_acceleration;
        acc_adjusted *= delta_time;
        acc_adjusted = 1.0f - acc_adjusted;
    }
    else if( m_acceleration > 1.0f ) {
        acc_adjusted = m_acceleration - 1.0f;
        acc_adjusted *= delta_time;
        acc_adjusted += 1.0f;
    }

    m_velocity.x *= acc_adjusted;
    m_velocity.y *= acc_adjusted;
    

    m_position.x += m_velocity.x;
    m_position.y += m_velocity.y;

    m_dest.x = (int)m_position.x;
    m_dest.y = (int)m_position.y;

    if( m_alpha_fade == true ) {
        m_alpha = 255.0 * (float)m_ttl / (float)m_life_time;
        if( m_alpha < 0 ) {
            m_alpha = 0;
        }
    }
    else {
        m_alpha = 255.0;
    }

    m_ttl -= diff_time;
    if( m_ttl <= 0 ) {
        m_active = false;
    }
}

void Particle::Render() {
    SDL_SetTextureAlphaMod(m_sprite_texture,
                              (Uint8)m_alpha );
    SDL_RenderCopy( renderer, m_sprite_texture, NULL, &m_dest );
}