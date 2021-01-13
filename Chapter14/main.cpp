#include "game.hpp"

PlayerShip* player;
EnemyShip* enemy;
Star* star;
Camera* camera;
RenderManager* render_manager;
Locator* locator;

SDL_AudioDeviceID device_id;

Audio* enemy_laser_snd;
Audio* player_laser_snd;
Audio* small_explosion_snd;
Audio* large_explosion_snd;
Audio* hit_snd;

ProjectilePool* projectile_pool;
std::vector<Asteroid*> asteroid_list; 

UIButton* play_btn;
UIButton* play_again_btn;

Uint32 last_time;
Uint32 last_frame_time;
Uint32 current_time;
Uint32 ms_per_frame = 100; // animate at 10 fps
int transition_time = 0;
bool you_win = false;

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Rect dest = {.x = 160, .y = 100, .w = 16, .h = 16 };

SDL_Texture *sprite_texture;

UISprite *you_win_sprite;
UISprite *game_over_sprite;

SDL_Event event;

SCREEN_STATE current_screen = START_SCREEN;

bool left_key_down = false;
bool right_key_down = false;
bool up_key_down = false;
bool down_key_down = false;
bool space_key_down = false;
bool f_key_down = false;

float delta_time = 0.0;
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

void start_input() {
    if(SDL_PollEvent( &event ) ) 
	{
		switch (event.type) 
		{
            case SDL_MOUSEMOTION:
			{
                int x_val = 0; 
                int y_val = 0;  
                SDL_GetMouseState( &x_val, &y_val );
                play_btn->MouseMove(x_val, y_val);
            }
            case SDL_MOUSEBUTTONDOWN:
			{
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
					{
                        int x_val = 0; 
                        int y_val = 0;  
                        SDL_GetMouseState( &x_val, &y_val );

                        play_btn->MouseClick(x_val, y_val);
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
            case SDL_MOUSEBUTTONUP:
			{
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
					{
                        int x_val = 0; 
                        int y_val = 0;  
                        SDL_GetMouseState( &x_val, &y_val );

                        play_btn->MouseUp(x_val, y_val);
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
            case SDL_KEYDOWN:
            {
                play_btn->KeyDown( event.key.keysym.sym );
            }
        }
    }
}

void end_input() {
    if(SDL_PollEvent( &event ) ) 
	{
		switch (event.type) 
		{
            case SDL_MOUSEMOTION:
			{
                int x_val = 0; 
                int y_val = 0;  
                SDL_GetMouseState( &x_val, &y_val );
                play_again_btn->MouseMove(x_val, y_val);
            }
            case SDL_MOUSEBUTTONDOWN:
			{
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
					{
                        int x_val = 0; 
                        int y_val = 0;  
                        SDL_GetMouseState( &x_val, &y_val );

                        play_again_btn->MouseClick(x_val, y_val);
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
            case SDL_MOUSEBUTTONUP:
			{
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
					{
                        int x_val = 0; 
                        int y_val = 0;  
                        SDL_GetMouseState( &x_val, &y_val );

                        play_again_btn->MouseUp(x_val, y_val);
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
            case SDL_KEYDOWN:
            {
                printf("SDL_KEYDOWN\n");
                play_again_btn->KeyDown( event.key.keysym.sym );
            }
        }
    }
}


void play_input() {
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
                    case SDLK_f:
                        f_key_down = true;
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
                    case SDLK_f:
                        f_key_down = false;
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

    Asteroid* asteroid;
    std::vector<Asteroid*>::iterator it;
    int i = 0;
    for( it = asteroid_list.begin(); it != asteroid_list.end(); it++ ) {
        asteroid = *it;
        if( asteroid->m_Active ) {
            asteroid->Move();
        }
    }
    star->Move();
    camera->Move();
    locator->Move();
}

void start_render() {
    render_manager->RenderStartBackground();
    play_btn->RenderUI();
}

void play_render() {
    render_manager->RenderBackground();

    player->Render();
    enemy->Render();
    projectile_pool->RenderProjectiles();

    Asteroid* asteroid;
    std::vector<Asteroid*>::iterator it;

    for( it = asteroid_list.begin(); it != asteroid_list.end(); it++ ) {
        asteroid = *it;

        asteroid->Render();
    }

    star->Render();
    locator->Render();
}

void collisions() {
    Asteroid* asteroid;
    std::vector<Asteroid*>::iterator ita;

    if( player->m_CurrentFrame == 0 && 
        player->CompoundHitTest( star ) ) {
        player->m_CurrentFrame = 1;
        player->m_NextFrameTime = ms_per_frame;

        player->m_Explode->Run(); // added 
        current_screen = GAME_OVER_SCREEN;
        large_explosion_snd->Play();

    }
    if( enemy->m_CurrentFrame == 0 && 
        enemy->CompoundHitTest( star ) ) {
        enemy->m_CurrentFrame = 1;
        enemy->m_NextFrameTime = ms_per_frame;

        current_screen = YOU_WIN_SCREEN;
        enemy->m_Explode->Run(); // added 
        large_explosion_snd->Play();
    }

    Projectile* projectile;
    std::vector<Projectile*>::iterator it;

    for( it = projectile_pool->m_ProjectileList.begin(); it != projectile_pool->m_ProjectileList.end(); it++ ) {
        projectile = *it;
        if(  projectile->m_CurrentFrame == 0 && projectile->m_Active ) {

            for( ita = asteroid_list.begin(); ita != asteroid_list.end(); ita++ ) {
                asteroid = *ita;
                if( asteroid->m_Active ) {
                    if( asteroid->HitTest( projectile ) ) {
                        asteroid->ElasticCollision( projectile );
                        projectile->m_CurrentFrame = 1;
                        projectile->m_NextFrameTime = ms_per_frame;
                        small_explosion_snd->Play();
                    }
                }
            }

            if( projectile->HitTest( star ) ){
                projectile->m_CurrentFrame = 1;
                projectile->m_NextFrameTime = ms_per_frame;
                small_explosion_snd->Play();
            }

            else if( player->m_CurrentFrame == 0 &&
                    ( projectile->HitTest( player ) ||
                      player->CompoundHitTest( projectile ) ) ) {

                if( player->m_Shield->m_Active == false ) {
                    player->m_CurrentFrame = 1;
                    player->m_NextFrameTime = ms_per_frame;
                    
                    current_screen = GAME_OVER_SCREEN;
                    player->m_Explode->Run(); 
                    large_explosion_snd->Play();
                }
                else {
                    hit_snd->Play();
                    player->ElasticCollision( projectile );
                }

                projectile->m_CurrentFrame = 1;
                projectile->m_NextFrameTime = ms_per_frame;
            }
            else if( enemy->m_CurrentFrame == 0 &&
                    ( projectile->HitTest( enemy ) ||
                      enemy->CompoundHitTest( projectile ) ) ) {

                if( enemy->m_Shield->m_Active == false ) {
                    enemy->m_CurrentFrame = 1;
                    enemy->m_NextFrameTime = ms_per_frame;

                    current_screen = YOU_WIN_SCREEN;
                    enemy->m_Explode->Run(); 
                    large_explosion_snd->Play();
                    enemy->m_Shield->m_ttl -= 1000;
                }
                else {
                    enemy->ElasticCollision( projectile );
                    hit_snd->Play();
                }
                projectile->m_CurrentFrame = 1;
                projectile->m_NextFrameTime = ms_per_frame;
            }
        }
    }

    for( ita = asteroid_list.begin(); ita != asteroid_list.end(); ita++ ) {
        asteroid = *ita;
        if( asteroid->m_Active ) {
            if( asteroid->HitTest( star ) ) {
                asteroid->Explode();
                small_explosion_snd->Play();
            }
        }
        else {
            continue;
        }
        
        if( player->m_CurrentFrame == 0 &&
            asteroid->m_Active &&
                ( asteroid->HitTest( player ) ||
                  player->CompoundHitTest( asteroid ) ) ) {
                if( player->m_Shield->m_Active == false ) {
                    player->m_CurrentFrame = 1;
                    player->m_NextFrameTime = ms_per_frame;
                    
                    player->m_Explode->Run(); 
                    current_screen = GAME_OVER_SCREEN;
                    large_explosion_snd->Play();
                }
                else {
                    player->ElasticCollision( asteroid );
                    small_explosion_snd->Play();
                }
        }
        if( enemy->m_CurrentFrame == 0 &&
            asteroid->m_Active &&
                ( asteroid->HitTest( enemy ) ||
                  enemy->CompoundHitTest( asteroid ) ) ) {
                if( enemy->m_Shield->m_Active == false ) {
                    enemy->m_CurrentFrame = 1;
                    enemy->m_NextFrameTime = ms_per_frame;
                    
                    enemy->m_Explode->Run();
                    current_screen = YOU_WIN_SCREEN;
                    large_explosion_snd->Play();
                }
                else {
                    enemy->ElasticCollision( asteroid );
                    small_explosion_snd->Play();
                }
        }
    }

    Asteroid* asteroid_1;
    Asteroid* asteroid_2;

    std::vector<Asteroid*>::iterator ita_1;
    std::vector<Asteroid*>::iterator ita_2;

    for( ita_1 = asteroid_list.begin(); ita_1 != asteroid_list.end(); ita_1++ ) {
        asteroid_1 = *ita_1;
        if( !asteroid_1->m_Active ) {
            continue;
        }

        for( ita_2 = ita_1+1; ita_2 != asteroid_list.end(); ita_2++ ) {
            asteroid_2 = *ita_2;
            if( !asteroid_2->m_Active ) {
                continue;
            }

            if( asteroid_1->HitTest( asteroid_2 ) ) {
                asteroid_1->ElasticCollision( asteroid_2 );
            }
        }
    }
}

void draw_play_transition() {
    transition_time -= diff_time;

    if( transition_time <= 0 ) {
        current_screen = PLAY_SCREEN;
        return;
    }
    render_manager->RenderStartBackground(transition_time/4);
}

void game_loop() {
    current_time = SDL_GetTicks();

    diff_time = current_time - last_time;
    delta_time = diff_time / 1000.0;
    last_time = current_time;

    if( current_screen == START_SCREEN ) {
        start_input();
        start_render();
    }
    else if( current_screen == PLAY_SCREEN  || current_screen == PLAY_TRANSITION ) {
        play_input();
        move();
        collisions();
        play_render();

        if( current_screen == PLAY_TRANSITION ) {
            draw_play_transition();
        }
    }
    else if( current_screen == GAME_OVER_SCREEN ) {
        end_input();
        move();
        collisions();
        play_render();

        game_over_sprite->RenderUI();
        play_again_btn->RenderUI();
    }
    else if( current_screen == YOU_WIN_SCREEN ) {
        end_input();
        move();
        collisions();
        play_render();

        you_win_sprite->RenderUI();
        play_again_btn->RenderUI();
    }
    SDL_RenderPresent( renderer );
}

void play_click() {
    current_screen = PLAY_TRANSITION;
    transition_time = 1020;
}

void play_again_click() {
  EM_ASM(
    location.reload();
  );
}

int main() {
    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );

    int return_val = SDL_CreateWindowAndRenderer( CANVAS_WIDTH, CANVAS_HEIGHT, 0, &window, &renderer );

    if( return_val != 0 ) {
        printf("Error creating renderer %d: %s\n", return_val, IMG_GetError() );
        return 0;
    }

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );

    game_over_sprite = new UISprite( 400, 300, (char*)"/sprites/GameOver.png" );
    game_over_sprite->m_Active = true;
    you_win_sprite   = new UISprite( 400, 300, (char*)"/sprites/YouWin.png" );
    you_win_sprite->m_Active = true;

    last_frame_time = last_time = SDL_GetTicks();

    player = new PlayerShip();
    enemy = new EnemyShip();
    star = new Star();
    camera = new Camera(CANVAS_WIDTH, CANVAS_HEIGHT);
    render_manager = new RenderManager();
    locator = new Locator();

    enemy_laser_snd = new Audio(ENEMY_LASER, false);
    player_laser_snd = new Audio(PLAYER_LASER, false);
    small_explosion_snd = new Audio(SMALL_EXPLOSION, true);
    large_explosion_snd = new Audio(LARGE_EXPLOSION, true);
    hit_snd = new Audio(HIT, false);

    device_id = SDL_OpenAudioDevice(NULL, 0, &(enemy_laser_snd->spec), NULL, 0);

    if (device_id == 0) {
        printf("Failed to open audio: %s\n", SDL_GetError());
    }    
    SDL_PauseAudioDevice(device_id, 0);

    int asteroid_x = 0;
    int asteroid_y = 0;
    int angle = 0;
    // SCREEN 1
    for( int i_y = 0; i_y < 8; i_y++ ) {
        asteroid_y += 100;
        asteroid_y += rand() % 400;
        asteroid_x = 0;

        for( int i_x = 0; i_x < 12; i_x++ ) {
            asteroid_x += 66;
            asteroid_x += rand() % 400;
            int y_save = asteroid_y;
            asteroid_y += rand() % 400 - 200;
            angle = rand() % 359;

            asteroid_list.push_back( 
                new Asteroid(
                    asteroid_x, asteroid_y, 
                    get_random_float(0.5, 1.0), 
                    DEG_TO_RAD(angle)
                )
            );
            asteroid_y = y_save;
        }
    } 
    projectile_pool = new ProjectilePool();

    play_btn = new UIButton(400, 500, 
                    (char*)"/sprites/play_button.png", 
                    (char*)"/sprites/play_button_hover.png",
                    (char*)"/sprites/play_button_click.png",
                    play_click );

    play_again_btn = new UIButton(400, 500, 
                    (char*)"/sprites/play_again_button.png", 
                    (char*)"/sprites/play_again_button_hover.png",
                    (char*)"/sprites/play_again_button_click.png",
                    play_again_click );

    emscripten_set_main_loop(game_loop, 0, 0);

    return 1;
}