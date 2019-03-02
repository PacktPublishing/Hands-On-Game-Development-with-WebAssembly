#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>
#include <stdio.h>

#define SPRITE_FILE "sprites/Franchise1.png"

int main() {
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Rect dest = {.x = 160, .y = 100, .w = 0, .h = 0 };

    SDL_Texture *texture;

    SDL_Init( SDL_INIT_VIDEO );

    SDL_CreateWindowAndRenderer( 320, 200, 0, &window, &renderer );

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );

    SDL_Surface *temp_surface = IMG_Load( SPRITE_FILE );
    
    if( !temp_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return 0;
    }
    
    texture = SDL_CreateTextureFromSurface( renderer, temp_surface );

    SDL_FreeSurface( temp_surface );

    SDL_QueryTexture( texture, 
                        NULL, NULL, 
                        &dest.w, &dest.h ); // query the width and height

    dest.x -= dest.w / 2;
    dest.y -= dest.h / 2;
    
    SDL_RenderCopy( renderer, texture, NULL, &dest );

    SDL_RenderPresent( renderer );
    
    SDL_Delay(5000);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 1;
}