#include "game.hpp"

Particle::Particle( SDL_Texture *sprite_texture, Uint32 life_time, float acceleration, 
                    bool alpha_fade, int width, int height, bool align_rotation,
                    Uint32 start_color, Uint32 end_color, Uint32 animation_frames ) {

    if( start_color != 0xffffff || end_color != 0xffffff ) {

        m_color_mod = true;

        m_start_red = (Uint8)(start_color >> 16);
        m_start_green = (Uint8)(start_color >> 8);
        m_start_blue = (Uint8)(start_color);
        
        m_end_red = (Uint8)(end_color >> 16);
        m_end_green = (Uint8)(end_color >> 8);
        m_end_blue = (Uint8)(end_color);
        
        m_current_red = m_start_red;
        m_current_green = m_start_green;
        m_current_blue = m_start_blue;

    }
    else {
        m_color_mod = false;

        m_start_red = (Uint8)255;
        m_start_green = (Uint8)255;
        m_start_blue = (Uint8)255;
        
        m_end_red = (Uint8)255;
        m_end_green = (Uint8)255;
        m_end_blue = (Uint8)255;
        
        m_current_red = m_start_red;
        m_current_green = m_start_green;
        m_current_blue = m_start_blue;

    }

    m_align_rotation = align_rotation;

    m_animation_frames = animation_frames;
    m_sprite_texture = sprite_texture;
    m_life_time = life_time;
    m_acceleration = acceleration;
    m_alpha_fade = alpha_fade;

    m_width = (float)width;
    m_height = (float)height;

    m_src.w = m_dest.w = (int)((float)width / (float)m_animation_frames);
    m_src.h = m_dest.h = height; 

    m_current_frame = 0;
    m_next_frame_ms = m_life_time / m_animation_frames;

    m_Active = false;
}

void Particle::Update( Uint32 life_time, float acceleration, bool alpha_fade, bool align_rotation,
                    Uint32 start_color, Uint32 end_color, Uint32 animation_frames ) {

    if( start_color != 0xffffff || end_color != 0xffffff ) {

        m_color_mod = true;

        m_start_red = (Uint8)(start_color >> 16);
        m_start_green = (Uint8)(start_color >> 8);
        m_start_blue = (Uint8)(start_color);
        
        m_end_red = (Uint8)(end_color >> 16);
        m_end_green = (Uint8)(end_color >> 8);
        m_end_blue = (Uint8)(end_color);
        
        m_current_red = m_start_red;
        m_current_green = m_start_green;
        m_current_blue = m_start_blue;

    }
    else {
        m_color_mod = false;

        m_start_red = (Uint8)255;
        m_start_green = (Uint8)255;
        m_start_blue = (Uint8)255;
        
        m_end_red = (Uint8)255;
        m_end_green = (Uint8)255;
        m_end_blue = (Uint8)255;
        
        m_current_red = m_start_red;
        m_current_green = m_start_green;
        m_current_blue = m_start_blue;

    }

    m_align_rotation = align_rotation;

    m_life_time = life_time;
    m_acceleration = acceleration;
    m_alpha_fade = alpha_fade;
    m_Active = false;

    m_current_frame = 0;
    m_animation_frames = animation_frames;
    m_next_frame_ms = m_life_time / m_animation_frames;

    m_src.w = m_dest.w = (int)((float)m_width / (float)m_animation_frames);
    m_src.h = m_dest.h = m_height; 
}

void Particle::Spawn( float x, float y, 
                    float velocity_x, float velocity_y, 
                    float start_scale, float end_scale,
                    float rotation ) {
    m_Position.x = x;
    m_dest.x = m_Position.x;
    m_Position.y = y;
    m_dest.y = m_Position.y;

    m_Velocity.x = velocity_x;
    m_Velocity.y = velocity_y;

    m_alpha = 255.0;
    m_Active = true;
    m_ttl = m_life_time;
    m_rotation = rotation;

    m_current_red = m_start_red;
    m_current_green = m_start_green;
    m_current_blue = m_start_blue;

    m_current_scale = m_start_scale = start_scale;
    m_end_scale = end_scale;

    m_current_frame = 0;
    m_next_frame_ms  = m_life_time / m_animation_frames;

}

void Particle::Move() {
    
    float time_pct = 1.0 - (float)m_ttl / (float)m_life_time;

    m_current_frame = (int)(time_pct * (float)m_animation_frames);
    

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

    m_Velocity.x *= acc_adjusted;
    m_Velocity.y *= acc_adjusted;
    

    m_Position.x += m_Velocity.x * delta_time;
    m_Position.y += m_Velocity.y * delta_time;

    m_dest.x = (int)m_Position.x; 
    m_dest.y = (int)m_Position.y; 

    if( m_alpha_fade == true ) {
        m_alpha = 255.0 * (1.0 - time_pct);
        if( m_alpha < 0 ) {
            m_alpha = 0;
        }
    }
    else {
        m_alpha = 255.0;
    }

    if( m_color_mod == true ) {
        m_current_red = m_start_red + (Uint8)(( m_end_red - m_start_red ) * time_pct);
        m_current_green = m_start_green + (Uint8)(( m_end_green - m_start_green ) * time_pct);
        m_current_blue = m_start_blue + (Uint8)(( m_end_blue - m_start_blue ) * time_pct);
    }

    m_current_scale = m_start_scale + (m_end_scale - m_start_scale) * time_pct;

    m_dest.w = (int)(m_src.w * m_current_scale);
    m_dest.h = (int)(m_src.h * m_current_scale);

    m_ttl -= diff_time;
    if( m_ttl <= 0 ) {
        m_Active = false;
    }
    else {
        m_src.x = (int)(m_src.w * m_current_frame);
    }
}

void Particle::Render() {
    render_manager->Render( m_sprite_texture, &m_src, &m_dest, m_rotation,
                    m_alpha, m_current_red, m_current_green, m_current_blue );

}