#include "game.hpp"
#define LOCATOR_SPRITE_FILE (char*)"/sprites/locator.png"

Locator::Locator() {
    SDL_Surface *temp_surface = IMG_Load( LOCATOR_SPRITE_FILE );
    
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

    m_Rotation = 0.0;
    m_Active = false;
}

void Locator::Move() {
    
    SetActive();

    Vector2D end_position = enemy->m_Position - camera->m_Position;
    Vector2D start_position;

    start_position.x = CANVAS_WIDTH/2; 
    start_position.y = CANVAS_HEIGHT/2;

    Vector2D dir = end_position - start_position;

    if( dir.x > LEVEL_WIDTH/2 ) {
        dir.x -= LEVEL_WIDTH;
    }
    else if( dir.x < -LEVEL_WIDTH/2 ) {
        dir.x += LEVEL_WIDTH;
    }

    if( dir.y > LEVEL_HEIGHT ) {
        dir.y -= LEVEL_HEIGHT;
    }
    else if( dir.y < -LEVEL_HEIGHT/2 ) {
        dir.y += LEVEL_HEIGHT;
    }

    m_ColorFlux = (int)(255.0 * (LEVEL_HEIGHT * LEVEL_HEIGHT - dir.MagSQ()) / (LEVEL_HEIGHT * LEVEL_HEIGHT));

    if( m_ColorFlux < 0 ) {
        m_ColorFlux = 0;
    }
    m_ColorFlux = 255 - m_ColorFlux;

    m_Rotation = dir.FindAngle(); 
    dir.Normalize();

    if( abs(dir.x) * 0.75 >= abs(dir.y) ) {
        if( dir.x > 0 ) {
            m_Position.x = CANVAS_WIDTH - 32;
        }
        else {
            m_Position.x = 0;
        }
        m_Position.y = CANVAS_HEIGHT / 2 + (CANVAS_HEIGHT / 2) * (dir.y / abs(dir.x*0.75));  
    }
    else {
        if( dir.y > 0 ) {
            m_Position.y = CANVAS_HEIGHT - 32;
        }
        else {
            m_Position.y = 0;
        }
        m_Position.x = CANVAS_WIDTH / 2 + (CANVAS_WIDTH / 2) * (dir.x / abs(dir.y/0.75));  
    }

    if( m_Position.x < 0 ) {
        m_Position.x = 0;
    }
    else if( m_Position.x > CANVAS_WIDTH - 32 ) {
        m_Position.x = CANVAS_WIDTH - 32;
    }

    if( m_Position.y < 0 ) {
        m_Position.y = 0;
    }
    else if( m_Position.y > CANVAS_HEIGHT - 32 ) {
        m_Position.y = CANVAS_HEIGHT - 32;
    }
}

void Locator::SetActive() {
    Vector2D camera_dest = enemy->m_Position;

    if( camera_dest.x <= CANVAS_WIDTH && 
        camera->m_Position.x >= LEVEL_WIDTH - CANVAS_WIDTH ) {
        camera_dest.x += (float)LEVEL_WIDTH;
    }
    else if( camera_dest.x >= LEVEL_WIDTH - CANVAS_WIDTH && 
        camera->m_Position.x <= CANVAS_WIDTH ) {
        camera_dest.x -= (float)LEVEL_WIDTH;
    }

    if( camera_dest.y <= CANVAS_HEIGHT && 
        camera->m_Position.y >= LEVEL_HEIGHT - CANVAS_HEIGHT ) {
        camera_dest.y += (float)LEVEL_HEIGHT;
    }
    else if( camera_dest.y >= LEVEL_HEIGHT - CANVAS_HEIGHT && 
        camera->m_Position.y <= CANVAS_HEIGHT ) {
        camera_dest.y -= (float)LEVEL_HEIGHT;
    }

    camera_dest.x -= (int)camera->m_Position.x;
    camera_dest.y -= (int)camera->m_Position.y;

    if( camera_dest.x < 0 || camera_dest.x > CANVAS_WIDTH ||
        camera_dest.y < 0 || camera_dest.y > CANVAS_HEIGHT ) {
        m_Active = true;
    }
    else {
        m_Active = false;
    }
}

void Locator::Render() {
    if( m_Active == false ) {
        return;
    }

    m_dest.x = (int)m_Position.x;
    m_dest.y = (int)m_Position.y;

    render_manager->RenderUI( m_SpriteTexture, NULL, &m_dest, m_Rotation,
                                255, 255, m_ColorFlux, m_ColorFlux );
    
}
