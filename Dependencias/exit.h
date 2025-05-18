#include "cube.h"
#include "point.h"

#ifndef EXIT_H
#define EXIT_H

class Exit {
  private:
    Point position;

  public:
    Exit();
    ~Exit();

    void set_position(Point position);

    bool is_in(Point p);

    Point get_position();

    void draw();
};

Exit::Exit(){}

void Exit::set_position(Point position) {
    this->position = position;
}

Exit::~Exit() {
}

bool Exit::is_in(Point p) {
    return (position.x == p.x && position.y == p.y);
}

void Exit::draw() {
    glPushMatrix();
    glTranslatef(position.x, position.y, 0);
    glColor3f(0.55f, 0.27f, 0.07f); 
    drawCube(1.0f);
    glPopMatrix();
}

Point Exit::get_position(){
  return position;
}

#endif