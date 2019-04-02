#include "game.hpp"

Emitter::Emitter(
            Uint32 max_particles,

            bool looping,
            bool flip,
            bool animated,
            char* sprite_file,
            Uint32 sprite_width,
            Uint32 sprite_height,

            Uint32 duration,

            Uint32 min_particle_lifetime,
            Uint32 max_particle_lifetime,

            Uint32 emission_time_ms,
            Uint32 emission_time_ms_variance,

            Uint32 burst_time,
            Uint32 burst_particles,

            double max_velocity,

            double min_start_velocity_x,
            double max_start_velocity_x,

            double min_start_velocity_y,
            double max_start_velocity_y,

            double min_acceleration,
            double max_acceleration,

            double min_rotation_range,
            double max_rotation_range,
            
            double min_rotation_velocity,
            double max_rotation_velocity ) {

    m_Active = false;
    m_Animated = animated;
    m_Looping = looping;
    m_Flip = flip;
    m_Shape = POINT;
    m_RunTimeRemaining = 0;
    m_NextEmissionTime = 0;
    m_Duration = duration;

    m_MinParticleLifetime = min_particle_lifetime;
    m_MaxParticleLifetime = max_particle_lifetime;
    m_LifetimeDiff = m_MaxParticleLifetime - m_MinParticleLifetime;

    m_MaxParticles = max_particles;

    m_EmissionTimeMS = emission_time_ms;
    m_EmissionTimeMSVariance = emission_time_ms_variance;

    m_BurstTime = burst_time;
    m_BurstParticles = burst_particles;

    m_MinStartVelocityX = min_start_velocity_x;
    m_MaxStartVelocityX = max_start_velocity_x;

    m_MinStartVelocityY = min_start_velocity_y;
    m_MaxStartVelocityY = max_start_velocity_y;

    m_MinAcceleration = min_acceleration;
    m_MaxAcceleration = max_acceleration;

    m_MinRotation = min_rotation_range;
    m_MaxRotation = max_rotation_range;
    m_RotationRange = max_rotation_range - min_rotation_range;

    m_MinRotationVelocity = min_rotation_velocity;
    m_MaxRotationVelocity = max_rotation_velocity;
    m_RotationVelocityRange = max_rotation_velocity - min_rotation_velocity;


    SDL_Surface *temp_surface = IMG_Load( sprite_file );
    
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

    for( int i = 0; i < m_MaxParticles; i++ ) {
        m_Pool.push_back(
            new Particle( m_SpriteTexture,
                          m_Animated,
                            sprite_width,
                            sprite_height,
                            max_velocity )
        );
    }
}

void Emitter::EmitParticle() {
    m_TempPoint = GetParticlePosition();

    Particle* particle;
    std::vector<Particle*>::iterator it;

    for( it = m_Pool.begin(); it != m_Pool.end(); it++ ) {
        particle = *it;
        if( particle->m_Active == false ) {

            Uint32 life_time = m_MinParticleLifetime + rand() % m_LifetimeDiff;
            Uint32 rotation_range_mult = (Uint32)(m_RotationRange * 1000.0);
            double starting_rotation = m_MinRotation + ((double)(rand() % rotation_range_mult) / 1000.0);

            rotation_range_mult = (Uint32)(m_RotationVelocityRange * 1000.0);
            double rotation_velocity = m_MinRotationVelocity + ((double)(rand() % rotation_range_mult) / 1000.0);
            double rotation_acceleration = (double)(rand() % 10 - 5) / 100.0;

            double alpha = 255;
            double alpha_fade_rate = 0;

            double starting_vx;
            double starting_vy;

            double acceleration;

            particle->Emit(
                m_TempPoint.x,
                m_TempPoint.y,
                life_time,
                starting_rotation,
                rotation_velocity,
                rotation_acceleration,
                alpha,
                alpha_fade_rate,
                starting_vx,
                starting_vy,
                acceleration
            );
        }
    }

    /*
    m_X = m_TempPoint.x;
    m_Y = m_TempPoint.y;

    void Particle::Emit(
            int x,
            int y,
            Uint32 life_time,

            double starting_rotation,
            double rotation_velocity,
            double rotation_acceleration,

            double alpha,
            double alpha_fade_rate,

            double starting_vx,
            double starting_vy,

            double acceleration ) {
        
    }
    */
}

Point Emitter::GetParticlePosition() {
   if( m_Shape == POINT ) {
       m_TempPoint.x = m_X;
       m_TempPoint.y = m_Y;
       return m_TempPoint;
   }
   else if( m_Shape == CIRCLE ) {
        int rval = rand() % 1000;
        double pct = (double)rval / 1000.0;
        double rotation = TWO_PI * pct;

        rval = rand() % 1000;
        pct = (double)rval / 1000.0;

        m_TempPoint.y = 0;
        m_TempPoint.x = m_CircleEmitRadius * pct;
        m_TempPoint.Rotate( rotation );
   }
   else if( m_Shape == RECT ) {
        int rval = rand() % 1000;
        double pct = (double)rval / 1000.0;

        m_TempPoint.x = m_RectEmitWidth * pct;

        rval = rand() % 1000;
        pct = (double)rval / 1000.0;

        m_TempPoint.y = m_RectEmitHeight * pct;
   }
   else if( m_Shape == DONUT ) {
        double rad_diff = m_DonutEmitOuterRadius - m_DonutEmitInnerRadius;
        int rval = rand() % 1000;
        double pct = (double)rval / 1000.0;
        double rotation = TWO_PI * pct;

        rval = rand() % 1000;
        pct = (double)rval / 1000.0;

        m_TempPoint.y = 0;
        m_TempPoint.x = m_DonutEmitInnerRadius + rad_diff * pct;
        m_TempPoint.Rotate( rotation );
   }
   
    m_TempPoint.x += m_X;
    m_TempPoint.y += m_Y;

    return m_TempPoint;
}

void Emitter::Start() {
    m_Active = true;

    m_RunTimeRemaining = m_Duration;
    m_NextEmissionTime = m_EmissionTimeMS + (rand() % m_EmissionTimeMSVariance) - (m_EmissionTimeMSVariance >> 1);
}

void Emitter::Stop() {
    m_Active = false;
    m_RunTimeRemaining = 0;
    m_NextEmissionTime = 0;

}

void Emitter::Move() {
    if( m_Active ) {
        m_NextEmissionTime -= diff_time;
        m_RunTimeRemaining -= diff_time;

        if( m_NextEmissionTime <= 0 ) {
            EmitParticle();
            m_NextEmissionTime = m_EmissionTimeMS + 
                                (rand() % m_EmissionTimeMSVariance) - 
                                (m_EmissionTimeMSVariance >> 1);
        }

        if( m_RunTimeRemaining <= 0 ) {
            if( m_Looping ) {
                m_RunTimeRemaining = m_Duration;
            }
            else {
                m_Active = false;
            }
        }
    }
//    std::vector<Particle*> m_Pool; 

    Particle* particle;
    std::vector<Particle*>::iterator it;

    for( it = m_Pool.begin(); it != m_Pool.end(); it++ ) {
        particle = *it;
        if( particle->m_Active ) {
            particle->Move();
            particle->Render();
        }
    }

}

void Emitter::SetEmitShapePoint() {
    m_Shape = POINT;
}

void Emitter::SetEmitShapeCircle( double radius ) {
    m_Shape = CIRCLE;
    m_CircleEmitRadius = radius;
}

void Emitter::SetEmitShapeRect( double width, double height ) {
    m_Shape = RECT;
    m_RectEmitWidth = width;
    m_RectEmitHeight = height;
}

void Emitter::SetEmitShapeDonut( double inner_radius, double outer_radius ) {
    m_Shape = DONUT;
    m_DonutEmitInnerRadius = inner_radius;
    m_DonutEmitOuterRadius = outer_radius;
}


/*


class Emitter {
    private:
        SDL_Texture *m_SpriteTexture;
        std::vector<Particle*> m_Pool; 
    public:
        bool m_Active;
        bool m_Looping;
        bool m_Flip;
        EMITTER_SHAPE m_Shape;
        Uint32 m_RunTimeRemaining;
        Uint32 m_NextEmissionTime;
        Uint32 m_Duration;

        Uint32 m_MinParticleLifetime;
        Uint32 m_MaxParticleLifetime;

        Uint32 m_MaxParticles;

        Uint32 m_EmissionRate;
        Uint32 m_EmissionRateVariance;

        Uint32 m_BurstTime;
        Uint32 m_BurstParticles;

        double m_MinStartVelocityX;
        double m_MaxStartVelocityX;

        double m_MinStartVelocityY;
        double m_MaxStartVelocityY;

        double m_MinAcceleration;
        double m_MaxAcceleration;

        double m_MinRotationRange;
        double m_MaxRotationRange;

        Emitter(
            Uint32 max_particles,

            bool looping,
            bool flip,
            EMITTER_SHAPE shape,
            Uint32 duration,

            Uint32 min_particle_lifetime,
            Uint32 max_particle_lifetime,

            Uint32 emission_rate,
            Uint32 emission_rate_variance,

            Uint32 burst_time,
            Uint32 burst_particles,

            double min_start_velocity_x,
            double max_start_velocity_x,

            double min_start_velocity_y,
            double max_start_velocity_y,

            double min_acceleration,
            double max_acceleration,

            double min_rotation_range,
            double max_rotation_range
        );
        void Start();
        void Stop();
        void Move();
        void Render();
};
*/