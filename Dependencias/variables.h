#ifdef __APPLE__ // macOS
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif

#include "sonido.h"

#ifndef VARIABLES_H
#define VARIABLES_H

// Conteo de tiempo para hacer movimiento independiente de FPS
Uint32 frame_previo = SDL_GetTicks();
Uint32 frame_actual;

const int WORM_MAX_LENGTH = 100; // Maximum length of the worm
const int MAX_MAP_HIGH = 100;

float game_speed = 2.0f; // Speed of the game

bool textures = true; // Defines whether to draw textures or not

bool wireframe = false; // Defines whether to activate wireframe or not

bool shadeFlat = false; // Defines shade model to be flat or smooth

int audio = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); //Initialize audio

Mix_Chunk* sfx_movement = cargarSonido(("../Dependencias/Sonidos/SE_Pu.wav")); //Sfx for moving the worm

Mix_Chunk* sfx_grow = cargarSonido(("../Dependencias/Sonidos/SE_KILLER_POWERUP2.wav")); //Sfx for growing the worm

Mix_Music* music = cargarMusica("../Dependencias/Sonidos/EdgeKakkoi.wav"); //Background music


#endif