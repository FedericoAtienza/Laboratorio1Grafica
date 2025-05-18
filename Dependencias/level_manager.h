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

struct Particle {
    float x, y, z;
    float dx, dy, dz;
};

class LevelManager{
    private:
        int lvl_number;

        std::vector<Particle> particles; // Para la animacion 1, sus cubitos

        bool animating;
        float animation_time;

        void drawCube(float x, float y, float z, float scale);
        void drawStar(float x, float y, float z, float cubeScale);



    public:
        void update_animation(float deltaTime);
        void draw_animation_1();
        void draw_animation_2();
        void start_animation();
        bool is_animating();
        
        void next_level(SDL_Window* win);

};

bool LevelManager::is_animating() {
    return this->animating;
}

void LevelManager::start_animation() {
    if (!animating) {
        animating = true;
        animation_time = 0.0f;

        particles.clear();
        int num_particles = 20;
        for (int i = 0; i < num_particles; ++i) {
            Particle p;
            p.x = 1.0f;
            p.y = 2.0f;
            p.z = 0.0f;
            // Dirección aleatoria (entre -1 y 1)
            // Direcc aleatoria entre -1 y 1
            p.dx = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
            p.dy = ((float)rand() / RAND_MAX) * 2.0f;
            p.dz = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
            particles.push_back(p);
        }
    }
}

void LevelManager::update_animation(float deltaTime) {
    if (animating) {
        animation_time += deltaTime;
        std::cout << "ANIMATION TIME: " << animation_time << std::endl;

        for (auto& p : particles) {
            p.x += p.dx * deltaTime * 2.0f; // velocidad
            p.y += p.dy * deltaTime * 2.0f;
            p.z += p.dz * deltaTime * 2.0f;
        }

        // Cuando pasa el tiempo de animacion, la desactivo y borro las particulas
        if (animation_time > 2.0f) {
            animating = false;
            particles.clear(); 
        }
    }
}


void LevelManager::draw_animation_1() {
    if (animating) {
        for (auto& p : particles) {
            float r = ((float)rand() / RAND_MAX);
            float g = ((float)rand() / RAND_MAX);
            float b = ((float)rand() / RAND_MAX);
            glColor3f(r,g,b);
            drawCube(p.x, p.y, p.z, .2f);
        }
    }
}

void LevelManager::draw_animation_2() {
    if (animating) {
        float scale = 0.5f + 0.5f * sin(animation_time * 3); // efecto "pulso" al dibujar
        float angle = animation_time * 90.0f;

        glPushMatrix();
        glTranslatef(1.0f, 4.0f, 0.0f);
        glRotatef(angle, 0.0f, 1.0f, 0.0f);
        glColor3f(1.0f, 0.8f, 0.0f); // dorado
        drawStar(0.0f, 0.0f, 0.0f, scale);
        glPopMatrix();
    }
}

void LevelManager::next_level(SDL_Window* window){
    //next_level_animation(window);
    /*
    level += 1; // Aumento el level global 
    std::string load_level = "../Dependencias/level" + std::to_string(level) + ".txt";
    std::cout << "Se cambiara a nivel: " << load_level << std::endl;
    level_map = Map(level);
    */

}

void LevelManager::drawStar(float x, float y, float z, float cubeScale) {
    // Centro de la estrella
    drawCube(x, y, z, cubeScale);

    float d = 1; // Separacion entre los cubos q forman la estrella

    drawCube(x + d, y, z, cubeScale); // der
    drawCube(x - d, y, z, cubeScale); // izq
    drawCube(x, y + d, z, cubeScale); // arriba
    drawCube(x, y - d, z, cubeScale); // abajo
    drawCube(x, y, z + d, cubeScale); // en frente
    drawCube(x, y, z - d, cubeScale); // atras
}


void LevelManager::drawCube(float x, float y, float z, float scale = 1.0f) {
    scale = scale / 2.0f;  // Para que el cubo tenga tamaño total = scale

    glPushMatrix();
    glTranslatef(x, y, z);      // Mover el cubo a la posición deseada
    glScalef(scale, scale, scale); // Escalar el cubo

    //Cara frontal
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glEnd();

    //Cara trasera
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glEnd();

    //Cara superior
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glEnd();

    //Cara inferior
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glEnd();

    //Cara derecha
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glEnd();

    //Cara izquierda
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glEnd();

    glPopMatrix();

}