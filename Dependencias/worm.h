#ifdef __APPLE__ // macOS
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#else
#include "SDL.h"
#include "SDL_opengl.h"
#endif

#include "cube.h"
#include "map_variable.h"
#include "point.h"
#include "variables.h"

#ifndef WORM_H
#define WORM_H

class Worm {
  public:
    Uint32 animation_start_time;
  private:
    Point* head;
    Point body[WORM_MAX_LENGTH];
    int body_length;
    bool animation;
    float animation_duration; // in seconds (with game speed 1)
    Point animation_start_body[WORM_MAX_LENGTH];
    Point animation_end_body[WORM_MAX_LENGTH];

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

    void animation_start_moving(Point next_position) {
        animation = true;
        animation_duration = 0.5f;
        animation_start_time = frame_actual;
        animation_start_body[0] = body[0];
        animation_end_body[0] = next_position;
        for (int i = 1; i < body_length; i++) {
            animation_start_body[i] = body[i];
            animation_end_body[i] = body[i - 1];
        }
    }

    void animation_start_falling(int distance) {
        animation = true;
        animation_duration = 0.5f * distance;
        animation_start_time = frame_actual;
        for (int i = 0; i < body_length; i++) {
            animation_start_body[i] = body[i];
            animation_end_body[i].x = body[i].x;
            animation_end_body[i].y = body[i].y - distance;
        }
    }

    // Calculates the progress of the animation, returning a value between 0 (start) and 1 (end)
    float animation_calculate_progress() {
        float miliseconds = 1000.0f;
        Uint32 elapsed_time = frame_actual - deltaPause - animation_start_time;
        float elapsed_time_in_seconds = elapsed_time / miliseconds;
        return (float) elapsed_time_in_seconds / animation_duration * game_speed;
    }

    void animation_move(float animation_progress) {
        for (int i = body_length - 1; i >= 0; i--) {
            body[i].x = (animation_start_body[i].x + animation_progress * (animation_end_body[i].x - animation_start_body[i].x));
            body[i].y = (animation_start_body[i].y + animation_progress * (animation_end_body[i].y - animation_start_body[i].y));
        }
    }

    void handle_is_not_supported() {
        int distance = get_distance_to_ground();
        if (distance > 0) {
            animation_start_falling(distance);
        }
    }

    void animation_handler() {
        if (!pause) {
            float animation_progress = animation_calculate_progress();
            if (animation_progress >= 1.0f) {
                animation_progress = 1.0f;
            }
            animation_move(animation_progress);
            if (animation_progress == 1.0f) {
                animation = false;
                deltaPause = 0;
                handle_is_not_supported();
            }
        }
    }

    void grow_while_moving() {
        body_length++;
        body[body_length - 1] = body[body_length - 2];
    }

    bool is_allowed_to_move(Point move_to) {
        // Check if game is paused
        if (pause)
            return false;

        // Check if is doing the animation
        if (animation)
            return false;

        // Chequeo colisión con el cuerpo
        if (is_worm_body_in_point(move_to))
            return false;

        // Chequeo colisión con bloques
        if (level_map.is_block_in_point(move_to))
            return false;

        // Check if the worm is growing
        if (level_map.is_apple_in_point(move_to)) {
            level_map.remove_apple(move_to);
            Mix_PlayChannel(-1, sfx_grow, 0);
            grow_while_moving();
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
        glColor3f(1.0f, 0.0f, 1.0f);
        drawCube(1.0f);
        glPopMatrix();
    }

    void draw_body() {
        for (int i = 1; i < body_length; i++) {
            glPushMatrix();
            glTranslatef(body[i].x, body[i].y, 0);
            glColor3f(0.0f, 1.0f, 0.0f);
            drawCube(0.99f);
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
        if (animation) {
            animation_handler();
        }
        draw_worm();
    }
};

#endif