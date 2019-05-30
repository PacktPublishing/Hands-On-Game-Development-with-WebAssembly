#include "game.hpp"

Collider::Collider(float radius) {
    m_ParentRotation = NULL;
    m_ParentX = NULL;
    m_ParentY = NULL;

    m_Radius = radius;
    m_SteeringRadius = m_Radius * 1.5;
    m_SteeringRadiusSQ = m_SteeringRadius * m_SteeringRadius;

    m_Active = false;
}

bool Collider::CCHitTest( Collider* collider ) {
    m_TempPoint.x = m_Position.x;
    m_TempPoint.y = m_Position.y;

    m_TempPoint.Rotate( *m_ParentRotation );

    float dist_x = (*m_ParentX + m_TempPoint.x) - collider->m_Position.x;
    float dist_y = (*m_ParentY + m_TempPoint.y) - collider->m_Position.y;
    float radius = m_Radius + collider->m_Radius;

    if( dist_x * dist_x + dist_y * dist_y <= radius * radius ) {
        return true;
    }

    return false;
}

bool Collider::HitTest( Collider *collider ) {
    if( m_ParentRotation != NULL && m_ParentX != NULL && m_ParentY != NULL ) {
        return CCHitTest( collider );
    }

    float dist_x = m_Position.x - collider->m_Position.x;
    float dist_y = m_Position.y - collider->m_Position.y;
    float radius = m_Radius + collider->m_Radius;

    if( dist_x * dist_x + dist_y * dist_y <= radius * radius ) {
        return true;
    }

    return false;
}

void Collider::SetParentInformation( float* rotation, float* x, float* y ) {
    m_ParentRotation = rotation;
    m_ParentX = x;
    m_ParentY = y;
}

bool Collider::SteeringLineTest( Vector2D &start, Vector2D &end ) {
    if( m_Active == false ) {
        return false;
    }

    Vector2D dist = start;

    dist -= m_Position;

    if( m_SteeringRadiusSQ > dist.MagSQ() ) {
        return true;
    }

    dist = end;
    dist -= m_Position;

    if( m_SteeringRadiusSQ > dist.MagSQ() ) {
        return true;
    }

    dist = end;
    dist -= start;

    Vector2D circle_vec = m_Position;
    circle_vec -= start;

    Vector2D near_point = circle_vec.Project( dist );
    near_point += start;

    Vector2D temp_vector = near_point; 
    circle_vec += start;
    temp_vector -= circle_vec; 

    Range x_range;
    x_range.min = start.x;
    x_range.max = end.x;
    x_range.Sort();

    Range y_range;
    y_range.min = start.y;
    y_range.max = end.y;
    y_range.Sort();

    if ((x_range.min <= near_point.x && near_point.x <= x_range.max &&
        y_range.min <= near_point.y && near_point.y <= y_range.max) == false) {
        return false;
    }

    if( temp_vector.MagSQ() < m_SteeringRadiusSQ ) {
        return true;
    }

    return false;
}

bool Collider::SteeringRectTest( Vector2D &start_point, Vector2D &end_point ) {
    if( m_Position.x < start_point.x || 
        m_Position.y < start_point.y ) {
        return false;
    }

    if( m_Position.x > end_point.x || 
        m_Position.y > end_point.y ) {
        return false;
    }

    return true;
}

void Collider::WrapPosition() {
    if( m_Position.x > LEVEL_WIDTH ) {
        m_Position.x -= LEVEL_WIDTH; 
    }
    else if( m_Position.x < 0 ) {
        m_Position.x += LEVEL_WIDTH; 
    }

    if( m_Position.y > LEVEL_HEIGHT ) {
        m_Position.y -= LEVEL_HEIGHT;
    }
    else if( m_Position.y < 0 ) {
        m_Position.y += LEVEL_HEIGHT;
    }

}
