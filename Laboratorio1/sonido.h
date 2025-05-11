#pragma once
#include "SDL_mixer.h"

Mix_Music* cargarMusica(string ruta) {
	Mix_Music* res = Mix_LoadMUS(ruta.c_str());
	if (res == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	return res;
}

Mix_Chunk* cargarSonido(string ruta) {
	Mix_Chunk* res = Mix_LoadWAV(ruta.c_str());
	if (res == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}
	return res;
}