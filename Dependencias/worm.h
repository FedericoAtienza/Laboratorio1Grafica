#ifdef __APPLE__ // macOS
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#else
#include "SDL.h"
#include "SDL_opengl.h"
#endif

#include "models.h"
#include "cube.h"
#include "map_variable.h"
#include "point.h"
#include "variables.h"

#ifndef WORM_H
#define WORM_H

class Worm {
  public:
    void check_explosives() {
        level_map.check_explosives();
    }

  private:
    Point* head;
    Point body[WORM_MAX_LENGTH];
    int body_length;
    bool animation;
    bool animating_death; // indica si estan animando muerte x pinchos en level manager
    float animation_progress;
    float animation_duration; // in seconds (with game speed 1)
    Point animation_start_body[WORM_MAX_LENGTH];
    Point animation_end_body[WORM_MAX_LENGTH];
    float body_rotation[WORM_MAX_LENGTH];
    float animation_start_body_rotation[WORM_MAX_LENGTH];
    float animation_end_body_rotation[WORM_MAX_LENGTH];
    bool exit;       // indica que esta en salida
    bool dead_spike; // indica que esta MUERTO
    bool dead_vacio;
    bool animation_vacio_end;
    float r_head, g_head, b_head;
    float r_body, g_body, b_body;
    float alpha;

    bool is_worm_body_in_point(Point p) {
        for (int i = 0; i < body_length; i++) {
            if (body[i].x == p.x && body[i].y == p.y) {
                return true;
            }
        }
        return false;
    }

    int get_distance_to_ground() {
        int distance = MAX_MAP_HIGH;
        int distance_aux;
        for (int i = 0; i < body_length; i++) {
            distance_aux = level_map.get_distance_to_ground(body[i]);
            if (distance > distance_aux) {
                distance = distance_aux;
            }
        }
        distance--; // distance (0,0) to (0,1) is 0
        return distance;
    }

    float animation_calculate_rotation(Point start, Point end) {
        if (start.x == end.x - 1)
            return 0.0f; // right
        if (start.x == end.x + 1)
            return 180.0f; // left
        if (start.y == end.y - 1)
            return 90.0f; // up
        if (start.y == end.y + 1)
            return 270.0f; // down

        return 0.0f; // default
    }

    void animation_start_moving(Point next_position) {
        animation = true;
        animation_duration = 0.5f;
        animation_progress = 0.0f;
        animation_start_body[0] = body[0];
        animation_end_body[0] = next_position;
        animation_start_body_rotation[0] = body_rotation[0];
        animation_end_body_rotation[0] = animation_calculate_rotation(body[0], next_position);

        for (int i = 1; i < body_length; i++) {
            animation_start_body[i] = body[i];
            animation_end_body[i] = body[i - 1];
            animation_start_body_rotation[i] = body_rotation[i];
            animation_end_body_rotation[i] = body_rotation[i - 1];
        }
    }

    void animation_start_falling(int distance) {
        animation = true;
        animation_duration = 0.5f * distance;
        animation_progress = 0.0f;

        for (int i = 0; i < body_length; i++) {
            animation_start_body[i] = body[i];
            animation_end_body[i].x = body[i].x;
            animation_end_body[i].y = body[i].y - distance;
            animation_start_body_rotation[i] = body_rotation[i];
            animation_end_body_rotation[i] = body_rotation[i];
        }
    }

    // Calculates the progress of the animation, returning a value between 0 (start) and 1 (end)
    void animation_calculate_progress() {
        float progress = (float) deltaTime / animation_duration * game_speed;
        animation_progress += progress;
    }

    void animation_move(float animation_progress) {
        bool cayo_al_vacio = false;
        for (int i = body_length - 1; i >= 0; i--) {
            body[i].x = (animation_start_body[i].x + animation_progress * (animation_end_body[i].x - animation_start_body[i].x));
            body[i].y = (animation_start_body[i].y + animation_progress * (animation_end_body[i].y - animation_start_body[i].y));
            float difference = animation_end_body_rotation[i] - animation_start_body_rotation[i];
            // Normalize the difference between 270 and 0 to be -90 or 90
            if (difference == -270.0f)
                difference = 90.0f;
            if (difference == 270.0f)
                difference = -90.0f;
            body_rotation[i] = (animation_start_body_rotation[i] + animation_progress * difference);
            // Normalize the rotation to be between 0 and 360
            if (body_rotation[i] >= 360.0f)
                body_rotation[i] -= 360.0f;
            if (body_rotation[i] < 0.0f)
                body_rotation[i] += 360.0f;
        }
        if (body[0].y < -5) {
            cayo_al_vacio = true;
        } else if (body[0].y > 8) {
            animation_vacio_end = true;
        }
        if (cayo_al_vacio) {
            animation_vacio_end = false;
            this->dead_vacio = true;
        }
    }

    void handle_is_not_supported() {
        int distance = get_distance_to_ground();
        if (distance > 0) {
            animation_start_falling(distance);
        }

        // Chequeo contacto con pinchos
        for (int i = 0; i < body_length; i++) {
            if (level_map.is_spike_in_point(body[i])) {
                this->dead_spike = true;
            }
        }
    }

    void animation_handler() {
        animation_calculate_progress();
        if (animation_progress >= 1.0f) {
            animation_progress = 1.0f;
        }
        animation_move(animation_progress);
        if (animation_progress == 1.0f) {
            animation = false;
            handle_is_not_supported();
        }
    }

    void grow_while_moving() {
        body_length++;
        body[body_length - 1] = body[body_length - 2];
    }

    bool is_allowed_to_move(Point move_to) {
        // Check if is doing the animation
        if (animation)
            return false;

        // Chequeo colisión con el cuerpo
        if (is_worm_body_in_point(move_to)) {
            Mix_PlayChannel(-1, sfx_blocked, 0);
            return false;
        }

        // Chequeo colisión con bloques
        if (level_map.is_block_in_point(move_to)) {
            Mix_PlayChannel(-1, sfx_blocked, 0);
            return false;
        }

        // Check if the worm is growing
        if (level_map.is_apple_in_point(move_to)) {
            level_map.remove_apple(move_to);
            Mix_PlayChannel(-1, sfx_grow, 0);
            grow_while_moving();
        }

        // Chequeo si se mueve a la salida
        if (level_map.is_exit_in_point(move_to)) {
            this->exit = true;
        }

        return true;
    }

    void move_to(Point next_position) {
        if (!is_allowed_to_move(next_position))
            return;
        Mix_PlayChannel(-1, sfx_movement, 0);
        animation_start_moving(next_position);
    }

    void draw_head() {
        glPushMatrix();
        glTranslatef(head->x, head->y, 0);
        glRotatef(body_rotation[0], 0.0f, 0.0f, 1.0f);

        glPushMatrix();
        glTranslatef(0.3f, 0.3f, 0.2f);
        glScalef(0.3f, 0.3f, 0.3f);
        wormEye.Draw(true, false);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.3f, 0.3f, -0.2f);
        glScalef(0.3f, 0.3f, 0.3f);
        wormEye.Draw(true, false);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.3f, 0.3f, 0.2f);
        glScalef(0.3f, 0.3f, 0.3f);
        wormEyelid.Draw(true, false);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.3f, 0.3f, -0.2f);
        glScalef(0.3f, 0.3f, 0.3f);
        wormEyelid.Draw(true, false);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.6f, -0.1f, 0.05f);
        glScalef(0.9f, 0.9f, 0.9f);
        wormLips.Draw(true, false);
        glPopMatrix();

        glPushMatrix();
        glScalef(0.6f, 0.6f, 0.6f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        wormHead.Draw(true, false);
        glPopMatrix();
        glPopMatrix();
    }

    void draw_body() {
        for (int i = 1; i < body_length; i++) {
            glPushMatrix();
            glTranslatef(body[i].x, body[i].y, 0);
            glRotatef(body_rotation[i], 0.0f, 0.0f, 1.0f);
            glScalef(0.50f, 0.50f, 0.50f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            wormHead.Draw(true, false);
            glPopMatrix();
            glPushMatrix();
            glTranslatef(body[i].x - (body[i].x - body[i - 1].x) / 2, body[i].y - (body[i].y - body[i - 1].y) / 2, 0);
            glRotatef(body_rotation[i], 0.0f, 0.0f, 1.0f);
            glScalef(0.50f, 0.50f, 0.50f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            wormBody.Draw(true, false);
            glPopMatrix();
        }
    }

    void draw_worm() {
        draw_head();
        draw_body();
    }

  public:
    Worm(Point head) {
        this->body_length = 2;
        this->body[0] = head;
        this->head = &this->body[0];
        this->body[1] = {head.x - 1, head.y};
        this->body_rotation[0] = 0.0f;
        this->body_rotation[1] = 0.0f;
        this->exit = false;
        this->r_head = 1.0f;
        this->g_head = 0.0f;
        this->b_head = 1.0f;
        this->r_body = 0.0f;
        this->g_body = 1.0f;
        this->b_body = 0.0f;
        this->alpha = 1.0; // inicialmente opaco
    }

    void reset(Point new_head) {
        this->body_length = 2;
        this->body[0] = new_head;
        this->head = &this->body[0];
        this->body[1] = {new_head.x - 1, new_head.y};
        this->body_rotation[0] = 0.0f;
        this->body_rotation[1] = 0.0f;
        this->animation = false;
        this->animation_progress = 0.0f;
        this->exit = false;
        this->r_head = 1.0f;
        this->g_head = 0.0f;
        this->b_head = 1.0f;
        this->r_body = 0.0f;
        this->g_body = 1.0f;
        this->b_body = 0.0f;
        this->alpha = 1.0; // inicialmente opaco
        this->animation_vacio_end = false;
        this->animating_death = false;
    }

    bool to_exit() {
        if (this->exit) {
            return true;
        } else {
            return false;
        }
    }

    void saliste() {
        this->exit = false;
    }

    bool is_dead_spike(Point& punto_muerte) {
        if (this->dead_spike) {
            punto_muerte = this->get_head();
            return true;
        } else {
            return false;
        }
    }

    void start_vacio_death_animation() {
        // Aca puedo disparar animacion "fantasma"
        // Seteo colores fantasmita
        this->dead_vacio = true;
        this->r_head = 0.6;
        this->g_head = 0.9;
        this->b_body = 1.0;
        this->r_body = 0.6;
        this->g_body = 0.9;
        this->b_body = 1.0;
        this->alpha = 0.3;
        for (int i = 0; i < body_length; i++) {
            animation_start_body[i] = body[i];        // la posicion actual
            animation_end_body[i].x = body[i].x;      // sin movimiento en x, solo quiero que "flote" para arriba
            animation_end_body[i].y = body[i].y + 99; // y que se mueva hasta muy arriba por eso sumo 99
        }
        animation_progress = 0.0f; // reinicio progreos animacion
    }

    bool animation_vacio_ended() {
        return this->animation_vacio_end;
    }

    bool is_dead_vacio() {
        if (this->dead_vacio) {
            return true;
        } else {
            return false;
        }
    }

    void moriste() {
        this->dead_spike = false;
        this->dead_vacio = false;
    }

    void move_right() {
        Point next_position = {head->x + 1, head->y};
        move_to(next_position);
    }

    void move_left() {
        Point next_position = {head->x - 1, head->y};
        move_to(next_position);
    }

    void move_up() {
        Point next_position = {head->x, head->y + 1};
        move_to(next_position);
    }

    void move_down() {
        Point next_position = {head->x, head->y - 1};
        move_to(next_position);
    }

    void draw() {
        if (animation && (!this->dead_spike)) {
            animation_handler();
        }
        draw_worm();
    }

    Point get_head() {
        return *head;
    }

    void set_animating_death(bool entry) {
        this->animating_death = entry;
    }

    bool is_animating_death() {
        return this->animating_death;
    }
};

#endif