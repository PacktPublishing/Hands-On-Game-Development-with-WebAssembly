#include "game.hpp"
#define ADSTEROID_SPRITE_FILE (char*)"/sprites/asteroid.png"

Asteroid::Asteroid( float x, float y, 
                    float velocity, 
                    float rotation ): Collider(8.0) {

    SDL_Surface *temp_surface = IMG_Load( ADSTEROID_SPRITE_FILE );
    
    if( !temp_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return;
    }
    else { 
        printf("success creating asteroid surface\n");
    }

    m_SpriteTexture = SDL_CreateTextureFromSurface( renderer, temp_surface );

    if( !m_SpriteTexture ) {
        printf("failed to create texture: %s\n", IMG_GetError() );
        return;
    }
    else { 
        printf("success creating asteroid texture\n");
    }

    SDL_FreeSurface( temp_surface );

    m_Explode = new Emitter((char*)"/sprites/Explode.png", 
                    100, 0, 360,        // int max_particles, float min_angle, float max_angle,
                    1000, 0.3, false,   // Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    20.0, 40.0,         // float min_starting_velocity, float max_starting_velocity,
                    10, 0, 0, 5,        // Uint32 emission_rate, int x_pos, int y_pos, float radius,
                    1.0, 2.0,           // float min_start_scale, float max_start_scale,
                    1.0, 2.0,           // float min_end_scale, float max_end_scale,
                    0xffffff, 0xffffff,
                    0.01, 10,            // float burst_time_pct, Uint32 burst_particles, 
                    false, false,       // bool loop, bool align_rotation, 
                    800, 8 );           // Uint32 emit_time_ms, Uint32 animation_frames

    m_Explode->m_parent_rotation_ptr = &m_Rotation;
    m_Explode->m_parent_x_ptr = &(m_Position.x);
    m_Explode->m_parent_y_ptr = &(m_Position.y);
    m_Explode->m_Active = false;

    m_Chunks = new Emitter((char*)"/sprites/small-asteroid.png", 
                    40, 0, 360,             // int max_particles, float min_angle, float max_angle,
                    1000, 0.05, false,      // Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    80.0, 150.0,            // float min_starting_velocity, float max_starting_velocity,
                    5, 0, 0, 10,            // Uint32 emission_rate, int x_pos, int y_pos, float radius,
                    2.0, 2.0,               // float min_start_scale, float max_start_scale,
                    0.25, 0.5,              // float min_end_scale, float max_end_scale,
                    0xffffff, 0xffffff,
                    0.1, 10,                // float burst_time_pct, Uint32 burst_particles, 
                    false, true,            // bool loop, bool align_rotation,
                    1000, 8 );              // Uint32 emit_time_ms, Uint32 animation_frames

    m_Chunks->m_parent_rotation_ptr = &m_Rotation;
    m_Chunks->m_parent_x_ptr = &m_Position.x;
    m_Chunks->m_parent_y_ptr = &m_Position.y;
    m_Chunks->m_Active = false;

    m_Position.x = x;
    m_Position.y = y;

    Vector2D direction;
    direction.x = 1;
    direction.Rotate( rotation );

    m_Direction = direction;
    m_Velocity = m_Direction * velocity;

    m_dest.h = m_src.h = m_dest.w = m_src.w = 16;
    

    m_Rotation = rotation;
    m_Active = true;
    m_CurrentFrame = 0;
    m_NextFrameTime = ms_per_frame;

    m_Mass = 100.0;
}

void Asteroid::Move() {

    m_NextFrameTime -= diff_time;
    if( m_NextFrameTime <= 0 ) {
        m_NextFrameTime = ms_per_frame;
        m_CurrentFrame++;
        if( m_CurrentFrame >= 8 ) {
            m_CurrentFrame = 0;
        }
    }

    m_Position += m_Velocity * delta_time;

    WrapPosition();
}

void Asteroid::Render() {

    m_Explode->Move();
    m_Chunks->Move();

    if( m_Active == false ) {
        return;
    }

    m_src.x = m_dest.w * m_CurrentFrame;
    m_dest.x = m_Position.x + m_Radius / 2;
    m_dest.y = m_Position.y + m_Radius / 2;
    /*
    SDL_RenderCopyEx( renderer, m_SpriteTexture, 
                        &m_src, &m_dest, 
                        RAD_TO_DEG(m_Rotation), NULL, SDL_FLIP_NONE );
                        */
    render_manager->Render( m_SpriteTexture, &m_src, &m_dest, m_Rotation );

}

void Asteroid::Explode() {
    m_Explode->Run();
    m_Chunks->Run();
    m_Active = false;
}

