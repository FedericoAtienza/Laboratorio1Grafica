#include "cube.h"
#include "models.h"
#include "point.h"

#ifndef APPLE_H
#define APPLE_H

class Apple {
  private:
    Point position;

  public:
    Apple(Point position);
    ~Apple();

    Point get_position();

    bool is_in(Point p);

    void draw();
};

Apple::Apple(Point position) {
    this->position = position;
}

Apple::~Apple() {
}

Point Apple::get_position() {
    return position;
}

bool Apple::is_in(Point p) {
    return (position.x == p.x && position.y == p.y);
}

void Apple::draw() {
    glPushMatrix();
    glTranslatef(position.x, position.y, 0);
    glTranslatef(0.0f, -0.45f, -0.1f);
    glScalef(0.07f, 0.07f, 0.07f);
    appleModel.Draw(textures, false);
    glPopMatrix();
}

#endif