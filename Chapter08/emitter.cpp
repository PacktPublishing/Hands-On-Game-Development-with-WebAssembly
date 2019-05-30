#include "game.hpp"


Emitter::Emitter(char* sprite_file, int max_particles, float min_angle, float max_angle,
                    Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    float min_starting_velocity, float max_starting_velocity,
                    Uint32 emission_rate, int x_pos, int y_pos, float radius  ) {

    if( min_starting_velocity > max_starting_velocity ) {
        m_min_starting_velocity = max_starting_velocity;
        m_max_starting_velocity = min_starting_velocity;
    }
    else {
        m_min_starting_velocity = min_starting_velocity;
        m_max_starting_velocity = max_starting_velocity;
    }

    SDL_Surface *temp_surface = IMG_Load( sprite_file );
    
    if( !temp_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return;
    }
    m_sprite_texture = SDL_CreateTextureFromSurface( renderer, temp_surface );

    SDL_FreeSurface( temp_surface );

    SDL_QueryTexture( m_sprite_texture, 
                        NULL, NULL, 
                        &m_sprite_width, &m_sprite_height );

    m_max_particles = max_particles;

    for( int i = 0; i < m_max_particles; i++ ) {
        m_particle_pool.push_back(
            new Particle(   m_sprite_texture, particle_lifetime, acceleration, 
                            alpha_fade, m_sprite_width, m_sprite_height )
        );
    }

    m_max_angle = max_angle;
    m_min_angle = min_angle;
    m_radius = radius;
    m_position.x = (float)x_pos;
    m_position.y = (float)y_pos;
    m_emission_rate = emission_rate;
    m_emission_time_ms = 1000 / m_emission_rate;
    m_next_emission = 0;
}

void Emitter::Update(int max_particles, float min_angle, float max_angle,
                    Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    float min_starting_velocity, float max_starting_velocity,
                    Uint32 emission_rate, int x_pos, int y_pos, float radius ) {

    if( min_starting_velocity > max_starting_velocity ) {
        m_min_starting_velocity = max_starting_velocity;
        m_max_starting_velocity = min_starting_velocity;
    }
    else {
        m_min_starting_velocity = min_starting_velocity;
        m_max_starting_velocity = max_starting_velocity;
    }

    m_max_particles = max_particles;
    m_min_angle = min_angle;
    m_max_angle = max_angle;
    m_emission_rate = emission_rate;
    m_position.x = (float)x_pos;
    m_position.y = (float)y_pos;
    m_radius = radius;

    if( m_particle_pool.size() > m_max_particles ) {
        m_particle_pool.resize( m_max_particles );
    }
    else if( m_max_particles > m_particle_pool.size() ) {
        while( m_max_particles > m_particle_pool.size() ) {
            m_particle_pool.push_back(
                new Particle(   m_sprite_texture, particle_lifetime, acceleration, 
                                alpha_fade, m_sprite_width, m_sprite_height )
            );
        }
    }

    Particle* particle;
    std::vector<Particle*>::iterator it;

    for( it = m_particle_pool.begin(); it != m_particle_pool.end(); it++ ) {
        particle = *it;
        particle->Update( particle_lifetime, acceleration, alpha_fade );
    }

}

Particle* Emitter::GetFreeParticle() {
    Particle* particle;
    std::vector<Particle*>::iterator it;

    for( it = m_particle_pool.begin(); it != m_particle_pool.end(); it++ ) {
        particle = *it;
        if( particle->m_active == false ) {
            return particle;
        }
    }
    return NULL;
}

void Emitter::Move() {
    Particle* particle;
    std::vector<Particle*>::iterator it;
    static int count = 0;

    m_next_emission -= diff_time;
    if( m_next_emission <= 0 ) {
        m_next_emission = m_emission_time_ms;

        particle = GetFreeParticle();

        if( particle != NULL ) {
            float rand_vel = (rand() % (int)((m_max_starting_velocity - m_min_starting_velocity) * 1000)) / 1000.0f;

            Point spawn_point;
            spawn_point.x = (float)(rand() % (int)(m_radius * 1000)) / 1000.0;
            Point velocity_point;
            velocity_point.x = (float)(rand() % (int)((m_max_starting_velocity + rand_vel) * 1000)) / 1000.0;

            int angle_int = (int)((m_max_angle - m_min_angle) * 1000.0);
            float add_angle = (float)(rand() % angle_int) /1000.0f;
            float angle = m_min_angle + add_angle;
            velocity_point.Rotate(angle);

            angle = (float)(rand() % 62832) / 10000.0;
            spawn_point.Rotate( angle );
            spawn_point.x += m_position.x;
            spawn_point.y += m_position.y;

            particle->Spawn(spawn_point.x, spawn_point.y, velocity_point.x, velocity_point.y, 255.0f );
        }
    }

    for( it = m_particle_pool.begin(); it != m_particle_pool.end(); it++ ) {
        particle = *it;
        if( particle->m_active ) {
            particle->Move();
            particle->Render();
        }
    }
}