#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define SPRITE_FILE "sprites/Franchise.png"

#define PI 3.14159
#define TWO_PI 6.28318

#define MAX_VELOCITY 2.0

Uint32 last_time;
Uint32 last_frame_time;
Uint32 current_time;

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Rect dest = {.x = 160, .y = 100, .w = 16, .h = 16 };

SDL_Texture *sprite_texture;

SDL_Event event;

bool left_key_down = false;
bool right_key_down = false;
bool up_key_down = false;
bool down_key_down = false;

float player_x = 160.0;
float player_y = 100.0;
float player_rotation = PI;
float player_dx = 0.0;
float player_dy = 1.0;
float player_vx = 0.0;
float player_vy = 0.0;

float delta_time = 0.0;

void rotate_left() {
    player_rotation -= delta_time;

    if( player_rotation < 0.0 ) {
        player_rotation += TWO_PI;
    }
    player_dx = sin(player_rotation);
    player_dy = -cos(player_rotation);
}

void rotate_right() {
    player_rotation += delta_time;

    if( player_rotation >= TWO_PI ) {
        player_rotation -= TWO_PI;
    }
    player_dx = sin(player_rotation);
    player_dy = -cos(player_rotation);
}

void accelerate() {
    player_vx += player_dx * delta_time;
    player_vy += player_dy * delta_time;
}

void decelerate() {
    player_vx -= (player_dx * delta_time) / 2.0;
    player_vy -= (player_dy * delta_time) / 2.0;
}


void cap_velocity() {
    float vel = sqrt( player_vx * player_vx + player_vy * player_vy );

    if( vel > MAX_VELOCITY ) {
        player_vx /= vel;
        player_vy /= vel;

        player_vx *= MAX_VELOCITY;
        player_vy *= MAX_VELOCITY;
    }
}

void move() {
    current_time = SDL_GetTicks();

    delta_time = (float)(current_time - last_time) / 1000.0;
    last_time = current_time;

    if( left_key_down ) {
        rotate_left();
    }
    if( right_key_down ) {
        rotate_right();
    }

    if( up_key_down ) {
        accelerate();
    }
    if( down_key_down ) {
        decelerate();
    }

    cap_velocity();
    player_x += player_vx;

    if( player_x > 320 ) {
        player_x = -16;
    }
    else if( player_x < -16 ) {
        player_x = 320;
    }

    player_y += player_vy;

    if( player_y > 200 ) {
        player_y = -16;
    }
    else if( player_y < -16 ) {
        player_y = 200;
    }

}

void input() {
     if( SDL_PollEvent( &event ) ){
        switch( event.type ){
            case SDL_KEYDOWN:
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        left_key_down = true;
                        break;
                    case SDLK_RIGHT:
                        right_key_down = true;
                        break;
                    case SDLK_UP:
                        up_key_down = true;
                        break;
                    case SDLK_DOWN:
                        down_key_down = true;
                        break;
                    default:
                        break;
                }
                break;

            case SDL_KEYUP:
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        left_key_down = false;
                        break;
                    case SDLK_RIGHT:
                        right_key_down = false;
                        break;
                    case SDLK_UP:
                        up_key_down = false;
                        break;
                    case SDLK_DOWN:
                        down_key_down = false;
                        break;
                    default:
                        break;
                }
                break;
            
            default:
                break;
        }
    }
}

void render() {
    SDL_RenderClear( renderer );

    dest.x = player_x;
    dest.y = player_y;

    float degrees = (player_rotation / PI) * 180.0;

    SDL_RenderCopyEx( renderer, sprite_texture, 
                        NULL, &dest, 
                        degrees, NULL, SDL_FLIP_NONE );

    SDL_RenderPresent( renderer );
}

void game_loop() {
    input();
    move();
    render();
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

    last_frame_time = last_time = SDL_GetTicks();

    emscripten_set_main_loop(game_loop, 0, 0);

    return 1;
}