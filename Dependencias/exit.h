#include "cube.h"
#include "point.h"

#ifndef EXIT_H
#define EXIT_H

class Exit {
  private:
    Point position;
    float textureYMinCoord, textureYMaxCoord;
    int positionTexYCoord;
    float animationCounter;

  public:
    Exit();
    ~Exit();

    void set_position(Point position);

    bool is_in(Point p);

    Point get_position();

    void draw();
};

Exit::Exit(){
    textureYMinCoord = 0.96875f;
    textureYMaxCoord = 1.0f;
    animationCounter = 0;
}

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

    float scale = 1.0f / 2.0f;
    glScalef(scale, scale, scale);

    if (textures) {

        glColor3f(0.25f, 0.0f, 0.38f);
        drawCube(1.8f);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, portalTexture);

        glColor3f(1.0f, 1.0f, 1.0f);

        //Cara frontal
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, textureYMinCoord);
        glVertex3f(-1.0, -1.0, 1.0);
        glTexCoord2f(1.0f, textureYMinCoord);
        glVertex3f(1.0, -1.0, 1.0);
        glTexCoord2f(1.0f, textureYMaxCoord);
        glVertex3f(1.0, 1.0, 1.0);
        glTexCoord2f(0.0f, textureYMaxCoord);
        glVertex3f(-1.0, 1.0, 1.0);
        glEnd();

        //Cara trasera
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, textureYMinCoord);
        glVertex3f(-1.0, -1.0, -1.0);
        glTexCoord2f(0.0f, textureYMaxCoord);
        glVertex3f(-1.0, 1.0, -1.0);
        glTexCoord2f(1.0f, textureYMaxCoord);
        glVertex3f(1.0, 1.0, -1.0);
        glTexCoord2f(1.0f, textureYMinCoord);
        glVertex3f(1.0, -1.0, -1.0);
        glEnd();

        //Cara superior
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, textureYMaxCoord);
        glVertex3f(-1.0, 1.0, -1.0);
        glTexCoord2f(0.0f, textureYMinCoord);
        glVertex3f(-1.0, 1.0, 1.0);
        glTexCoord2f(1.0f, textureYMinCoord);
        glVertex3f(1.0, 1.0, 1.0);
        glTexCoord2f(1.0f, textureYMaxCoord);
        glVertex3f(1.0, 1.0, -1.0);
        glEnd();

        //Cara inferior
        glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0.0f, textureYMaxCoord);
        glVertex3f(-1.0, -1.0, -1.0);
        glTexCoord2f(1.0f, textureYMaxCoord);
        glVertex3f(1.0, -1.0, -1.0);
        glTexCoord2f(1.0f, textureYMinCoord);
        glVertex3f(1.0, -1.0, 1.0);
        glTexCoord2f(0.0f, textureYMinCoord);
        glVertex3f(-1.0, -1.0, 1.0);
        glEnd();

        //Cara derecha
        glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, textureYMinCoord);
        glVertex3f(1.0, -1.0, -1.0);
        glTexCoord2f(1.0f, textureYMaxCoord);
        glVertex3f(1.0, 1.0, -1.0);
        glTexCoord2f(0.0f, textureYMaxCoord);
        glVertex3f(1.0, 1.0, 1.0);
        glTexCoord2f(0.0f, textureYMinCoord);
        glVertex3f(1.0, -1.0, 1.0);
        glEnd();

        //Cara izquierda
        glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, textureYMinCoord);
        glVertex3f(-1.0, -1.0, -1.0);
        glTexCoord2f(1.0f, textureYMinCoord);
        glVertex3f(-1.0, -1.0, 1.0);
        glTexCoord2f(1.0f, textureYMaxCoord);
        glVertex3f(-1.0, 1.0, 1.0);
        glTexCoord2f(0.0f, textureYMaxCoord);
        glVertex3f(-1.0, 1.0, -1.0);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        glPopMatrix();
    }
    else {
       
        glColor3f(1.0f, 1.0f, 1.0f);

        //Cara frontal
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-1.0, -1.0, 1.0);
        glVertex3f(1.0, -1.0, 1.0);
        glVertex3f(1.0, 1.0, 1.0);
        glVertex3f(-1.0, 1.0, 1.0);
        glEnd();

        //Cara trasera
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-1.0, -1.0, -1.0);
        glVertex3f(-1.0, 1.0, -1.0);
        glVertex3f(1.0, 1.0, -1.0);
        glVertex3f(1.0, -1.0, -1.0);
        glEnd();

        //Cara superior
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0, 1.0, -1.0);
        glVertex3f(-1.0, 1.0, 1.0);
        glVertex3f(1.0, 1.0, 1.0);
        glVertex3f(1.0, 1.0, -1.0);
        glEnd();

        //Cara inferior
        glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-1.0, -1.0, -1.0);
        glVertex3f(1.0, -1.0, -1.0);
        glVertex3f(1.0, -1.0, 1.0);
        glVertex3f(-1.0, -1.0, 1.0);
        glEnd();

        //Cara derecha
        glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(1.0, -1.0, -1.0);
        glVertex3f(1.0, 1.0, -1.0);
        glVertex3f(1.0, -1.0, 1.0);
        glEnd();

        //Cara izquierda
        glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0, -1.0, -1.0);
        glVertex3f(-1.0, -1.0, 1.0);
        glVertex3f(-1.0, 1.0, 1.0);
        glVertex3f(-1.0, 1.0, -1.0);
        glEnd();

        glPopMatrix();
    }

    if (!pause) {
        animationCounter = animationCounter + (deltaTime * game_speed / 2.0f);
    }

    if (animationCounter >= 0.032f) {
        animationCounter = 0.0f;
        if (textureYMinCoord == 0.0f) {
            textureYMinCoord = 0.96875f;
            textureYMaxCoord = 1.0f;
        }
        else {
            textureYMinCoord = textureYMinCoord - 0.03125f;
            textureYMaxCoord = textureYMaxCoord - 0.03125f;
        }
    }
}

Point Exit::get_position(){
  return position;
}

#endif