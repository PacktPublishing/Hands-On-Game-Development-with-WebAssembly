#include "game.hpp"


Projectile::Projectile() {
    m_Active = false;
    m_X = 0.0;
    m_Y = 0.0;
    m_VX = 0.0;
    m_VY = 0.0;
    
    SDL_Surface *temp_surface = IMG_Load( c_SpriteFile );
    
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

void Projectile::Move() {
    m_X += m_VX;
    m_Y += m_VY;

    m_TTL -= diff_time;

    if( m_TTL < 0 ) {
        m_Active = false;
        m_TTL = 0;
    }
}

void Projectile::Render() {
    dest.x = m_X;
    dest.y = m_Y;
    dest.w = c_Width;
    dest.h = c_Height;

    int return_val = SDL_RenderCopy( renderer, m_SpriteTexture, 
                                        NULL, &dest );

    if( return_val != 0 ) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
    }

}

void Projectile::Launch(double x, double y, double dx, double dy) {
    m_X = x;
    m_Y = y;

    m_VX = velocity * dx;
    m_VY = velocity * dy;
    m_TTL = alive_time;
    m_Active = true;
}
