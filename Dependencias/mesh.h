#ifndef MESH_H
#define MESH_H

using namespace std;

struct Vertex {
    //Posicion
    glm::vec3 Position;
    //Normal
    glm::vec3 Normal;
    //Coordenadas de textura
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    string path;
};

class Mesh {
public:
    //Datos del mesh
    map<int,Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;

    //Constructor
    Mesh(map<int,Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
    }

    //Dibuja el mesh
    void Draw(bool conTexturas, bool conRojo)
    {
        
        if (conTexturas) {
            //Preparacion de las texturas, si deben ser dibujadas
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textures[0].id);
        }
        
        Vertex ver1, ver2, ver3;

        auto iterador = indices.begin();
        float colorAux;

        if (conRojo) {
            colorAux = 0;
        }
        else {
            colorAux = 1;
        }

        //Tres vertices tomados del vector de indices serán los que formaran un triangulo a dibujar,
        //iteramos de a tres vertices en los indices y asi dibujamos todos los triangulos.
        while (iterador != indices.end()) {

            ver1 = vertices[*iterador];
            iterador++;
            ver2 = vertices[*iterador];
            iterador++;
            ver3 = vertices[*iterador];
            iterador++;

            glBegin(GL_TRIANGLES);
            glColor3f(1, colorAux, colorAux);
            glNormal3f(ver1.Normal.x, ver1.Normal.y, ver1.Normal.z);
            glTexCoord2f(ver1.TexCoords.x, ver1.TexCoords.y);
            glVertex3f(ver1.Position.x, ver1.Position.y, ver1.Position.z);
            glNormal3f(ver2.Normal.x, ver2.Normal.y, ver2.Normal.z);
            glTexCoord2f(ver2.TexCoords.x, ver2.TexCoords.y);
            glVertex3f(ver2.Position.x, ver2.Position.y, ver2.Position.z);
            glNormal3f(ver3.Normal.x, ver3.Normal.y, ver3.Normal.z);
            glTexCoord2f(ver3.TexCoords.x, ver3.TexCoords.y);
            glVertex3f(ver3.Position.x, ver3.Position.y, ver3.Position.z);
            glEnd();

        }

        glDisable(GL_TEXTURE_2D);

    }

private:
    
};
#endif
