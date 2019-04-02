#include "game.hpp"

UISprite::UISprite( int x, int y, char* file_name ) {
    SDL_Surface *temp_surface = IMG_Load( file_name );
    m_dest.x = x;
    m_dest.y = y;
    
    if( !temp_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return;
    }
    else { 
        printf("success creating ui button surface\n");
    }

    m_SpriteTexture = SDL_CreateTextureFromSurface( renderer, temp_surface );

    if( !m_SpriteTexture ) {
        return;
    }

    SDL_QueryTexture( m_SpriteTexture, 
                      NULL, NULL, 
                      &m_dest.w, &m_dest.h ); 

    SDL_FreeSurface( temp_surface );

    m_dest.x -= m_dest.w / 2;
    m_dest.y -= m_dest.h / 2;
}

void UISprite::RenderUI() {
    if( m_Active == false ) {
        return;
    }

    render_manager->RenderUI( m_SpriteTexture, NULL, &m_dest, 0.0,
                                0xff, 0xff, 0xff, 0xff );
}