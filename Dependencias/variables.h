#ifdef __APPLE__ // macOS
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif

#ifndef VARIABLES_H
#define VARIABLES_H

// Conteo de tiempo para hacer movimiento independiente de FPS
Uint32 frame_previo = SDL_GetTicks();
Uint32 frame_actual;

const int WORM_MAX_LENGTH = 100; // Maximum length of the worm
const int MAX_MAP_HIGH = 100;

float game_speed = 2.0f; // Speed of the game

#endif