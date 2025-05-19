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

    Point get_position();

    bool is_in(Point p);

    void draw(bool conTextura);
};

Block::Block(Point position) {
    this->position = position;
}

Block::~Block() {
}

Point Block::get_position() {
    return position;
}

bool Block::is_in(Point p) {
    return (position.x == p.x && position.y == p.y);
}

void Block::draw(bool conTextura) {
    if (conTextura && textures) {
        glPushMatrix();
        glTranslatef(position.x, position.y, 0);

        float scale = 1.0f / 2.0f;
        glScalef(scale, scale, scale);

        glEnable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);

        //Cara frontal
        glBindTexture(GL_TEXTURE_2D, dirtBlockSide);

        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0, -1.0, 1.0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0, -1.0, 1.0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0, 1.0, 1.0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0, 1.0, 1.0);
        glEnd();

        //Cara trasera
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0, -1.0, -1.0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0, 1.0, -1.0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0, 1.0, -1.0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0, -1.0, -1.0);
        glEnd();

        //Cara superior
        glBindTexture(GL_TEXTURE_2D, dirtBlockTop);

        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0, 1.0, -1.0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0, 1.0, 1.0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0, 1.0, 1.0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0, 1.0, -1.0);
        glEnd();

        //Cara inferior
        glBindTexture(GL_TEXTURE_2D, dirtBlockBottom);

        glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0, -1.0, -1.0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0, -1.0, -1.0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0, -1.0, 1.0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0, -1.0, 1.0);
        glEnd();

        //Cara derecha
        glBindTexture(GL_TEXTURE_2D, dirtBlockSide);

        glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0, -1.0, -1.0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0, 1.0, -1.0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0, 1.0, 1.0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0, -1.0, 1.0);
        glEnd();

        //Cara izquierda
        glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0, -1.0, -1.0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0, -1.0, 1.0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0, 1.0, 1.0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0, 1.0, -1.0);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        glPopMatrix();
    }
    else {
        glPushMatrix();
        glTranslatef(position.x, position.y, 0);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCube(1.0f);
        glPopMatrix();
    }
}

#endif