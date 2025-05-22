#include "cube.h"
#include "models.h"
#include "point.h"
#include "variables.h"

#ifndef SPIKE_H
#define SPIKE_H

class Spike {
  private:
    Point position;

  public:
    Spike(Point position);
    ~Spike();

    Point get_position();

    bool is_in(Point p);

    void draw();
};

Spike::Spike(Point position) {
    this->position = position;
}

Spike::~Spike() {
}

Point Spike::get_position() {
    return position;
}

bool Spike::is_in(Point p) {
    return (position.x == p.x && position.y == p.y);
}

void Spike::draw() {
    glPushMatrix();
    glTranslatef(position.x, position.y, 0);
    glTranslatef(0.0f, -0.45f, 0.0f);
    glScalef(0.6f, 0.6f, 0.6f);
    spikeModel.Draw(textures, false, false);
    glPopMatrix();
}

#endif