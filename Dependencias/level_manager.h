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

struct Particula {
    float x, y, z;
    float dx, dy, dz;
    float c1, c2, c3;
};

class LevelManager {
  private:
    bool animating;
    float animation_time;

    bool animating_death;
    Point animation_point;
    Point death_animation_point;
    std::vector<Particula> death_particulas; // Para la animacion 1, sus cubitos

    std::vector<Particula> particulas; // Para la animacion 1, sus cubitos

    void drawCube(float x, float y, float z, float scale);
    void drawStar(float x, float y, float z, float cubeScale);

  public:
    LevelManager();
    bool update_animation(float deltaTime);
    void draw_animation_1();
    void draw_animation_2();
    void start_animation();
    bool is_animating();
    void set_animation_point(Point p);

    void next_level();

    void set_dead_animation_point(Point p);
    bool is_animating_death();
    void start_death_animation();
    void draw_animation_death();
    bool update_death_animation(float deltaTime);

    void reset(); // reset level despues de morir

    void chequeo_muerte(HUD& my_hud);          // chequeo si el gusano murio por pinchos o vacio
    void chequeo_cambio_de_nivel(HUD& my_hud); // chequeo si el gusano salio del mapa
};

LevelManager::LevelManager() {
    animating = false;
    animating_death = false;
    animation_time = 0.0f;
}

bool LevelManager::is_animating() {
    return this->animating;
}

void LevelManager::start_animation() {
    animating = true;
    animation_time = 0.0f;

    int cant_particulas = 30;
    for (int i = 0; i < cant_particulas; ++i) {
        Particula p;
        p.x = animation_point.x;
        p.y = animation_point.y;
        p.z = 0.0f;

        // Direcc aleatoria entre -1 y 1
        p.dx = ((float) rand() / RAND_MAX) * 2.0f - 1.0f;
        p.dy = ((float) rand() / RAND_MAX) * 2.0f;
        p.dz = ((float) rand() / RAND_MAX) * 2.0f - 1.0f;
        particulas.push_back(p);
    }

    for (auto& p : particulas) {
        p.c1 = ((float) rand() / RAND_MAX);
        p.c2 = ((float) rand() / RAND_MAX);
        p.c3 = ((float) rand() / RAND_MAX);
    }
}

bool LevelManager::update_animation(float deltaTime) {
    if (animating) {
        animation_time += deltaTime;

        for (auto& p : particulas) {
            p.x += p.dx * deltaTime * 2.0f; // velocidad
            p.y += p.dy * deltaTime * 2.0f;
            p.z += p.dz * deltaTime * 2.0f;
        }

        // Cuando pasa el tiempo de animacion, la desactivo y borro las particulas,
        // y cargo el proximo nivel
        if (animation_time > 1.5f) {
            animating = false;
            particulas.clear();
            next_level(); // Carga proximo nivel
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

// Animacion de las particulas
void LevelManager::draw_animation_1() {
    if (animating) {
        for (auto& p : particulas) {
            if (!pause) { // Si estoy animando y no en pausa altero sus colores
                p.c1 = ((float) rand() / RAND_MAX);
                p.c2 = ((float) rand() / RAND_MAX);
                p.c3 = ((float) rand() / RAND_MAX);
            }
            glColor3f(p.c1, p.c2, p.c3);
            drawCube(p.x, p.y, p.z, .2f);
        }
    }
}

// Animacion de la estrella
void LevelManager::draw_animation_2() {
    if (animating) {
        float scale = 0.5f + 0.5f * sin(animation_time * 3); // efecto "pulso" al dibujar
        float angle = animation_time * 90.0f;

        glPushMatrix();
        glTranslatef(animation_point.x, animation_point.y + 3, 0.0f);
        glRotatef(angle, 0.0f, 1.0f, 0.0f);
        glColor3f(1.0f, 0.8f, 0.0f); // dorado
        drawStar(0.0f, 0.0f, 0.0f, scale);
        glPopMatrix();
    }
}

void LevelManager::next_level() {
    level += 1; // Aumento el level global
    std::string load_level = "../Dependencias/levels/level" + std::to_string(level) + ".xml";
    std::cout << "Se cambiara a nivel: " << load_level << std::endl;
    level_map = Map(level);
}

void LevelManager::set_animation_point(Point p) {
    animation_point = p;
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
    scale = scale / 2.0f;

    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(scale, scale, scale);

    // Cara frontal
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glEnd();

    // Cara trasera
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glEnd();

    // Cara superior
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glEnd();

    // Cara inferior
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glEnd();

    // Cara derecha
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glEnd();

    // Cara izquierda
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glEnd();

    glPopMatrix();
}

void LevelManager::set_dead_animation_point(Point p) {
    death_animation_point = p;
}

void LevelManager::start_death_animation() {
    std::cout << "start death anim" << std::endl;
    animating_death = true;
    animation_time = 0.0f;

    int cant_particulas = 50;
    for (int i = 0; i < cant_particulas; ++i) {
        Particula p;
        p.x = death_animation_point.x;
        p.y = death_animation_point.y;
        p.z = 0.0f;

        // Direcc aleatoria entre -1 y 1
        p.dx = ((float) rand() / RAND_MAX) * 2.0f - 1.0f;
        p.dy = 0;
        p.dz = ((float) rand() / RAND_MAX) * 2.0f - 1.0f;
        death_particulas.push_back(p);
    }
}

void LevelManager::draw_animation_death() {
    if (animating_death) {
        for (auto& p : death_particulas) {
            ;
            glColor4f(0.4f, 0.1f, 0.1f, 0.8f);
            drawCube(p.x, p.y, p.z, .2f);
        }
    }
}

bool LevelManager::is_animating_death() {
    return this->animating_death;
}

bool LevelManager::update_death_animation(float deltaTime) {
    if (animating_death) {
        animation_time += deltaTime;

        for (auto& p : death_particulas) {
            p.x += p.dx * deltaTime * .75f; // velocidad
            p.y += p.dy * deltaTime * .75f;
            p.z += p.dz * deltaTime * .75f;
        }

        // Cuando pasa el tiempo de animacion, la desactivo y borro las particulas
        if (animation_time > 2.0f) {
            animating_death = false;
            death_particulas.clear();
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

// Cuando muere el gusano, es necesario resetear el nivel
void LevelManager::reset() {
    level_map = Map(level);
}

void LevelManager::chequeo_muerte(HUD& my_hud) {
    Point punto_muerte;
    if (worm.is_dead_spike(punto_muerte)) { // tiene que retornar ubicacion
        // Invoco animacion muerte
        set_dead_animation_point(punto_muerte);
        worm.set_animating_death(true);
        start_death_animation();
        worm.moriste();
    }

    if (is_animating_death()) {
        draw_animation_death();
        my_hud.show_you_died();
    }

    if (update_death_animation(deltaTime)) {
        // Una vez muere por pinchos:
        // 1. reseteo el nivel (reconstruyendo manzanas)
        // 2. reseteo manzanas en el hud
        // 3. gusano vuelve a spawn y con el largo inicial
        worm.set_animating_death(false);
        reset();
        my_hud.reset_except_timer();
        Point spawn = level_map.get_spawn();
        worm.reset({spawn.x, spawn.y}); // Aca le paso la nueva posicion inicial
        my_hud.hide_you_died();
    }

    // Es true una vez que cayo al vacio y termino su animacion de fantasma
    if (worm.is_dead_vacio()) {
        worm.start_vacio_death_animation();
        worm.set_animating_fall(true);
        my_hud.show_you_died();
        worm.moriste();
    }

    if (worm.animation_vacio_ended()) {
        reset();
        my_hud.reset_except_timer();
        Point spawn = level_map.get_spawn();
        worm.reset({spawn.x, spawn.y}); // Aca le paso la nueva posicion inicial
        my_hud.hide_you_died();
    }
}

void LevelManager::chequeo_cambio_de_nivel(HUD& my_hud) {
    // Controlo si debo involar al level manager
    if (worm.to_exit()) {
        set_animation_point(level_map.get_exit());
        start_animation();
        worm.saliste();
    }

    // Animacion next level
    if (update_animation(deltaTime)) {
        // Donde spawneara el gusano el prox nivel
        Point spawn = level_map.get_spawn();
        worm.reset({spawn.x, spawn.y}); // Aca le paso la nueva posicion inicial
        my_hud.update_level_number();   // Actualiza datos del hud correspondientes a sig nivel
        my_hud.hide_next_level();
    }

    // Comienzan animaciones de next level
    if (is_animating()) {
        draw_animation_1();
        draw_animation_2();
        my_hud.show_next_level();
    }
}