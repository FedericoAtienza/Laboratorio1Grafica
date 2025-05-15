#include <iostream>
#include <stdio.h>

#ifdef __APPLE__ // macOS
#include <Freeimage/FreeImage.h>
#include <OpenGL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#else
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_ttf.h"
#include "FreeImage.h"
#include <GL/glu.h>
#endif

#include "camera.h"
#include "hud.h"
#include "skybox.h"
#include "settings.h"
#include "variables.h"
#include "worm.h"

using namespace std;

int window_width = 800;
int window_height = 600;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "No se pudo iniciar SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    SDL_Window* win = SDL_CreateWindow("Apple Worm",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       window_width,
                                       window_height,
                                       SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext context = SDL_GL_CreateContext(win);
    glEnable(GL_TEXTURE_2D);

    if (TTF_Init() == -1) {
        printf("Error al inicializar SDL_ttf: %s\n", TTF_GetError());
        exit(1);
    }
    glMatrixMode(GL_PROJECTION);

    float color = 0.0f;
    glClearColor(color, color, color, 1.0f);

    gluPerspective(45, (float) (window_width / window_height), 0.1, 2500);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    // Admits blending of objects, important for text blending for example
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    bool fin = false;
    bool fullscreen = false;
    bool opciones = false;

    bool wireframeActivado = false;
    bool sombreadoInterpolado = true;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);

    SDL_Event evento;

    Camera camera;

    // Carga de datos (imagenes) para poder dibujar la Skybox
    LoadSkybox();

    loadModels();

    Settings settings;
    settings.init_settings();

    /* INICIALIZACION HUD */
    int cantManzanas = level_map.apple_quantity();
    int cantManzanasComidas = 0; // Inicialmente 0
    int level_number = level_map.level_number();
    HUD my_hud = HUD(cantManzanas, level_number);

    Mix_PlayMusic(music, 0);

    do {
        // Cuanto tardo en procesar el frame
        frame_actual = SDL_GetTicks();
        // Calculo el tiempo que paso desde el frame anterior
        deltaTime = (frame_actual - frame_previo) / 1000.0f;
        // Tiempo del frame "previo" se transforma en el frame que se acabo de dibujar, asi el "actual" sera el siguiente
        frame_previo = frame_actual;

        my_hud.update_time(deltaTime);

        cantManzanasComidas = level_map.apple_quantity();
        my_hud.update_remaining_apples(cantManzanas - cantManzanasComidas);
        my_hud.update_speed(game_speed);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        camera.setViewMatrix();

        // Dibujo la skybox
        glPushMatrix();
        DrawSkybox(1000.0f);
        glPopMatrix();

        // Dibujo el worm
        worm.draw();

        // Dibujo el mapa
        level_map.draw();

        // Dibujo el HUD
        if (wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            my_hud.draw();
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            my_hud.draw();
        }
        // Dibujo las opciones
        if (opciones) {
            if (wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                settings.draw_settings();
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                settings.draw_settings();
            }
        }

        // Dibujo la pausa
        if (pause && !opciones) {
            settings.draw_pause();
        }

        while (SDL_PollEvent(&evento)) {
            switch (evento.type) {
            case SDL_MOUSEMOTION:
                camera.handleMouseMotion(evento.motion.xrel, evento.motion.yrel);
                break;
            case SDL_KEYDOWN:
                switch (evento.key.keysym.sym) {
                case SDLK_UP:
                    if (!opciones && !pause)
                        worm.move_up();
                    break;
                case SDLK_DOWN:
                    if (!opciones && !pause)
                        worm.move_down();
                    break;
                case SDLK_LEFT:
                    if (!opciones && !pause)
                        worm.move_left();
                    break;
                case SDLK_RIGHT:
                    if (!opciones && !pause)
                        worm.move_right();
                    break;
                case SDLK_ESCAPE:
                    fin = true;
                    break;
                }
                break;
            case SDL_QUIT:
                fin = true;
                break;
            case SDL_KEYUP:
                switch (evento.key.keysym.sym) {
                case SDLK_l:
                    if (light) {
                        light = false;
                    } else {
                        light = true;
                    }
                    break;
                case SDLK_x:
                    if (wireframe) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        wireframe = false;
                    } else {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        wireframe = true;
                    }
                    break;
                case SDLK_z:
                    if (shadeFlat) {
                        shadeFlat = false;
                        glShadeModel(GL_SMOOTH);
                    } else {
                        shadeFlat = true;
                        glShadeModel(GL_FLAT);
                    }
                    break;
                case SDLK_t:
                    if (textures) {
                        textures = false;
                    } else {
                        textures = true;
                    }
                    break;
                case SDLK_RETURN:
                    if (opciones) {
                        opciones = false;
                        Mix_ResumeMusic();
                        pause_end = SDL_GetTicks();
                        if (worm.animation_start_time < pause_init) {
                            deltaPause = pause_end - pause_init;
                        } else {
                            deltaPause = 0;
                        }
                        pause = false;
                    } else if (!pause) {
                        Mix_PauseMusic();
                        pause_init = SDL_GetTicks();
                        opciones = true;
                        pause = true;
                    }
                    break;
                case SDLK_v:
                    if (game_speed == 2.0f) {
                        game_speed = 4.0f;
                    } else if (game_speed == 4.0f) {
                        game_speed = 8.0f;
                    } else {
                        game_speed = 2.0f;
                    }
                    break;
                case SDLK_f:
                    if (!fullscreen) {
                        SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
                        fullscreen = true;
                    } else {
                        SDL_SetWindowFullscreen(win, 0);
                        fullscreen = false;
                    }
                    break;
                case SDLK_q:
                    fin = true;
                    break;
                case SDLK_p:
                    if (!opciones) {
                        if (pause) {
                            Mix_ResumeMusic();
                            pause_end = SDL_GetTicks();
                            if (worm.animation_start_time < pause_init) {
                                deltaPause = pause_end - pause_init;
                            } else {
                                deltaPause = 0;
                            }
                            pause = false;
                        } else {
                            Mix_PauseMusic();
                            pause_init = SDL_GetTicks();
                            pause = true;
                        }
                    }
                    break;
                case SDLK_c:
                    camera.changeCameraType();
                    break;
                }
            }
        }

        camera.handleMovementKeys();

        SDL_GL_SwapWindow(win);
    } while (!fin);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
