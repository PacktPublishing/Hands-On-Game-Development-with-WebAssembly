#include <SDL2/SDL.h>
#include <emscripten.h>
#include <stdio.h>
#include <stdbool.h>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

bool left_key_down = false;
bool right_key_down = false;
bool up_key_down = false;
bool down_key_down = false;

void input_loop() {
     if( SDL_PollEvent( &event ) ){
        switch( event.type ){
            case SDL_KEYDOWN:
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        if( !left_key_down ) {
                            printf("left arrow key down\n");
                        }
                        left_key_down = true;
                        break;
                    case SDLK_RIGHT:
                        if( !right_key_down ) {
                            printf("right arrow key down\n");
                        }
                        right_key_down = true;
                        break;
                    case SDLK_UP:
                        if( !up_key_down ) {
                            printf("up arrow key down\n");
                        }
                        up_key_down = true;
                        break;
                    case SDLK_DOWN:
                        if( !down_key_down ) {
                            printf("down arrow key down\n");
                        }
                        down_key_down = true;
                        break;
                    default:
                        printf("unknown key down\n");
                        break;
                }
                break;

            case SDL_KEYUP:
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        if( left_key_down ) {
                            printf("left arrow key up\n");
                        }
                        left_key_down = false;
                        break;
                    case SDLK_RIGHT:
                        if( right_key_down ) {
                            printf("right arrow key up\n");
                        }
                        right_key_down = false;
                        break;
                    case SDLK_UP:
                        if( up_key_down ) {
                            printf("up arrow key up\n");
                        }
                        up_key_down = false;
                        break;
                    case SDLK_DOWN:
                        if( down_key_down ) {
                            printf("down arrow key up\n");
                        }
                        down_key_down = false;
                        break;
                    default:
                        printf("unknown key up\n");
                        break;
                }
                break;
            
            default:
                break;
        }
    }
}

int main() {
    SDL_Init( SDL_INIT_VIDEO );

    SDL_CreateWindowAndRenderer( 320, 200, 0, &window, &renderer );

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );

    SDL_RenderPresent( renderer );

    emscripten_set_main_loop(input_loop, 0, 0);

    return 1;
}