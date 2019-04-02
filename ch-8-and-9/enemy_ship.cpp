#include "game.hpp"

EnemyShip::EnemyShip() {
    m_X = 60.0;
    m_Y = 50.0;
    m_Rotation = PI;
    m_DX = 0.0;
    m_DY = 1.0;
    m_VX = 0.0;
    m_VY = 0.0;
    m_AIStateTTL = c_AIStateTime;
    m_Alive = true;
    m_LastLaunchTime = current_time;

    
    Collider* temp_collider = new Collider(2.0);
    temp_collider->SetParentInformation( &(this->m_Rotation), 
                                         &(this->m_X), &(this->m_Y) );

    temp_collider->m_X = -6.0;
    temp_collider->m_Y = -6.0;
    m_Colliders.push_back( temp_collider );

    temp_collider = new Collider(2.0);
    temp_collider->SetParentInformation( &(this->m_Rotation), 
                                         &(this->m_X), &(this->m_Y) );

    temp_collider->m_X = 6.0;
    temp_collider->m_Y = -6.0;
    m_Colliders.push_back( temp_collider );


    
    SDL_Surface *temp_surface = IMG_Load( c_SpriteFile );
    
    if( !temp_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return;
    }
    else { 
        printf("success creating enemy ship surface\n");
    }

    m_SpriteTexture = SDL_CreateTextureFromSurface( renderer, temp_surface );

    if( !m_SpriteTexture ) {
        printf("failed to create texture: %s\n", IMG_GetError() );
        return;
    }
    else { 
        printf("success creating enemy ship texture\n");
    }

    SDL_FreeSurface( temp_surface );
}

void EnemyShip::Move() {
    if( m_Alive == false ) {
        return;
    }

    AIStub();

    if( m_AIState == TURN_LEFT ) {
        RotateLeft();
    }
    if( m_AIState == TURN_RIGHT ) {
        RotateRight();
    }

    if( m_AIState == ACCELERATE ) {
        Accelerate();
    }
    if( m_AIState == DECELERATE ) {
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

    if( m_AIState == SHOOT ) {
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

void EnemyShip::AIStub() {
    m_AIStateTTL -= diff_time;

    if( m_AIStateTTL <= 0 ) {
        // for now get a random AI state.
        m_AIState = (FSM_STUB)(rand() % 5);
        m_AIStateTTL = c_AIStateTime;
    }

}