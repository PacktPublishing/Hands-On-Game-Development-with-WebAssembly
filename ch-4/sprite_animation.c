#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>
#include <stdio.h>

#define SPRITE_FILE "sprites/Franchise1.png"
#define EXP_FILE "sprites/FranchiseExplosion%d.png"
#define FRAME_COUNT 7

int current_frame = 0;
Uint32 last_time;
Uint32 current_time;
Uint32 ms_per_frame = 100; // animate at 10 fps

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Rect dest = {.x = 160, .y = 100, .w = 0, .h = 0 };

SDL_Texture *sprite_texture;
SDL_Texture *temp_texture;

SDL_Texture* anim[FRAME_COUNT];

void show_animation() {
    current_time = SDL_GetTicks();

    int ms = current_time - last_time;
    if( ms < ms_per_frame) {
        return;
    }
    if( current_frame >= FRAME_COUNT ) {
        SDL_RenderClear( renderer );
        return;
    }

    last_time = current_time;

    SDL_RenderClear( renderer );

    temp_texture = anim[current_frame++];

    SDL_QueryTexture( temp_texture, 
                        NULL, NULL, 
                        &dest.w, &dest.h ); // query the width and height

    dest.x = 160 - dest.w / 2;
    dest.y = 100 - dest.h / 2;
    
    SDL_RenderCopy( renderer, temp_texture, NULL, &dest );

    SDL_RenderPresent( renderer );
}

int main() {
    char explosion_file_string[40];

    SDL_Init( SDL_INIT_VIDEO );

    SDL_CreateWindowAndRenderer( 320, 200, 0, &window, &renderer );

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );

    SDL_Surface *temp_surface = IMG_Load( SPRITE_FILE );
    
    if( !temp_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return 0;
    }

    sprite_texture = SDL_CreateTextureFromSurface( renderer, temp_surface );

    SDL_FreeSurface( temp_surface );

    for( int i = 1; i <= FRAME_COUNT; i++ ) {
        sprintf( explosion_file_string, EXP_FILE, i );

        SDL_Surface *temp_surface = IMG_Load( explosion_file_string );
        
        if( !temp_surface ) {
            printf("failed to load image: %s\n", IMG_GetError() );
            return 0;
        }
        temp_texture = SDL_CreateTextureFromSurface( renderer, temp_surface );
        anim[i-1] = temp_texture;

        SDL_FreeSurface( temp_surface );
    }

    SDL_QueryTexture( sprite_texture, 
                        NULL, NULL, 
                        &dest.w, &dest.h ); // query the width and height

    dest.x -= dest.w / 2;
    dest.y -= dest.h / 2;
    
    SDL_RenderCopy( renderer, sprite_texture, NULL, &dest );

    SDL_RenderPresent( renderer );

    last_time = SDL_GetTicks();

    emscripten_set_main_loop(show_animation, 0, 0);

    return 1;
}