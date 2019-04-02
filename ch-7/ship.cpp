#include "game.hpp"

Ship::Ship() : Collider(8.0) {
    m_Rotation = PI;
    m_DX = 0.0;
    m_DY = 1.0;
    m_VX = 0.0;
    m_VY = 0.0;
    m_LastLaunchTime = current_time;
    
}

void Ship::RotateLeft() {
    m_Rotation -= delta_time;

    if( m_Rotation < 0.0 ) {
        m_Rotation += TWO_PI;
    }
    m_DX = sin(m_Rotation);
    m_DY = -cos(m_Rotation);
}

void Ship::RotateRight() {
    m_Rotation += delta_time;

    if( m_Rotation >= TWO_PI ) {
        m_Rotation -= TWO_PI;
    }
    m_DX = sin(m_Rotation);
    m_DY = -cos(m_Rotation);
}

void Ship::Accelerate() {
    m_VX += m_DX * delta_time;
    m_VY += m_DY * delta_time;
}

void Ship::Decelerate() {
    m_VX -= (m_DX * delta_time) / 2.0;
    m_VY -= (m_DY * delta_time) / 2.0;
}

void Ship::CapVelocity() {
    double vel = sqrt( m_VX * m_VX + m_VY * m_VY );

    if( vel > MAX_VELOCITY ) {
        m_VX /= vel;
        m_VY /= vel;

        m_VX *= MAX_VELOCITY;
        m_VY *= MAX_VELOCITY;
    }
}

void Ship::Render() {
    if( m_Alive == false ) {
        return;
    }

    dest.x = (int)m_X;
    dest.y = (int)m_Y;
    dest.w = c_Width;
    dest.h = c_Height;

    src.x = 32 * m_CurrentFrame;

    double degrees = (m_Rotation / PI) * 180.0;
//    SDL_SetTextureAlphaMod(m_SpriteTexture, 100);

    int return_code = SDL_RenderCopyEx( renderer, m_SpriteTexture, 
                        &src, &dest, 
                        degrees, NULL, SDL_FLIP_NONE );

    if( return_code != 0 ) {
        printf("failed to render image: %s\n", IMG_GetError() );
    }

}

bool Ship::CompoundHitTest( Collider* collider ) {
    Collider* col;
    std::vector<Collider*>::iterator it;

    for( it = m_Colliders.begin(); it != m_Colliders.end(); it++ ) {
        col = *it;
        if( col->HitTest(collider) ) {
            return true;
        }
    }

    return false;
}

