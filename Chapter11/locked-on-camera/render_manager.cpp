#include "game.hpp"
#define BACKGROUND_SPRITE_FILE (char*)"/sprites/starfield.png"

RenderManager::RenderManager() {
    SDL_Surface *temp_surface = IMG_Load( BACKGROUND_SPRITE_FILE );
    if( !temp_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return;
    }

    m_BackgroundTexture = SDL_CreateTextureFromSurface( renderer,
    temp_surface );

    if( !m_BackgroundTexture ) {
        printf("failed to create texture: %s\n", IMG_GetError() );
        return;
    }
    SDL_FreeSurface( temp_surface );
}

void RenderManager::RenderBackground() {
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
    SDL_Rect background_rect = {.x = 0, .y=0, .w=CANVAS_WIDTH, .h=CANVAS_HEIGHT};
 
    int start_x = (int)(camera->m_Position.x) % CANVAS_WIDTH;
    int start_y = (int)(camera->m_Position.y) % CANVAS_HEIGHT;
 
    background_rect.x -= start_x;
    background_rect.y -= start_y;
 
    SDL_RenderCopy( renderer, m_BackgroundTexture, NULL,
                    &background_rect );
    background_rect.x += CANVAS_WIDTH;

    SDL_RenderCopy( renderer, m_BackgroundTexture, NULL,
                    &background_rect );
    background_rect.x -= CANVAS_WIDTH;
    background_rect.y += CANVAS_HEIGHT;

    SDL_RenderCopy( renderer, m_BackgroundTexture, NULL,
                    &background_rect );

    background_rect.x += CANVAS_WIDTH;
    SDL_RenderCopy( renderer, m_BackgroundTexture, NULL,
                    &background_rect );
}

void RenderManager::Render( SDL_Texture *tex, SDL_Rect *src, SDL_Rect
                            *dest, float rad_rotation,int alpha, int red, int green, int blue ) {
    SDL_Rect camera_dest = *dest;

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
    SDL_SetTextureAlphaMod(tex, (Uint8)alpha );
    SDL_SetTextureColorMod(tex,
                            (Uint8)red,
                            (Uint8)green,
                            (Uint8)blue );

    if( rad_rotation != 0.0 ) {
        float degree_rotation = RAD_TO_DEG(rad_rotation);
        SDL_RenderCopyEx( renderer, tex, src, &camera_dest,
        degree_rotation, NULL, SDL_FLIP_NONE );
    }
    else {
        SDL_RenderCopy( renderer, tex, src, &camera_dest );
    }
}


void RenderManager::RenderUI( SDL_Texture *tex, SDL_Rect *src, SDL_Rect *dest, float rad_rotation,
                            int alpha, int red, int green, int blue ) {

    SDL_SetTextureAlphaMod(tex,
                            (Uint8)alpha );
    
    SDL_SetTextureColorMod(tex,
                            (Uint8)red,
                            (Uint8)green,
                            (Uint8)blue );  
    
    if( rad_rotation != 0.0 ) {
        double degree_rotation = RAD_TO_DEG(rad_rotation);

        SDL_RenderCopyEx( renderer, tex, src, dest, 
                            degree_rotation, NULL, SDL_FLIP_NONE );
    }
    else {
        SDL_RenderCopy( renderer, tex, src, dest );
    }

}
