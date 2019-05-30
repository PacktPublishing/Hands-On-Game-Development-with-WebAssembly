#include <SDL2/SDL.h>
#include <emscripten.h>
#include <stdio.h>
#include <stdbool.h>

#define ENEMY_LASER "/audio/enemy-laser.wav"
#define PLAYER_LASER "/audio/player-laser.wav"
#define LARGE_EXPLOSION "/audio/large-explosion.wav"
#define SMALL_EXPLOSION "/audio/small-explosion.wav"
#define HIT "/audio/hit.wav"

SDL_AudioDeviceID device_id;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

struct audio_clip {
    char file_name[100];
    SDL_AudioSpec spec;
    Uint32 len;
    Uint8 *buf;
} enemy_laser_snd, player_laser_snd, small_explosion_snd, large_explosion_snd, hit_snd;

void play_audio( struct audio_clip* clip ) {
    int success = SDL_QueueAudio(device_id, clip->buf, clip->len);
    if( success < 0 ) {
        printf("SDL_QueueAudio %s failed: %s\n", clip->file_name, SDL_GetError());
    }
}

void init_audio( char* file_name, struct audio_clip* clip ) {

    strcpy( clip->file_name, file_name );

    if( SDL_LoadWAV(file_name, &(clip->spec), &(clip->buf), &(clip->len)) == NULL ) {
        printf("Failed to load wave file: %s\n", SDL_GetError());
    }
}

void input_loop() {
     if( SDL_PollEvent( &event ) ){
        if( event.type == SDL_KEYUP ) {
            switch( event.key.keysym.sym ){
                case SDLK_1:
                    printf("one key release\n");
                    play_audio(&enemy_laser_snd);
                    break;
                case SDLK_2:
                    printf("two key release\n");
                    play_audio(&player_laser_snd);
                    break;
                case SDLK_3:
                    printf("three key release\n");
                    play_audio(&small_explosion_snd);
                    break;
                case SDLK_4:
                    printf("four key release\n");
                    play_audio(&large_explosion_snd);
                    break;
                case SDLK_5:
                    printf("five key release\n");
                    play_audio(&hit_snd);
                    break;
                default:
                    printf("unknown key release\n");
                    break;
            }
        }
    }
}

int main() {
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) { 
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        return 0;
    }

    SDL_CreateWindowAndRenderer( 320, 200, 0, &window, &renderer );

    init_audio( ENEMY_LASER, &enemy_laser_snd );
    init_audio( PLAYER_LASER, &player_laser_snd );
    init_audio( SMALL_EXPLOSION, &small_explosion_snd );
    init_audio( LARGE_EXPLOSION, &large_explosion_snd );
    init_audio( HIT, &hit_snd );

    device_id = SDL_OpenAudioDevice(NULL, 0, &(enemy_laser_snd.spec), NULL, 0);

    if (device_id == 0) {
        printf("Failed to open audio: %s\n", SDL_GetError());
    }    
    SDL_PauseAudioDevice(device_id, 0);

    emscripten_set_main_loop(input_loop, 0, 0);

    return 1;
}