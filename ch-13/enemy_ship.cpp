#include "game.hpp"

EnemyShip::EnemyShip() {
    m_Position.x = 810.0;
    m_Position.y = 800.0;

    m_Rotation = PI;
    m_AIStateTTL = c_AIStateTime;
    m_Active = true;
    m_LastLaunchTime = current_time;

    m_FSM = new FiniteStateMachine(this);

    m_Shield = new Shield(this, c_ShieldSpriteFile);
    
    Collider* temp_collider = new Collider(2.0);
    temp_collider->SetParentInformation( &(this->m_Rotation), 
                                         &(m_Position.x), &(m_Position.y) );

    temp_collider->m_Position.x = -6.0;
    temp_collider->m_Position.y = -6.0;
    m_Colliders.push_back( temp_collider );

    temp_collider = new Collider(2.0);
    temp_collider->SetParentInformation( &(this->m_Rotation), 
                                         &(m_Position.x), &(m_Position.y) );

    temp_collider->m_Position.x = 6.0;
    temp_collider->m_Position.y = -6.0;
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
    if( m_Active == false ) {
        return;
    }

    m_FSM->Move();

    if( m_CurrentFrame > 0 ) {
        m_NextFrameTime -= diff_time;

        if( m_NextFrameTime <= 0 ) {
            m_NextFrameTime = ms_per_frame;
            if( ++m_CurrentFrame >= 8 ) {
                m_Active = false;
                return;
            }
        }
    }

    m_Position += m_Velocity * delta_time;

    m_Shield->Move();

    WrapPosition();

}
