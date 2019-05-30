#include "game.hpp"

Camera::Camera( float width, float height ) {
    m_HalfWidth = width / 2;
    m_HalfHeight = height / 2;

    m_Position = player->m_Position;

    m_Position.x -= CANVAS_WIDTH / 2;
    m_Position.y -= CANVAS_HEIGHT / 2;
}

void Camera::Move() {
    m_DesiredPosition = player->m_Position;

    m_DesiredPosition.x -= CANVAS_WIDTH / 2;
    m_DesiredPosition.y -= CANVAS_HEIGHT / 2;

    if( player->m_Active == false ) {
        m_Position.x = m_Position.x + (m_DesiredPosition.x - m_Position.x) * delta_time;
        m_Position.y = m_Position.y + (m_DesiredPosition.y - m_Position.y) * delta_time;
        return;
    }

//    m_DesiredPosition = m_Position;

    Projectile* projectile;
    std::vector<Projectile*>::iterator it;

    Vector2D attractor;

    for( it = projectile_pool->m_ProjectileList.begin(); it != projectile_pool->m_ProjectileList.end(); it++ ) {
        projectile = *it;
        if( projectile->m_Active ) {
            attractor = projectile->m_Position;
            attractor -= player->m_Position;
            attractor.Normalize();
            attractor *= 5;
            m_DesiredPosition += attractor;
        }
    }

    m_DesiredPosition += player->m_Velocity * 2;

    if( abs(m_DesiredPosition.x - m_Position.x) > CANVAS_WIDTH ) {
        if( m_DesiredPosition.x > m_Position.x ) {
            m_Position.x += LEVEL_WIDTH;
        }
        else {
            m_Position.x -= LEVEL_WIDTH;
        }
    }

    if( abs(m_DesiredPosition.y - m_Position.y) > CANVAS_HEIGHT ) {
        if( m_DesiredPosition.y > m_Position.y ) {
            m_Position.y += LEVEL_HEIGHT;
        }
        else {
            m_Position.y -= LEVEL_HEIGHT;
        }
    }
    
    m_Position.x = m_Position.x + (m_DesiredPosition.x - m_Position.x) * delta_time;
    m_Position.y = m_Position.y + (m_DesiredPosition.y - m_Position.y) * delta_time;
}

