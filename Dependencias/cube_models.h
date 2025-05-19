#ifndef CUBE_MODELS_H
#define CUBE_MODELS_H

// CARGADO DE IMAGENES
GLuint LoadTexture(const char* filename) {
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
    FIBITMAP* image = FreeImage_Load(format, filename);
    FIBITMAP* image32 = FreeImage_ConvertTo32Bits(image);

    int width = FreeImage_GetWidth(image32);
    int height = FreeImage_GetHeight(image32);
    BYTE* bits = FreeImage_GetBits(image32);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_BGRA, GL_UNSIGNED_BYTE, bits);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FreeImage_Unload(image32);
    FreeImage_Unload(image);

    return textureID;
};

GLuint worm1Textures[6];

void loadWorm1Model() {
    worm1Textures[0] = LoadTexture("../Dependencias/TexturasWorm/texture_worm_1.png"); // Cara derecha
    worm1Textures[1] = worm1Textures[0];                                               // Cara izquierda
    worm1Textures[2] = worm1Textures[0];                                               // Cara arriba
    worm1Textures[3] = worm1Textures[0];                                               // Cara abajo
    worm1Textures[4] = worm1Textures[0];                                               // Cara enfrente
    worm1Textures[5] = worm1Textures[0];                                               // Cara trasera
}

GLuint worm2Textures[6];

void loadWorm2Model() {
    worm2Textures[0] = LoadTexture("../Dependencias/TexturasWorm/texture_worm_2.png"); // Cara derecha
    worm2Textures[1] = worm2Textures[0];                                               // Cara izquierda
    worm2Textures[2] = worm2Textures[0];                                               // Cara arriba
    worm2Textures[3] = worm2Textures[0];                                               // Cara abajo
    worm2Textures[4] = worm2Textures[0];                                               // Cara enfrente
    worm2Textures[5] = worm2Textures[0];                                               // Cara trasera
}

GLuint wormHeadTextures[6];

void loadWormHeadModel() {
    wormHeadTextures[0] = LoadTexture("../Dependencias/TexturasWorm/texture_worm_2.png");    // Cara derecha
    wormHeadTextures[1] = wormHeadTextures[0];                                               // Cara izquierda
    wormHeadTextures[2] = wormHeadTextures[0];                                               // Cara arriba
    wormHeadTextures[3] = wormHeadTextures[0];                                               // Cara abajo
    wormHeadTextures[4] = LoadTexture("../Dependencias/TexturasWorm/texture_worm_head.png"); // Cara enfrente
    wormHeadTextures[5] = wormHeadTextures[0];                                               // Cara trasera
}

GLuint skyboxTextures[6];

void loadSkybox() {
    skyboxTextures[0] = LoadTexture("../Dependencias/Skyboxes/elyvisions/rainbow_rt.png"); // Cara derecha
    skyboxTextures[1] = LoadTexture("../Dependencias/Skyboxes/elyvisions/rainbow_lf.png"); // Cara izquierda
    skyboxTextures[2] = LoadTexture("../Dependencias/Skyboxes/elyvisions/rainbow_up.png"); // Cara arriba
    skyboxTextures[3] = LoadTexture("../Dependencias/Skyboxes/elyvisions/rainbow_dn.png"); // Cara abajo
    skyboxTextures[4] = LoadTexture("../Dependencias/Skyboxes/elyvisions/rainbow_ft.png"); // Cara enfrente
    skyboxTextures[5] = LoadTexture("../Dependencias/Skyboxes/elyvisions/rainbow_bk.png"); // Cara trasera
}

void loadCubesTextures() {
    loadWorm1Model();
    loadWorm2Model();
    loadWormHeadModel();
    loadSkybox();
}

void drawModel(float scale, GLuint* textures) {
    scale = scale / 2.0f;
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);

    glScalef(scale, scale, scale);
    // Cada cara del cubo
    for (int i = 0; i < 6; ++i) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        switch (i) {
        case 0: // Cara derecha
            glNormal3f(1.0f, 0.0f, 0.0f);
            glTexCoord2f(0, 0);
            glVertex3f(1, -1, -1);
            glTexCoord2f(1, 0);
            glVertex3f(1, -1, 1);
            glTexCoord2f(1, 1);
            glVertex3f(1, 1, 1);
            glTexCoord2f(0, 1);
            glVertex3f(1, 1, -1);
            break;
        case 1: // Cara izquierda
            glNormal3f(-1.0f, 0.0f, 0.0f);
            glTexCoord2f(0, 0);
            glVertex3f(-1, -1, 1);
            glTexCoord2f(1, 0);
            glVertex3f(-1, -1, -1);
            glTexCoord2f(1, 1);
            glVertex3f(-1, 1, -1);
            glTexCoord2f(0, 1);
            glVertex3f(-1, 1, 1);
            break;
        case 2: // Cara superior
            glNormal3f(0.0f, 1.0f, 0.0f);
            glTexCoord2f(0, 0);
            glVertex3f(-1, 1, -1);
            glTexCoord2f(1, 0);
            glVertex3f(1, 1, -1);
            glTexCoord2f(1, 1);
            glVertex3f(1, 1, 1);
            glTexCoord2f(0, 1);
            glVertex3f(-1, 1, 1);
            break;
        case 3: // Cara inferior
            glNormal3f(0.0f, -1.0f, 0.0f);
            glTexCoord2f(0, 0);
            glVertex3f(-1, -1, 1);
            glTexCoord2f(1, 0);
            glVertex3f(1, -1, 1);
            glTexCoord2f(1, 1);
            glVertex3f(1, -1, -1);
            glTexCoord2f(0, 1);
            glVertex3f(-1, -1, -1);
            break;
        case 4: // Cara frontal
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(0, 0);
            glVertex3f(1, -1, 1);
            glTexCoord2f(1, 0);
            glVertex3f(-1, -1, 1);
            glTexCoord2f(1, 1);
            glVertex3f(-1, 1, 1);
            glTexCoord2f(0, 1);
            glVertex3f(1, 1, 1);
            break;
        case 5: // Cara trasera
            glNormal3f(0.0f, 0.0f, -1.0f);
            glTexCoord2f(0, 0);
            glVertex3f(-1, -1, -1);
            glTexCoord2f(1, 0);
            glVertex3f(1, -1, -1);
            glTexCoord2f(1, 1);
            glVertex3f(1, 1, -1);
            glTexCoord2f(0, 1);
            glVertex3f(-1, 1, -1);
            break;
        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawWorm1Model(float scale) {
    drawModel(scale, worm1Textures);
}

void drawWorm2Model(float scale) {
    drawModel(scale, worm2Textures);
}

void drawWormHeadModel(float scale) {
    drawModel(scale, wormHeadTextures);
}

// Implementación diferente porque las normales están invertidas
void DrawSkybox(float size) {
    glPushMatrix();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // Cada cara del cubo
    glScalef(size, size, size);
    for (int i = 0; i < 6; ++i) {
        glBindTexture(GL_TEXTURE_2D, skyboxTextures[i]);
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        switch (i) {
        case 0: // Cara derecha
            glNormal3f(-1.0f, 0.0f, 0.0f);
            glTexCoord2f(0, 0);
            glVertex3f(1, -1, -1);
            glTexCoord2f(1, 0);
            glVertex3f(1, -1, 1);
            glTexCoord2f(1, 1);
            glVertex3f(1, 1, 1);
            glTexCoord2f(0, 1);
            glVertex3f(1, 1, -1);
            break;
        case 1: // Cara izquierda
            glNormal3f(1.0f, 0.0f, 0.0f);
            glTexCoord2f(0, 0);
            glVertex3f(-1, -1, 1);
            glTexCoord2f(1, 0);
            glVertex3f(-1, -1, -1);
            glTexCoord2f(1, 1);
            glVertex3f(-1, 1, -1);
            glTexCoord2f(0, 1);
            glVertex3f(-1, 1, 1);
            break;
        case 2: // Cara superior
            glNormal3f(0.0f, -1.0f, 0.0f);
            glTexCoord2f(0, 0);
            glVertex3f(-1, 1, -1);
            glTexCoord2f(1, 0);
            glVertex3f(1, 1, -1);
            glTexCoord2f(1, 1);
            glVertex3f(1, 1, 1);
            glTexCoord2f(0, 1);
            glVertex3f(-1, 1, 1);
            break;
        case 3: // Cara inferior
            glNormal3f(0.0f, 1.0f, 0.0f);
            glTexCoord2f(0, 0);
            glVertex3f(-1, -1, 1);
            glTexCoord2f(1, 0);
            glVertex3f(1, -1, 1);
            glTexCoord2f(1, 1);
            glVertex3f(1, -1, -1);
            glTexCoord2f(0, 1);
            glVertex3f(-1, -1, -1);
            break;
        case 4: // Cara frontal
            glNormal3f(0.0f, 0.0f, -1.0f);
            glTexCoord2f(0, 0);
            glVertex3f(1, -1, 1);
            glTexCoord2f(1, 0);
            glVertex3f(-1, -1, 1);
            glTexCoord2f(1, 1);
            glVertex3f(-1, 1, 1);
            glTexCoord2f(0, 1);
            glVertex3f(1, 1, 1);
            break;
        case 5: // Cara trasera
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(0, 0);
            glVertex3f(-1, -1, -1);
            glTexCoord2f(1, 0);
            glVertex3f(1, -1, -1);
            glTexCoord2f(1, 1);
            glVertex3f(1, 1, -1);
            glTexCoord2f(0, 1);
            glVertex3f(-1, 1, -1);
            break;
        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}

#endif // CUBE_MODELS_H