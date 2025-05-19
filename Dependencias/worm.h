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
#include "cube_models.h"

#ifndef WORM_H
#define WORM_H

class Worm {
  private:
    Point* head;
    Point body[WORM_MAX_LENGTH];
    float body_rotation[WORM_MAX_LENGTH];
    int body_length;
    bool animation;
    float animation_progress;
    float animation_duration; // in seconds (with game speed 1)
    Point animation_start_body[WORM_MAX_LENGTH];
    float animation_start_body_rotation[WORM_MAX_LENGTH];
    Point animation_end_body[WORM_MAX_LENGTH];
    float animation_end_body_rotation[WORM_MAX_LENGTH];

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
    }

    void handle_is_not_supported() {
        int distance = get_distance_to_ground();
        if (distance > 0) {
            animation_start_falling(distance);
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

        if (level_map.is_exit_in_point(move_to)) {
            std::cout << "ENTRASTE A SALIDA!" << std::endl;
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
        glColor3f(1.0f, 0.0f, 1.0f);
        glScalef(0.01f, 0.01f, 0.01f);
        wormHeadModel.Draw(textures, false);
        glPopMatrix();
    }

    void draw_body() {
        for (int i = 1; i < body_length; i++) {
            glPushMatrix();
            glTranslatef(body[i].x, body[i].y, 0);
            glRotatef(body_rotation[i], 0.0f, 0.0f, 1.0f);
            glColor3f(0.0f, 1.0f, 0.0f);
            wormBodyModel.Draw(textures, false);
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
        this->animation = false;
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

    Point get_head() {
        return *head;
    }
};

#endif