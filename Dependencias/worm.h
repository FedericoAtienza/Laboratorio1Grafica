#ifdef __APPLE__ // macOS
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#else
#include "SDL.h"
#include "SDL_opengl.h"
#endif

#include "cube.h"
#include "point.h"
#include "variables.h"

#ifndef WORM_H
#define WORM_H

class Worm {
  private:
    // Attributes
    Point* head;
    Point body[WORM_MAX_LENGTH];
    int length;
    bool animation;
    Uint32 animation_start_time;
    Point animation_end_head;
    Point animation_start_body[WORM_MAX_LENGTH];
    bool move_and_grow;

    // Check if the worm is in the given position
    bool is_worm_in(Point p) {
        for (int i = 0; i < length; i++) {
            if (body[i].x == p.x && body[i].y == p.y) {
                return true;
            }
        }
        return false;
    }

    void start_animation(Point next_position) {
        animation_end_head = next_position;
        animation = true;
        animation_start_time = frame_actual;
        for (int i = 0; i < length; i++) {
            animation_start_body[i].x = body[i].x;
            animation_start_body[i].y = body[i].y;
        }
    }

    void grow_while_moving() {
        length++;
        body[length - 1] = body[length - 2];
    }

    bool is_allowed_to_move(Point move_to) {
        // Check if is doing the animation
        if (animation) {
            return false;
        }

        // Check if there is worm body in the next position
        if (is_worm_in(move_to)) {
            return false;
        }

        if (my_map.is_block_in_point(move_to)) {
            return false;
        }

        // Check if the worm is growing
        if (my_map.is_apple_in_point(move_to)) {
            my_map.remove_apple(move_to);
            grow_while_moving();
        }

        return true;
    }

  public:
    // Constructor
    Worm(Point head) {
        this->length = 2;
        this->body[0] = head;
        this->head = &this->body[0];
        this->body[1] = {head.x - 1, head.y};
        move_and_grow = false;
    }

    void set_grow(bool move_and_grow) {
        this->move_and_grow = move_and_grow;
    }

    bool is_growing() {
        return move_and_grow;
    }

    void move_to(Point next_position) {
        if (!is_allowed_to_move(next_position))
            return;
        start_animation(next_position);
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
        // Check if the worm is animating
        if (animation) {
            Uint32 elapsed_time = frame_actual - animation_start_time;
            float t = (float) elapsed_time / 500.0f * game_speed; // Animation duration
            if (t >= 1.0f) {
                t = 1.0f;
                animation = false;
            }
            body[0].x = (animation_start_body[0].x + t * (animation_end_head.x - animation_start_body[0].x));
            body[0].y = (animation_start_body[0].y + t * (animation_end_head.y - animation_start_body[0].y));
            for (int i = length - 1; i > 0; i--) {
                body[i].x = (animation_start_body[i].x + t * (animation_start_body[i - 1].x - animation_start_body[i].x));
                body[i].y = (animation_start_body[i].y + t * (animation_start_body[i - 1].y - animation_start_body[i].y));
            }
        }
        // Draw the head
        glPushMatrix();
        glTranslatef(head->x, head->y, 0);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawCube(1.0f);
        glPopMatrix();
        // Draw the body
        for (int i = 1; i < length; i++) {
            glPushMatrix();
            glTranslatef(body[i].x, body[i].y, 0);
            glColor3f(0.0f, 1.0f, 0.0f);
            drawCube(0.99f);
            glPopMatrix();
        }
    }
};

#endif