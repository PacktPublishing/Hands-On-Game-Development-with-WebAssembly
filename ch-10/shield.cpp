#include "game.hpp"

Shield::Shield( Ship* ship, const char* sprite_string ) : Collider(12.0) {
    m_Active = false;
    m_ttl = 25500;
    m_Ship = ship;
    m_CurrentFrame = 0;
    m_NextFrame = ms_per_frame;

    SDL_Surface *temp_surface = IMG_Load( sprite_string );

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
    
void Shield::Move() {
    if( m_Active ) {
        m_NextFrame -= diff_time;
        m_ttl -= diff_time;

        if( m_NextFrame <= 0 ) {

            m_NextFrame = ms_per_frame;
            m_CurrentFrame++;

            if( m_CurrentFrame >= 6 ) {
                m_CurrentFrame = 0;
            }
        }

        if( m_ttl <= 0 ) {
            m_Active = false;
        }
    }
}

void Shield::Render() {
    if( m_Active ) {
        int color_green = m_ttl / 100 + 1;
        int color_red = 255 - color_green;

        m_src.x = m_CurrentFrame * m_dest.w;

        m_dest.x = m_Ship->m_Position.x;
        m_dest.y = m_Ship->m_Position.y;

        SDL_SetTextureColorMod(m_SpriteTexture,
                                color_red,
                                color_green,
                                0 );

        SDL_RenderCopyEx( renderer, m_SpriteTexture, 
                          &m_src, &m_dest, 
                          RAD_TO_DEG(m_Ship->m_Rotation), 
                          NULL, SDL_FLIP_NONE );

    }
}

bool Shield::Activate() {
    if( m_ttl > 0 ) {
        m_Active = true;
    }
    else {
        m_Active = false;
        m_NextFrame = ms_per_frame;
        m_CurrentFrame = 0;
    }
    return m_Active;
}

void Shield::Deactivate() {
    m_Active = false;
}

/*
class Shield : public Collider {
    boolean m_Active;
    int m_ttl;
    Ship* m_Ship;
    SDL_Texture *m_SpriteTexture;

    SDL_Rect m_src =  {.x = 0, .y = 0, .w = 32, .h = 32 };
    SDL_Rect m_dest = {.x = 0, .y = 0, .w = 32, .h = 32 };

    Shield( Ship* ship );

    void Move();
    void Render();
};
*/