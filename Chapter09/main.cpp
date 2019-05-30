#include "game.hpp"

PlayerShip* player;
EnemyShip* enemy;
ProjectilePool* projectile_pool;
Uint32 last_time;
Uint32 last_frame_time;
Uint32 current_time;
Uint32 ms_per_frame = 100; // animate at 10 fps

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Rect dest = {.x = 160, .y = 100, .w = 16, .h = 16 };

SDL_Texture *sprite_texture;

SDL_Event event;

bool left_key_down = false;
bool right_key_down = false;
bool up_key_down = false;
bool down_key_down = false;
bool space_key_down = false;

double delta_time = 0.0;
int diff_time = 0;

float get_random_float( float min, float max ) {
    int int_min = (int)(min * 1000);
    int int_max = (int)(max * 1000);

    if( int_min > int_max ) {
        int temp = int_max;
        int_max = int_min;
        int_min = temp;
    }

    int int_diff = int_max - int_min;
    int int_rand = (int_diff == 0) ? 0 : rand() % int_diff;
    int_rand += int_min;

    return (float)int_rand / 1000.0;
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
                    case SDLK_SPACE:
                        space_key_down = true;
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
                    case SDLK_SPACE:
                        space_key_down = false;
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

void move() {
    player->Move();
    enemy->Move();
    projectile_pool->MoveProjectiles();
}

void render() {
    SDL_RenderClear( renderer );

    player->Render();
    enemy->Render();
    projectile_pool->RenderProjectiles();

    SDL_RenderPresent( renderer );
}

void game_loop() {
    current_time = SDL_GetTicks();

    diff_time = current_time - last_time;
    delta_time = (double)diff_time / 1000.0;
    last_time = current_time;

    input();
    move();
    render();
}

int main() {
    SDL_Init( SDL_INIT_VIDEO );

    int return_val = SDL_CreateWindowAndRenderer( 320, 200, 0, &window, &renderer );

    if( return_val != 0 ) {
        printf("Error creating renderer %d: %s\n", return_val, IMG_GetError() );
        return 0;
    }

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );

    last_frame_time = last_time = SDL_GetTicks();

    player = new PlayerShip();
    enemy = new EnemyShip();
    projectile_pool = new ProjectilePool();

    emscripten_set_main_loop(game_loop, 0, 0);

    return 1;
}