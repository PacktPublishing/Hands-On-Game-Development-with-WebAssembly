#include "game.hpp"

PlayerShip::PlayerShip() {
    m_X = 160.0;
    m_Y = 100.0;
    SDL_Surface *temp_surface = IMG_Load( c_SpriteFile );
    
    Collider* temp_collider = new Collider(3.0);
    temp_collider->SetParentInformation( &(this->m_Rotation), 
                                         &(this->m_X), &(this->m_Y) );

    temp_collider->m_X = -6.0;
    temp_collider->m_Y = 6.0;
    m_Colliders.push_back( temp_collider );

    temp_collider = new Collider(3.0);
    temp_collider->SetParentInformation( &(this->m_Rotation), 
                                         &(this->m_X), &(this->m_Y) );

    temp_collider->m_X = 6.0;
    temp_collider->m_Y = 6.0;
    m_Colliders.push_back( temp_collider );


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

void PlayerShip::Move() {
    if( m_Alive == false ) {
        return;
    }

    if( left_key_down ) {
        RotateLeft();
    }
    if( right_key_down ) {
        RotateRight();
    }

    if( up_key_down ) {
        Accelerate();
    }
    if( down_key_down ) {
        Decelerate();
    }

    if( m_CurrentFrame > 0 ) {
        m_NextFrameTime -= diff_time;
        if( m_NextFrameTime <= 0 ) {
            m_NextFrameTime = ms_per_frame;
            if( ++m_CurrentFrame >= 8 ) {
                m_Alive = false;
                return;
            }
        }
    }

    CapVelocity();
    m_X += m_VX;

    if( m_X > 320 ) {
        m_X = -16;
    }
    else if( m_X < -16 ) {
        m_X = 320;
    }

    m_Y += m_VY;

    if( m_Y > 200 ) {
        m_Y = -16;
    }
    else if( m_Y < -16 ) {
        m_Y = 200;
    }

    if( space_key_down ) {
        Projectile* projectile;
        if( current_time - m_LastLaunchTime >= c_MinLaunchTime ) {
            m_LastLaunchTime = current_time;
            projectile = projectile_pool->GetFreeProjectile();
            if( projectile != NULL ) {
                projectile->Launch( m_X, m_Y, m_DX, m_DY );
            }
        }
    }

}
