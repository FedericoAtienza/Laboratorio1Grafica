#pragma once
#ifdef __APPLE__ // macOS
#include <SDL2/SDL_mixer.h>
#else
#include "SDL_mixer.h"
#endif

Mix_Music* cargarMusica(std::string ruta) {
    Mix_Music* res = Mix_LoadMUS(ruta.c_str());
    if (res == NULL) {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }
    return res;
}

Mix_Chunk* cargarSonido(std::string ruta) {
    Mix_Chunk* res = Mix_LoadWAV(ruta.c_str());
    if (res == NULL) {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }
    return res;
}