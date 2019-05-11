#include "game.hpp"


Emitter::Emitter(char* sprite_file, int max_particles, float min_angle, float max_angle,
                    Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    float min_starting_velocity, float max_starting_velocity,
                    Uint32 emission_rate, int x_pos, int y_pos, float radius,
                    float min_start_scale, float max_start_scale,
                    float min_end_scale, float max_end_scale,
                    Uint32 start_color, Uint32 end_color,
                    float burst_time_pct, Uint32 burst_particles, 
                    bool loop, bool align_rotation, Uint32 emit_time_ms, Uint32 animation_frames ) {

    m_start_color = start_color;
    m_end_color = end_color;
    m_active = true;

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
                            alpha_fade, m_sprite_width, m_sprite_height, align_rotation,
                            m_start_color, m_end_color, animation_frames )
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

    /* new values */
    m_min_start_scale = min_start_scale;
    m_max_start_scale = max_start_scale;
    m_min_end_scale = min_end_scale;
    m_max_end_scale = max_end_scale;
    
    m_loop = loop;
    m_align_rotation = align_rotation;
    m_emit_loop_ms = emit_time_ms;
    m_ttl = m_emit_loop_ms;

    m_animation_frames = animation_frames;

    m_burst_time_pct = burst_time_pct;
    m_burst_particles = burst_particles;

    m_has_burst = false;


}

void Emitter::Update(int max_particles, float min_angle, float max_angle,
                    Uint32 particle_lifetime, float acceleration, bool alpha_fade,
                    float min_starting_velocity, float max_starting_velocity,
                    Uint32 emission_rate, int x_pos, int y_pos, float radius,
                    /* new parameters here */
                    float min_start_scale, float max_start_scale,
                    float min_end_scale, float max_end_scale,
                    Uint32 start_color, Uint32 end_color,
                    float burst_time_pct, Uint32 burst_particles, 
                    bool loop, bool align_rotation, Uint32 emit_time_ms, 
                    Uint32 animation_frames ) {

    if( min_starting_velocity > max_starting_velocity ) {
        m_min_starting_velocity = max_starting_velocity;
        m_max_starting_velocity = min_starting_velocity;
    }
    else {
        m_min_starting_velocity = min_starting_velocity;
        m_max_starting_velocity = max_starting_velocity;
    }

    m_active = true;
    m_has_burst = false;
    m_max_particles = max_particles;
    m_min_angle = min_angle;
    m_max_angle = max_angle;
    m_emission_rate = emission_rate;
    m_emission_time_ms = 1000 / m_emission_rate;
    m_position.x = (float)x_pos;
    m_position.y = (float)y_pos;
    m_radius = radius;

    /* new values */
    m_min_start_scale = min_start_scale;
    m_max_start_scale = max_start_scale;
    m_min_end_scale = min_end_scale;
    m_max_end_scale = max_end_scale;
    m_start_color = start_color;
    m_end_color = end_color;
    m_burst_time_pct = burst_time_pct;
    m_burst_particles = burst_particles;
    m_loop = loop;
    m_align_rotation = align_rotation;
    m_emit_loop_ms = emit_time_ms;
    m_ttl = m_emit_loop_ms;
    m_animation_frames = animation_frames;

    if( m_particle_pool.size() > m_max_particles ) {
        m_particle_pool.resize( m_max_particles );
    }
    else if( m_max_particles > m_particle_pool.size() ) {
        while( m_max_particles > m_particle_pool.size() ) {
            m_particle_pool.push_back(
                new Particle(   m_sprite_texture, particle_lifetime, acceleration, 
                                alpha_fade, m_sprite_width, m_sprite_height, m_align_rotation,
                                m_start_color, m_end_color, m_animation_frames )
            );
        }
    }

    Particle* particle;
    std::vector<Particle*>::iterator it;

    for( it = m_particle_pool.begin(); it != m_particle_pool.end(); it++ ) {
        particle = *it;
        particle->Update( particle_lifetime, acceleration, alpha_fade, m_align_rotation,
                          m_start_color, m_end_color, m_animation_frames );
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

    if( m_active == true ) {


        m_next_emission -= diff_time;
        m_ttl -= diff_time;

        if( m_ttl <= 0 ) {
            if( m_loop ) {
                m_ttl = m_emit_loop_ms;
                m_has_burst = false;
            }
            else {
                m_active = false;
            }
        }

        if( m_burst_particles > 0 && m_has_burst == false ) {
            if( (float)m_ttl / (float)m_emit_loop_ms <= 1.0 - m_burst_time_pct ) {
                m_has_burst = true;
                m_next_emission -= m_burst_particles * m_emission_time_ms;
            }
        }

        while( m_next_emission <= 0 ) {
            m_next_emission += m_emission_time_ms;

            particle = GetFreeParticle();

            if( particle != NULL ) {
                Point spawn_point;
                spawn_point.x = get_random_float( 0.0, m_radius ); 
                Point velocity_point;
                velocity_point.x = get_random_float( m_min_starting_velocity, m_max_starting_velocity ); 

                float angle = get_random_float( m_min_angle, m_max_angle ); 

                float start_scale = get_random_float( m_min_start_scale, m_max_start_scale );
                float end_scale = get_random_float( m_min_end_scale, m_max_end_scale );

                velocity_point.Rotate(angle);
                spawn_point.Rotate( angle );

                spawn_point.x += m_position.x;
                spawn_point.y += m_position.y;

                particle->Spawn(spawn_point.x, spawn_point.y, velocity_point.x, velocity_point.y, 
                                start_scale, end_scale,
                                (int)(angle / 3.14159 * 180.0 + 360.0) % 360 );
            }
            else {
                m_next_emission = m_emission_time_ms;
            }
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