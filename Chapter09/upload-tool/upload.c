#include <emscripten.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window *window;
SDL_Renderer *renderer;
char* fileName;
SDL_Texture *sprite_texture = NULL;
SDL_Rect dest = {.x = 160, .y = 100, .w = 16, .h = 16 };

int sprite_x = 0;
int sprite_y = 0;


void add_image(char* file_name) {
    SDL_Surface *temp_surface = IMG_Load( file_name );
    
    if( !temp_surface ) {
        printf("failed to load image: %s\n", IMG_GetError() );
        return;
    }
    sprite_texture = SDL_CreateTextureFromSurface( renderer, temp_surface );

    SDL_FreeSurface( temp_surface );

    SDL_QueryTexture( sprite_texture, 
                        NULL, NULL, 
                        &dest.w, &dest.h );
}

void show_animation() {
    if( sprite_texture == NULL ) {
        return;
    }

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );

    sprite_x += 2;
    sprite_y++;

    if( sprite_x >= 800 ) {
        sprite_x = -dest.w;
    }
    if( sprite_y >= 600 ) {
        sprite_y = -dest.h;
    }


    dest.x = sprite_x;
    dest.y = sprite_y;

    SDL_RenderCopy( renderer, sprite_texture, NULL, &dest );

    SDL_RenderPresent( renderer );

}

int main() {
    printf("Enter Main\n");

    SDL_Init( SDL_INIT_VIDEO );

    int return_val = SDL_CreateWindowAndRenderer( 800, 600, 0, &window, &renderer );

    if( return_val != 0 ) {
        printf("Error creating renderer %d: %s\n", return_val, IMG_GetError() );
        return 0;
    }

    emscripten_set_main_loop(show_animation, 0, 0);
    printf("Exit Main\n");
    return 1;
}
