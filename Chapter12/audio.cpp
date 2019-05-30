#include "game.hpp"

Audio::Audio( char* file_name,  bool priority_value ) {
    strcpy( FileName, file_name );
    priority = priority_value;

    if( SDL_LoadWAV(FileName, &spec, &buf, &len) == NULL ) {
        printf("Failed to load wave file: %s\n", SDL_GetError());
    }
}

void Audio::Play() {
    if( priority || SDL_GetQueuedAudioSize(device_id) > 1 ) {
        SDL_ClearQueuedAudio(device_id);
    }

    int success = SDL_QueueAudio(device_id, buf, len);
    if( success < 0 ) {
        printf("SDL_QueueAudio %s failed: %s\n", FileName, SDL_GetError());
    }
}