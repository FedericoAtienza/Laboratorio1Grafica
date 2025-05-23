#ifndef LIGHT_H
#define LIGHT_H

#include <array>

class Light {
  private:
    std::array<GLfloat, 4> position;
    std::array<GLfloat, 4> color;

  public:
    Light();
    ~Light();

    void draw();

    std::array<GLfloat, 4> get_position();

    void set_position(Point position);

    void set_color(float r, float g, float b);
};

Light::Light() {
}

Light::~Light() {
}

std::array<GLfloat, 4> Light::get_position() {
    return this->position;
}

void Light::set_position(Point position) {
    this->position[0] = position.x;
    this->position[1] = position.y;
    this->position[2] = 0.0f;
    this->position[3] = 1.0f;
}

void Light::set_color(float r, float g, float b) {
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
    this->color[3] = 1.0f;
}

void Light::draw() {
    if (!light) {
        return;
    }

    GLfloat glPosition[4] = {position[0], position[1], position[2], position[3]};
    GLfloat glColor[4] = {color[0], color[1], color[2], color[3]};

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, glPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, glColor);
}

#endif