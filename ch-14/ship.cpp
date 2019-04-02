#include "game.hpp"

Ship::Ship() : Collider(8.0) {
    m_Rotation = PI;

    m_LastLaunchTime = current_time;

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
    m_Exhaust->m_parent_x_ptr = &(m_Position.x);
    m_Exhaust->m_parent_y_ptr = &(m_Position.y);
    m_Exhaust->m_x_adjustment = 10;
    m_Exhaust->m_y_adjustment = 10;
    m_Exhaust->m_Active = false;

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
    m_Explode->m_parent_x_ptr = &(m_Position.x);
    m_Explode->m_parent_y_ptr = &(m_Position.y);
    m_Explode->m_Active = false;

    m_Direction.y = 1.0;

    m_Active = true;
    m_Mass = 50.0;
}

void Ship::RotateLeft() {
    m_Rotation -= delta_time * 2;

    if( m_Rotation < 0.0 ) {
        m_Rotation += TWO_PI;
    }
    m_Direction.x = sin(m_Rotation);
    m_Direction.y = -cos(m_Rotation);
}

void Ship::RotateRight() {
    m_Rotation += delta_time * 2;

    if( m_Rotation >= TWO_PI ) {
        m_Rotation -= TWO_PI;
    }
    m_Direction.x = sin(m_Rotation);
    m_Direction.y = -cos(m_Rotation);
}

void Ship::Accelerate() {
    m_Accelerating = true;

    m_Velocity += m_Direction * ((c_MaxVelocity/4)* delta_time);

    CapVelocity();
}

void Ship::Shoot() {
    Projectile* projectile;
    if( current_time - m_LastLaunchTime >= c_MinLaunchTime ) {
        m_LastLaunchTime = current_time;
        projectile = projectile_pool->GetFreeProjectile();
        if( projectile != NULL ) {
            projectile->Launch( m_Position, m_Direction );
            player_laser_snd->Play();
            m_Velocity -= m_Direction * (projectile->c_Velocity * projectile->m_Mass / m_Mass);
            CapVelocity();
        }
    }
}

void Ship::Decelerate() {
    m_Velocity -= m_Direction * ((c_MaxVelocity/10)* delta_time);

    CapVelocity();
}

void Ship::CapVelocity() {

    float vel_sq = m_Velocity.MagSQ();

    if( vel_sq > c_MaxVelocity * c_MaxVelocity ) {
        m_Velocity.Normalize();
        m_Velocity *= c_MaxVelocity;
    }
}

void Ship::Render() {

    m_Explode->Move();

    if( m_Active == false ) {
        return;
    }

    m_Exhaust->Move();

    dest.x = (int)m_Position.x; 
    dest.y = (int)m_Position.y; 
    dest.w = c_Width;
    dest.h = c_Height;

    src.x = 32 * m_CurrentFrame;

    render_manager->Render( m_SpriteTexture, &src, &dest, m_Rotation );

    if( m_Accelerating == false ) {
        m_Exhaust->m_Active = false;
    }
    else {
        m_Exhaust->m_Active = true;
    }

    m_Shield->Render();

    m_Accelerating = false;
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

