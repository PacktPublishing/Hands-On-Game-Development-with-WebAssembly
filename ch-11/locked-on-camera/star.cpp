#include "game.hpp"

#define STAR_SPRITE_FILE "/sprites/rotating-star.png"
#define FLARE_FILE (char*)"/sprites/flare.png"

Star::Star()  : Collider(32.0) {
    SDL_Surface *temp_surface = IMG_Load( STAR_SPRITE_FILE );
    
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

    m_Radius = 36;

    m_Position.x = LEVEL_WIDTH / 2;
    m_Position.y = LEVEL_HEIGHT / 2;

    m_dest.x = m_Position.x - m_Radius / 2;
    m_dest.y = m_Position.y - m_Radius / 2;

    m_FlareList.push_back( 
        new Emitter(
            FLARE_FILE, 100, 160, 220,
            1500, 0.05, true,
            30, 40,
            1, m_Position.x+8, m_Position.y+8, 10,
            0.1, 0.2,
            0.5, 1.0,
            0xffffff, 0xffffff,
            0.1, 50, 
            true, true, 
            4409, 1
        )
    );

    m_FlareList.push_back( 
        new Emitter(
            FLARE_FILE, 100, 220, 280,
            1500, 0.05, true,
            30, 40,
            1, m_Position.x+8, m_Position.y+8, 10,
            0.1, 0.2,
            0.5, 1.0,
            0xffffff, 0xffffff,
            0.0, 50, 
            true, true, 
            3571, 1
        )
    );

    m_FlareList.push_back( 
        new Emitter(
            FLARE_FILE, 100, 280, 360,
            1500, 0.05, true,
            30, 40,
            1, m_Position.x+8, m_Position.y+8, 10,
            0.1, 0.2,
            0.5, 1.0,
            0xffffff, 0xffffff,
            0.2, 50, 
            true, true, 
            3989, 1
        )
    );

    m_FlareList.push_back( 
        new Emitter(
            FLARE_FILE, 100, 0, 60,
            1500, 0.05, true,
            30, 40,
            1, m_Position.x+8, m_Position.y+8, 10,
            0.1, 0.2,
            0.5, 1.0,
            0xffffff, 0xffffff,
            0.1, 50, 
            true, true, 
            3371, 1
        )
    );

    m_FlareList.push_back( 
        new Emitter(
            FLARE_FILE, 100, 60, 100,
            1500, 0.05, true,
            30, 40,
            1, m_Position.x+8, m_Position.y+8, 10,
            0.1, 0.2,
            0.5, 1.0,
            0xffffff, 0xffffff,
            0.3, 50, 
            true, true, 
            4637, 1
        )
    );

}
                    
void Star::Move() {
    m_NextFrameTime -= diff_time;

    if( m_NextFrameTime <= 0 ) {
        ++m_CurrentFrame;
        m_NextFrameTime = ms_per_frame;
        if( m_CurrentFrame >= 8 ) {
            m_CurrentFrame = 0;
        }
    }
}

void Star::Render() {

    Emitter* flare;
    std::vector<Emitter*>::iterator it;

    for( it = m_FlareList.begin(); it != m_FlareList.end(); it++ ) {
        flare = *it;
        flare->Move();
    }

    m_src.x = m_dest.w * m_CurrentFrame;

    render_manager->Render( m_SpriteTexture, &m_src, &m_dest );
}
