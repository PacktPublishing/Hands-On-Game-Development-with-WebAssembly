#include "game.hpp"

UIButton::UIButton( int x, int y, 
                    char* file_name, char* hover_file_name, char* click_file_name,
                    void (*callback)() ) {
    m_Callback = callback;
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


    temp_surface = IMG_Load( click_file_name );
    if( !temp_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return;
    }
    else { 
        printf("success creating ui button click surface\n");
    }
    m_ClickTexture = SDL_CreateTextureFromSurface( renderer, temp_surface );

    if( !m_ClickTexture ) {
        return;
    }
    SDL_FreeSurface( temp_surface );

    temp_surface = IMG_Load( hover_file_name );
    if( !temp_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return;
    }
    else { 
        printf("success creating ui button hover surface\n");
    }
    m_HoverTexture = SDL_CreateTextureFromSurface( renderer, temp_surface );

    if( !m_HoverTexture ) {
        return;
    }
    SDL_FreeSurface( temp_surface );

    m_dest.x -= m_dest.w / 2;
    m_dest.y -= m_dest.h / 2;

    m_Hover = false;
    m_Click = false;
    m_Active = true;
}

void UIButton::MouseMove(int x, int y) {
    if( x >= m_dest.x && x <= m_dest.x + m_dest.w &&
        y >= m_dest.y && y <= m_dest.y + m_dest.h ) {
        m_Hover = true;
    }
    else {
        m_Hover = false;
    }
}

void UIButton::MouseClick(int x, int y) {
    if( x >= m_dest.x && x <= m_dest.x + m_dest.w &&
        y >= m_dest.y && y <= m_dest.y + m_dest.h ) {
        m_Click = true;
    }
    else {
        m_Click = false;
    }
}

void UIButton::MouseUp(int x, int y) {
    if( x >= m_dest.x && x <= m_dest.x + m_dest.w &&
        y >= m_dest.y && y <= m_dest.y + m_dest.h ) {
        if( m_Callback != NULL ) {
            m_Callback();
        }
    }

    m_Click = false;
}


void UIButton::KeyDown( SDL_Keycode key ) {
    if( key == SDLK_RETURN) {
        if( m_Callback != NULL ) {
            m_Callback();
        }
    }
}

void UIButton::RenderUI() {

    if( m_Active == false ) {
        return;
    }

    if( m_Click == true ) {
        render_manager->RenderUI( m_ClickTexture, NULL, &m_dest, 0.0,
                                    0xff, 0xff, 0xff, 0xff );
    }
    else if( m_Hover == true ) {
        render_manager->RenderUI( m_HoverTexture, NULL, &m_dest, 0.0,
                                    0xff, 0xff, 0xff, 0xff );
    }
    else {
        render_manager->RenderUI( m_SpriteTexture, NULL, &m_dest, 0.0,
                                    0xff, 0xff, 0xff, 0xff );
    }

}

