#include <iostream>
#include <stdio.h>
#include <iomanip>

#ifdef __APPLE__ // macOS
#include <Freeimage/FreeImage.h>
#include <OpenGL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <tinyxml2/tinyxml2.h>

#else
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_ttf.h"
#include "FreeImage.h"
#include "tinyxml2.h"
#include <GL/glu.h>
#endif

#include "skybox.h"
#include "camera.h"
#include "settings.h"
#include "variables.h"
#include "level_manager.h"
#include "worm.h"
#include "hud.h"
#include "light.h"

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

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);

    // Considers color as material, important for showing light with colors
    glEnable(GL_COLOR_MATERIAL);

    SDL_Event evento;

    Camera camera;

    // Carga de datos (imagenes) para poder dibujar la Skybox
    LoadSkybox();

    loadModels();

    // Carga de textura portal para la salida
    portalTexture = LoadTexture("../Dependencias/nether_portal.png");

    // Carga de textura bloques de tierra
    dirtBlockTop = LoadTexture("../Dependencias/GrassTop.jpg");
    dirtBlockSide = LoadTexture("../Dependencias/GrassSide.png");
    dirtBlockBottom = LoadTexture("../Dependencias/GrassBottom.jpg");

    // Carga de textura bloques de TNT
    tntTopBottom = LoadTexture("../Dependencias/TNTTopBottom.png");
    tntSide = LoadTexture("../Dependencias/TNTSide.png");

    Settings settings;

    HUD my_hud;

    LevelManager level_manager;

    // Mix_PlayMusic(music, 0);

    Light lightSource;
    lightSource.set_position({0.0f, 3.0f});
    lightSource.set_color(1.0f, 1.0f, 1.0f);

    //
    glEnable(GL_LIGHTING);

    do {
        // Cuanto tardo en procesar el frame
        frame_actual = SDL_GetTicks();
        // Calculo el tiempo que paso desde el frame anterior
        deltaTime = (frame_actual - frame_previo) / 1000.0f;
        // Tiempo del frame "previo" se transforma en el frame que se acabo de dibujar, asi el "actual" sera el siguiente
        frame_previo = frame_actual;

        if (pause) {
            // Si el juego esta en pausa, no se actualiza el tiempo
            deltaTime = 0.0f;
        }

        my_hud.update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        camera.setViewMatrix();

        // Dibujo la luz
        if (light) {
            lightSource.draw();
        }

        // Dibujo la skybox
        DrawSkybox(1000.0f);

        // Controlo explosivos
        worm.check_explosives();

        // Dibujo el mapa
        level_map.draw();

        /*                         */
        /* seccion CAMBIO DE NIVEL */

        // Controlo si debo involar al level manager
        if (worm.to_exit()) {
            level_manager.set_animation_point(level_map.get_exit());
            level_manager.start_animation();
            worm.saliste();
            // level_manager.next_level(win);
        }

        // Animacion next level
        if (level_manager.update_animation(deltaTime)) {
            // Donde spawneara el gusano el prox nivel
            Point spawn = level_map.get_spawn();
            worm.reset({spawn.x, spawn.y}); // Aca le paso la nueva posicion inicial
            my_hud.update_level_number();   // Actualiza datos del hud correspondientes a sig nivel
            my_hud.hide_next_level();
        }

        // Comienzan animaciones de next level
        if (level_manager.is_animating()) {
            level_manager.draw_animation_1();
            level_manager.draw_animation_2();
            my_hud.show_next_level();
        }

        /* fin secc cambio nivel */
        /*                       */

        // Dibujo el worm
        worm.draw();

        // Dibujo el HUD
        if (wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            my_hud.draw();
            settings.draw_settings();
            settings.draw_pause();
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            my_hud.draw();
            settings.draw_settings();
            settings.draw_pause();
        }

        while (SDL_PollEvent(&evento)) {
            if (!pause) {
                switch (evento.type) {
                case SDL_MOUSEMOTION:
                    camera.handleMouseMotion(evento.motion.xrel, evento.motion.yrel);
                    break;
                case SDL_KEYUP:
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
                    }
                }
            }
            switch (evento.type) {
            case SDL_QUIT:
                fin = true;
                break;
            case SDL_KEYUP:
                switch (evento.key.keysym.sym) {
                case SDLK_ESCAPE:
                    fin = true;
                    break;
                case SDLK_q:
                    fin = true;
                    break;
                case SDLK_l:
                    settings.switch_light();
                    break;
                case SDLK_x:
                    settings.switch_wireframe();
                    break;
                case SDLK_z:
                    settings.switch_shadeModel();
                    break;
                case SDLK_t:
                    settings.switch_textures();
                    break;
                case SDLK_RETURN:
                    settings.switch_opciones();
                    break;
                case SDLK_v:
                    settings.switch_speed();
                    break;
                case SDLK_f:
                    settings.switch_fullscreen(win);
                    break;
                case SDLK_p:
                    settings.switch_pause();
                    break;
                case SDLK_c:
                    camera.changeCameraType();
                    break;
                }
                break;
            }
            break;
        }

        camera.handleMovementKeys();

        SDL_GL_SwapWindow(win);
    } while (!fin);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
