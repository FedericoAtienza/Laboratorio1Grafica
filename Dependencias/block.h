#include "cube.h"
#include "point.h"

#ifndef BLOCK_H
#define BLOCK_H

class Block {
  private:
    Point position;

  public:
    Block(Point position);
    ~Block();

    bool is_in(Point p);

    void draw();
};

Block::Block(Point position) {
    this->position = position;
}

Block::~Block() {
}

bool Block::is_in(Point p) {
    return (position.x == p.x && position.y == p.y);
}

void Block::draw() {
    glPushMatrix();
    glTranslatef(position.x, position.y, 0);
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCube(1.0f);
    glPopMatrix();
}

#endif