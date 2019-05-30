#include "game.hpp"

FiniteStateMachine::FiniteStateMachine(EnemyShip* ship) {
    m_Ship = ship;
    m_CurrentState = APPROACH;
    m_HasLOS = false;
    m_DesiredRotation = 0.0;
    m_CheckCycle = 0;
    m_PlayerDistSQ = 0;
}

void FiniteStateMachine::SeekState(Vector2D &seek_point) {
    Vector2D direction = seek_point;
    direction -= m_Ship->m_Position;

    m_DesiredRotation = direction.FindAngle();

    float rotate_direction = m_Ship->m_Rotation - m_DesiredRotation;

    if( rotate_direction > PI ) {
        rotate_direction -= 2 * PI;
    }
    else if( rotate_direction < -PI ) {
        rotate_direction += 2 * PI;
    }

    if( rotate_direction < -0.05 ) {
        m_Ship->RotateRight();
        m_Ship->RotateRight();
    }
    else if( rotate_direction > 0.05 ) {
        m_Ship->RotateLeft();
        m_Ship->RotateLeft();
    }
    m_Ship->Accelerate();
    m_Ship->Accelerate();
    m_Ship->Accelerate();
    m_Ship->Accelerate();

}

void FiniteStateMachine::FleeState(Vector2D& flee_point) {
    Vector2D direction = flee_point;
    direction -= m_Ship->m_Position;

    m_DesiredRotation = direction.FindAngle();

    float rotate_direction = m_DesiredRotation - m_Ship->m_Rotation;
    rotate_direction -= PI;

    if( rotate_direction > 0 ) {
        m_Ship->RotateRight();
    }
    else {
        m_Ship->RotateLeft();
    }
    m_Ship->Accelerate();
    m_Ship->Accelerate();
}

void FiniteStateMachine::WanderState() {
    m_NextTurnMS -= delta_time;

    if( m_NextTurnMS <= 0 ) {
        bool same_turn = ( m_SameTurnPct >= rand() % 100 );

        m_NextTurnMS = c_MinRandomTurnMS + rand() % c_RandTurnMS;

        if( m_LastTurnLeft ) {
            if( same_turn ) {
                m_SameTurnPct -= 10;
                m_Ship->RotateLeft();
            }
            else {
                m_SameTurnPct = 80;
                m_Ship->RotateRight();
            }
        }
        else {
            if( same_turn ) {
                m_SameTurnPct -= 10;
                m_Ship->RotateRight();
            }
            else {
                m_SameTurnPct = 80;
                m_Ship->RotateLeft();
            }
        }
    }
    m_Ship->Accelerate();
}

void FiniteStateMachine::AttackState() {
    Vector2D prediction = PredictPosition();
    SeekState( prediction );
    m_Ship->Shoot();
}

Vector2D FiniteStateMachine::PredictPosition() {
    Vector2D dist = player->m_Position;
    dist -= m_Ship->m_Position;

    float mag = dist.Magnitude();
    Vector2D dir = player->m_Velocity;
    if( dir.MagSQ() > 0 ) {
        dir.Normalize();
    }
    dir *= (mag / 10);

    Vector2D prediction = player->m_Position;
    prediction += dir;

    return prediction;
}

void FiniteStateMachine::AvoidForce() {
    Vector2D start_corner;
    Vector2D end_corner;

    Vector2D avoid_vec;
    Vector2D dist;
    float closest_square = 999999999999.0;
    float msq;

    Vector2D star_avoid;
    star_avoid.x = LEVEL_WIDTH / 2;
    star_avoid.y = LEVEL_HEIGHT / 2;

    star_avoid -= m_Ship->m_Position;

    msq = star_avoid.MagSQ();

    if( msq >= c_StarAvoidDistSQ ) {
        start_corner = m_Ship->m_Position;
        start_corner.x -= c_AvoidDist;
        start_corner.y -= c_AvoidDist;

        end_corner = m_Ship->m_Position;
        end_corner.x += c_AvoidDist;
        end_corner.y += c_AvoidDist;

        Asteroid* asteroid;
        std::vector<Asteroid*>::iterator it;
        int i = 0;
        for( it = asteroid_list.begin(); it != asteroid_list.end(); it++ ) {
            asteroid = *it;
            if( asteroid->m_Active == true &&
                asteroid->SteeringRectTest( start_corner, end_corner ) ) {
                dist = asteroid->m_Position;
                dist -= m_Ship->m_Position;
                msq = dist.MagSQ();

                if( msq <= closest_square ) {
                    closest_square = msq;
                    avoid_vec = asteroid->m_Position;
                }
            }
        }

        // LOOP OVER PROJECTILES
        Projectile* projectile;
        std::vector<Projectile*>::iterator proj_it;
        for( proj_it = projectile_pool->m_ProjectileList.begin(); proj_it != projectile_pool->m_ProjectileList.end(); proj_it++ ) {
            projectile = *proj_it;
            if( projectile->m_Active == true &&
                projectile->SteeringRectTest( start_corner, end_corner ) ) {
                dist = projectile->m_Position;
                dist -= m_Ship->m_Position;
                msq = dist.MagSQ();
                
                if( msq <= closest_square ) {
                    closest_square = msq;
                    avoid_vec = projectile->m_Position;
                }
            }
        }

        if( closest_square != 999999999999.0 ) {
            avoid_vec -= m_Ship->m_Position;
            avoid_vec.Normalize();

            float rot_to_obj = avoid_vec.FindAngle();

            if( std::abs( rot_to_obj - m_Ship->m_Rotation ) < 0.75 ) {
                if( rot_to_obj >= m_Ship->m_Rotation ) {
                    m_Ship->RotateLeft();
                }
                else {
                    m_Ship->RotateRight();
                }
            }
            m_Ship->m_Velocity -= avoid_vec * delta_time * c_ObstacleAvoidForce;
        }
    }
    else {
        avoid_vec.x = LEVEL_WIDTH / 2;
        avoid_vec.y = LEVEL_HEIGHT / 2;

        avoid_vec -= m_Ship->m_Position;
        avoid_vec.Normalize();

        float rot_to_obj = avoid_vec.FindAngle();

        if( std::abs( rot_to_obj - m_Ship->m_Rotation ) < 0.75 ) {
            if( rot_to_obj >= m_Ship->m_Rotation ) {
                m_Ship->RotateLeft();
            }
            else {
                m_Ship->RotateRight();
            }
        }
        m_Ship->m_Velocity -= avoid_vec * delta_time * c_StarAvoidForce; 
    }


}

bool FiniteStateMachine::ShieldCheck() {
    Vector2D start_corner;
    Vector2D end_corner;

    start_corner = m_Ship->m_Position;
    start_corner.x -= c_ShieldDist;
    start_corner.y -= c_ShieldDist;

    end_corner = m_Ship->m_Position;
    end_corner.x += c_ShieldDist;
    end_corner.y += c_ShieldDist;

    Asteroid* asteroid;
    std::vector<Asteroid*>::iterator it;
    int i = 0;
    for( it = asteroid_list.begin(); it != asteroid_list.end(); it++ ) {
        asteroid = *it;
        if( asteroid->m_Active &&
            asteroid->SteeringRectTest( start_corner, end_corner ) ) {
            return true;
        }
    }

    // LOOP OVER PROJECTILES
    Projectile* projectile;
    std::vector<Projectile*>::iterator proj_it;
    for( proj_it = projectile_pool->m_ProjectileList.begin(); proj_it != projectile_pool->m_ProjectileList.end(); proj_it++ ) {
        projectile = *proj_it;
        if( projectile->m_Active &&
            projectile->SteeringRectTest( start_corner, end_corner ) ) {
            return true;
        }
    }

    return false;
}

bool FiniteStateMachine::LOSCheck() {  // LINE OF SIGHT CHECK
    // LOOP OVER ASTEROIDS
    Asteroid* asteroid;
    std::vector<Asteroid*>::iterator it;
    int i = 0;
    for( it = asteroid_list.begin(); it != asteroid_list.end(); it++ ) {
        asteroid = *it;
        if( asteroid->SteeringLineTest( m_Ship->m_Position, player->m_Position ) ) {
            m_Ship->line_to_x = asteroid->m_Position.x - m_Ship->m_Position.x;
            m_Ship->line_to_y = asteroid->m_Position.y - m_Ship->m_Position.y;
            m_Ship->green = false;
            return false;
        }
    }

    // LOOP OVER PROJECTILES
    Projectile* projectile;
    std::vector<Projectile*>::iterator proj_it;
    for( proj_it = projectile_pool->m_ProjectileList.begin(); proj_it != projectile_pool->m_ProjectileList.end(); proj_it++ ) {
        projectile = *proj_it;
        if( projectile->SteeringLineTest( m_Ship->m_Position, player->m_Position ) ) {
            m_Ship->line_to_x = projectile->m_Position.x - m_Ship->m_Position.x;
            m_Ship->line_to_y = projectile->m_Position.y - m_Ship->m_Position.y;
            m_Ship->green = false;
            return false;
        }
    }

    m_Ship->green = true;
    m_Ship->line_to_x = 0; //player->m_Position.x - m_Ship->m_Position.x;
    m_Ship->line_to_y = 0; //player->m_Position.y - m_Ship->m_Position.y;
    return true;
}

float FiniteStateMachine::GetPlayerDistSq() {
    float x_diff = m_Ship->m_Position.x - player->m_Position.x;
    float y_diff = m_Ship->m_Position.y - player->m_Position.y;

    return x_diff * x_diff + y_diff * y_diff;
}

void FiniteStateMachine::Move() {

    m_CheckCycle++;

    if( m_CheckCycle == 0 ) {
        m_HasLOS = LOSCheck();
        if( !m_HasLOS ) {
            m_CurrentState = WANDER;
        }
        float player_dist_sq = 0.0f;
    }
    else if( m_CheckCycle == 1 ) {
        if( m_HasLOS ) {
            m_PlayerDistSQ = GetPlayerDistSq();

            if( m_PlayerDistSQ <= c_FleeDistSq ) {
                m_CurrentState = FLEE;
            }
            else if( m_PlayerDistSQ <= c_AttackDistSq ) {
                m_CurrentState = ATTACK;
            }
            else {
                m_CurrentState = APPROACH;
            }
        }
    }
    else {
        AvoidForce();
        m_CheckCycle = -1;
    }

    if( ShieldCheck() ) {
        m_Ship->m_Shield->Activate();
    }
    else {
        m_Ship->m_Shield->Deactivate();
    }

    if( m_CurrentState == APPROACH ) {
        Vector2D predict = PredictPosition();
        SeekState(predict);
m_Ship->line_to_x = predict.x - m_Ship->m_Position.x;
m_Ship->line_to_y = predict.y - m_Ship->m_Position.y;
m_Ship->green = true;
    }
    else if( m_CurrentState == ATTACK ) {
        AttackState();
    }
    else if( m_CurrentState == FLEE ) {
        Vector2D predict = PredictPosition();
        FleeState(predict);    
    }
    else if( m_CurrentState == WANDER ) {
        WanderState();        
    }
}
