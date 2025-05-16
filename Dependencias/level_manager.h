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
#include "map_variable.h"

class LevelManager{
    private:
        int lvl_number;

        void next_level_animation(SDL_Window* win);
        void drawStar(float x, float y, float z, float cubeScale);
        void drawCube(float x, float y, float z, float scale);

    public:
        void next_level(SDL_Window* win);

};

void LevelManager::next_level(SDL_Window* window){
    next_level_animation(window);
    /*
    level += 1; // Aumento el level global 
    std::string load_level = "../Dependencias/level" + std::to_string(level) + ".txt";
    std::cout << "Se cambiara a nivel: " << load_level << std::endl;
    level_map = Map(level);
    */

}

void LevelManager::next_level_animation(SDL_Window* window){
    for (int i = 0; i < 60; i++) { // 60 cuadros (~1 segundo)

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ← LIMPIEZA IMPORTANTE
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f);

        drawStar(4.0, 4.0, 0.0, 1.0);

        glPopMatrix();

        SDL_GL_SwapWindow(window);

        SDL_Delay(16); // 60 FPS aprox.
    }
}


void LevelManager::drawStar(float x, float y, float z, float cubeScale) {
    drawCube(x, y, z, cubeScale); // centro

    float d = cubeScale / 2.0f;

    drawCube(x + d, y, z, cubeScale);
    drawCube(x - d, y, z, cubeScale);
    drawCube(x, y + d, z, cubeScale);
    drawCube(x, y - d, z, cubeScale);
    drawCube(x, y, z + d, cubeScale);
    drawCube(x, y, z - d, cubeScale);
}

void LevelManager::drawCube(float x, float y, float z, float scale = 1.0f) {
    scale = scale / 2.0f;  // Para que el cubo tenga tamaño total = scale

    glPushMatrix();
    glTranslatef(x, y, z);      // Mover el cubo a la posición deseada
    glScalef(scale, scale, scale); // Escalar el cubo

    // Cara frontal (+Z)
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glEnd();

    // Cara trasera (-Z)
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glEnd();

    // Cara superior (+Y)
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glEnd();

    // Cara inferior (-Y)
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glEnd();

    // Cara derecha (+X)
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glEnd();

    // Cara izquierda (-X)
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glEnd();

    glPopMatrix();
}



/*
void HUD::draw() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 600, 0); // PONER window_width, window_height

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    int x_frame = 10;
    int x = 20;
    int y = 10;
    int separacion = 10;

    // 0. Dibujo carta/marco
    glBindTexture(GL_TEXTURE_2D, carta_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x_frame, y);
    glTexCoord2f(1, 0);
    glVertex2f(x_frame + 120, y);
    glTexCoord2f(1, 1);
    glVertex2f(x_frame + 120, y + 180);
    glTexCoord2f(0, 1);
    glVertex2f(x_frame, y + 180);
    glEnd();

    // 1. Dibujo Level
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, level_number_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + level_w, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + level_w, y + level_h);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + level_h);
    glEnd();

    // 2. Dibujo time
    y += level_h + separacion;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, time_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + time_w, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + time_w, y + time_h);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + time_h);
    glEnd();

    // 3. Dibujo speed
    y += time_h + separacion;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, speed_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + speed_w, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + speed_w, y + speed_h);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + speed_h);
    glEnd();

    // 4. Dibujo apple y apple_text
    y += speed_h + separacion;
    int desired_width_apple = 50; // tamaño en pantalla de la manzana
    int desired_height_apple = 50;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, apple_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + desired_width_apple, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + desired_width_apple, y + desired_height_apple);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + desired_height_apple);
    glEnd();

    x += desired_width_apple;
    y += (desired_height_apple - apple_h) / 2;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, apple_text_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + apple_w, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + apple_w, y + apple_h);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + apple_h);
    glEnd();

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
*/