#include "game.hpp"


Projectile::Projectile(): Collider(4.0) {
    m_Active = false;
    
    SDL_Surface *temp_surface = IMG_Load( c_SpriteFile );
    
    if( !temp_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return;
    }

    m_SpriteTexture = SDL_CreateTextureFromSurface( renderer, temp_surface );
    
    if( !m_SpriteTexture ) {
        printf("failed to create texture: %s\n", IMG_GetError() );
        return;
    }

    SDL_FreeSurface( temp_surface );

}

void Projectile::Move() {

    if( m_CurrentFrame > 0 ) {
        m_NextFrameTime -= diff_time;

        if( m_NextFrameTime <= 0 ) {
            ++m_CurrentFrame;
            m_NextFrameTime = ms_per_frame;
            if( m_CurrentFrame >= 4 ) {
                m_Active = false;
                m_CurrentFrame = 0;
                return;
            }
        }
        return;
    }

    m_Position += m_Velocity * delta_time;

    WrapPosition();

    m_TTL -= diff_time;

    if( m_TTL < 0 ) {
        m_Active = false;
        m_TTL = 0;
    }
}

void Projectile::Render() {
    dest.x = m_Position.x + 8;
    dest.y = m_Position.y + 8;
    dest.w = c_Width;
    dest.h = c_Height;

    src.x = 16 * m_CurrentFrame;

    render_manager->Render( m_SpriteTexture, &src, &dest );
}

void Projectile::Launch(Vector2D &position, Vector2D &direction) {
    m_Position = position;
    Vector2D offset = direction;
    offset *= 18;

    m_Position += offset;

    m_Velocity = direction;
    m_Velocity *= c_Velocity;

    m_TTL = c_AliveTime;
    m_Active = true;
}
