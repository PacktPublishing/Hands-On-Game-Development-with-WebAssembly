#include "game.hpp"

Ship::Ship() : Collider(8.0) {
    m_Rotation = PI;
    m_DX = 0.0;
    m_DY = 1.0;
    m_VX = 0.0;
    m_VY = 0.0;
    m_LastLaunchTime = current_time;

    // added -------------------------------------------------------
    m_Accelerating = false;

    m_Exhaust = new Emitter((char*)"/sprites/ProjectileExpOrange.png", 200, 
                    -10, 10,
                    400, 1.0, true,
                    0.1, 0.1,
                    30, 0, 12, 0.5,
                    0.5, 1.0,
                    0.5, 1.0,
                    0xffffff, 0xffffff,
                    0.7, 10, 
                    true, true, 
                    1000, 6 );    
    
    m_Exhaust->m_parent_rotation_ptr = &m_Rotation;
    m_Exhaust->m_parent_x_ptr = &m_X;
    m_Exhaust->m_parent_y_ptr = &m_Y;
    m_Exhaust->m_x_adjustment = 10;
    m_Exhaust->m_y_adjustment = 10;
    m_Exhaust->m_active = false;

    m_Explode = new Emitter((char*)"/sprites/Explode.png", 100, 
                    0, 360,
                    1000, 0.3, false,
                    20.0, 40.0,
                    10, 0, 0, 5,
                    1.0, 2.0,
                    1.0, 2.0,
                    0xffffff, 0xffffff,
                    0.0, 10, 
                    false, false, 
                    800, 8 );    

    m_Explode->m_parent_rotation_ptr = &m_Rotation;
    m_Explode->m_parent_x_ptr = &m_X;
    m_Explode->m_parent_y_ptr = &m_Y;
    m_Explode->m_active = false;
    // -------------------------------------------------------------

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
    // added -------------------------------------------------------
    m_Accelerating = true;
    // -------------------------------------------------------------

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

    m_Explode->Move(); // added

    if( m_Alive == false ) {
        return;
    }

    m_Exhaust->Move(); // added

    dest.x = (int)m_X; 
    dest.y = (int)m_Y; 
    dest.w = c_Width;
    dest.h = c_Height;

    src.x = 32 * m_CurrentFrame;

    double degrees = (m_Rotation / PI) * 180.0;

    int return_code = SDL_RenderCopyEx( renderer, m_SpriteTexture, 
                        &src, &dest, 
                        degrees, NULL, SDL_FLIP_NONE );

    if( return_code != 0 ) {
        printf("failed to render image: %s\n", IMG_GetError() );
    }

    // added -------------------------------------------------------
    if( m_Accelerating == false ) {
        m_Exhaust->m_active = false;
    }
    else {
        m_Exhaust->m_active = true;
    }

    m_Accelerating = false;
    // -------------------------------------------------------------
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

