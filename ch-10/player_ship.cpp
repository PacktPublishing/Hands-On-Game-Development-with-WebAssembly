#include "game.hpp"

PlayerShip::PlayerShip() {
    m_Position.x = CANVAS_WIDTH - 210.0;
    m_Position.y = CANVAS_HEIGHT - 200.0;

    m_Shield = new Shield(this, c_ShieldSpriteFile);

    SDL_Surface *temp_surface = IMG_Load( c_SpriteFile );
    
    Collider* temp_collider = new Collider(3.0);
    temp_collider->SetParentInformation( &m_Rotation, 
                                         &(m_Position.x), &(m_Position.y) );

    temp_collider->m_Position.x = -6.0;
    temp_collider->m_Position.y = 6.0;
    m_Colliders.push_back( temp_collider );

    temp_collider = new Collider(3.0);
    temp_collider->SetParentInformation( &m_Rotation, 
                                         &(m_Position.x), &(m_Position.y) );

    temp_collider->m_Position.x = 6.0;
    temp_collider->m_Position.y = 6.0;
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
    if( m_Active == false ) {
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

    if( f_key_down ) {
        m_Shield->Activate();
    }
    else {
        m_Shield->Deactivate();
    }

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

    m_Shield->Move();

    m_Position += m_Velocity * delta_time;

    WrapPosition();
    
    if( space_key_down ) {
        this->Shoot(); // m_Position, m_Direction );
    }

}
