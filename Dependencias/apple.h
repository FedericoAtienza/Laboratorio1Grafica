#include "cube.h"
#include "point.h"

#ifndef APPLE_H
#define APPLE_H

class Apple {
  private:
    Point position;

  public:
    Apple(Point position);
    ~Apple();

    bool is_in(Point p);

    void draw();
};

Apple::Apple(Point position) {
    this->position = position;
}

Apple::~Apple() {
}

bool Apple::is_in(Point p) {
    return (position.x == p.x && position.y == p.y);
}

void Apple::draw() {
    glPushMatrix();
    glTranslatef(position.x, position.y, 0);
    glColor3f(0.0f, 0.0f, 1.0f);
    drawCube(1.0f);
    glPopMatrix();
}

#endif