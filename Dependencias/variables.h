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
float deltaTime = 0.0f;

const int WORM_MAX_LENGTH = 100; // Maximum length of the worm
const int MAX_MAP_HIGH = 100;

// Settings
float game_speed = 1.0f; // Speed of the game

bool light = false;

bool textures = true; // Draw textures or not

bool wireframe = false; // Activates wireframe or deactivates it

bool shadeFlat = false; // Defines shade model to be flat or smooth

bool pause = false; // Pause or resume game

bool fullscreen = false;

bool opciones = false;

// Audio
int audio = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); // Initialize audio

Mix_Chunk* sfx_movement = cargarSonido("../Dependencias/Sonidos/SE_Pu.wav"); // Sfx for moving the worm

Mix_Chunk* sfx_grow = cargarSonido("../Dependencias/Sonidos/SE_KILLER_POWERUP2.wav"); // Sfx for growing the worm

Mix_Chunk* sfx_blocked = cargarSonido("../Dependencias/Sonidos/PokemonWall.wav");

Mix_Chunk* sfx_menu_option_enable = cargarSonido("../Dependencias/Sonidos/ZeldaMenuHigh.wav");

Mix_Chunk* sfx_menu_option_disable = cargarSonido("../Dependencias/Sonidos/ZeldaMenuLow.wav");

Mix_Music* music = cargarMusica("../Dependencias/Sonidos/EdgeKakkoi.wav"); // Background music

// Variable for portal texture
GLuint portalTexture;

// Variables for dirt blocks
GLuint dirtBlockTop;
GLuint dirtBlockSide;
GLuint dirtBlockBottom;

// Variables for TNT blocks
GLuint tntTopBottom;
GLuint tntSide;

#endif