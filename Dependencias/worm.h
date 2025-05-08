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
    int body_length;
    bool animation;
    Uint32 animation_start_time;
    Point animation_end_head;
    Point animation_start_body[WORM_MAX_LENGTH];
    //bool move_and_grow;

    // Check if the worm is in the given position
    bool is_worm_body_in_point(Point p) {
        for (int i = 0; i < body_length; i++) {
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
        for (int i = 0; i < body_length; i++) {
            animation_start_body[i].x = body[i].x;
            animation_start_body[i].y = body[i].y;
        }
    }

    void grow_while_moving() {
        body_length++;
        body[body_length - 1] = body[body_length - 2];
    }

    bool is_allowed_to_move(Point move_to) {
        // Check if is doing the animation
        if (animation) {
            return false;
        }

        // Chequeo clision con el cuerpo
        if (is_worm_body_in_point(move_to)) {
            return false;
        }

        // Chequeo colision con bloques
        if (level_map.is_block_in_point(move_to)) {
            return false;
        }

        // Check if the worm is growing
        if (level_map.is_apple_in_point(move_to)) {
            level_map.remove_apple(move_to);
            grow_while_moving();
        }

        return true;
    }

  public:
    // Constructor
    Worm(Point head) {
        this->body_length = 2;
        this->body[0] = head;
        this->head = &this->body[0];
        this->body[1] = {head.x - 1, head.y};
        //move_and_grow = false;
    }

    /*
    void set_grow(bool move_and_grow) {
        this->move_and_grow = move_and_grow;
    }
    

    bool is_growing() {
        return move_and_grow;
    }
    */

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
            for (int i = body_length - 1; i > 0; i--) {
                body[i].x = (animation_start_body[i].x + t * (animation_start_body[i - 1].x - animation_start_body[i].x));
                body[i].y = (animation_start_body[i].y + t * (animation_start_body[i - 1].y - animation_start_body[i].y));
            }
        }
        
        // Dibujo cabeza
        glPushMatrix();
        glTranslatef(head->x, head->y, 0);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawCube(1.0f);
        glPopMatrix();

        // Dibujo cuerpo
        for (int i = 1; i < body_length; i++) {
            glPushMatrix();
            glTranslatef(body[i].x, body[i].y, 0);
            glColor3f(0.0f, 1.0f, 0.0f);
            drawCube(0.99f);
            glPopMatrix();
        }
    }
};

#endif