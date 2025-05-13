#include <iostream>
#include <stdio.h>

#ifdef __APPLE__ // macOS
#include <Freeimage/FreeImage.h>
#include <OpenGL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#else
#include "FreeImage.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <GL/glu.h>
#endif

#include "camera.h"
#include "skybox.h"
#include "variables.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "No se pudo iniciar SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    SDL_Window* win = SDL_CreateWindow("Apple Worm",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       800,
                                       600,
                                       SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext context = SDL_GL_CreateContext(win);

    glMatrixMode(GL_PROJECTION);

    float color = 0.0f;
    glClearColor(color, color, color, 1.0f);

    gluPerspective(45, 800 / 600.f, 0.1, 2500);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    bool fin = false;
    bool fullscreen = false;

    bool wireframeActivado = false;
    bool sombreadoInterpolado = true;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);

    SDL_Event evento;

    Camera camera;

    // Carga de datos (imagenes) para poder dibujar la Skybox
    LoadSkybox();

    loadModels();

    Mix_PlayMusic(music, 0);

    do {
        // Cuanto tardo en procesar el frame
        frame_actual = SDL_GetTicks();
        // Calculo el tiempo que paso desde el frame anterior
        deltaTime = (frame_actual - frame_previo) / 1000.0f;
        // Tiempo del frame "previo" se transforma en el frame que se acabo de dibujar, asi el "actual" sera el siguiente
        frame_previo = frame_actual;

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

        while (SDL_PollEvent(&evento)) {
            switch (evento.type) {
            case SDL_MOUSEMOTION:
                camera.handleMouseMotion(evento.motion.xrel, evento.motion.yrel);
                break;
            case SDL_KEYDOWN:
                switch (evento.key.keysym.sym) {
                case SDLK_UP:
                    worm.move_up();
                    break;
                case SDLK_DOWN:
                    worm.move_down();
                    break;
                case SDLK_LEFT:
                    worm.move_left();
                    break;
                case SDLK_RIGHT:
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
                case SDLK_x:
                    if (wireframe) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        wireframe = false;
                    } else {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        wireframe = true;
                    }
                    break;
                case SDLK_l:
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
