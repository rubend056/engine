#ifndef _audio_h
#define _audio_h

#include "SDL2/SDL_audio.h"

/*
 * Queue structure for all loaded sounds
 *
 */
typedef struct sound
{
    uint32_t length;
    uint32_t lengthTrue;
    uint8_t * bufferTrue;
    uint8_t * buffer;
    uint8_t loop;
    uint8_t fade;
    uint8_t free;
    uint8_t volume;

    SDL_AudioSpec audio;

    struct sound * next;
} Audio;

#endif