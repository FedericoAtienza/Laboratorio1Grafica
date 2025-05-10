#ifndef MODEL_H
#define MODEL_H

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <mesh.h>

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory);

class Model {
  public:
    // Datos del Modelo
    vector<Texture> textures_loaded; // Almacena todas las texturas cargadas, optimizado para asegurarse que
                                     // algunas texturas no sean cargadas mas de una vez.
    vector<Mesh> meshes;             // Meshes del modelo
    string directory;                // Directorio del modelo

    // Constructor, requiere de la ruta del archivo para cargar el modelo.
    Model() {
    }

    // Dibuja el modelo, lo cual equivale a dibujar todos sus meshes
    void Draw(bool conTexturas, bool conRojo) {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(conTexturas, conRojo);
    }

    // Carga el modelo con Assimp y almacena los meshes resultantes en el vector de meshes.
    void loadModel(string const& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
        // Revision de errores
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // Obtiene la ruta de directorio de la ruta del archivo
        directory = path.substr(0, path.find_last_of('/'));

        // Procesa los nodos de Assimp de manera recursiva
        processNode(scene->mRootNode, scene);
    }

  private:
    // Procesa un nodo de manera recursiva. Procesa cada mesh localizado en el nodo y repite este
    // proceso en los nodos hijos (si tiene).
    void processNode(aiNode* node, const aiScene* scene) {
        // Procesa cada mesh localizado en el nodo actual
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            // El objeto nodo solo contiene indices para encontrar el mesh actual en la escena.
            // La escena contiene toda la informacion, el nodo se utiliza solamente para mantener las cosas organizadas.
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // Luego de que procesamos todos los meshes (si hubo alguno) pasamos a procesar de forma recursiva
        // cada uno de los nodos hijos.
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
        // Datos a completar
        map<int, Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        // Recorremos cada uno de los vertices del mesh.
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            glm::vec3 vector; // Declaramos un vector auxiliar ya que Assimp utiliza su propia clase vector la cual
                              // no se convierte directamente en la clase vec3 de glm, por lo tanto transferimos
                              // los datos a este auxiliar primero.
            // Posiciones
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // Normales
            if (mesh->HasNormals()) {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // Coordenadas de texturas
            if (mesh->mTextureCoords[0]) // El mesh contiene coordenadas de texturas?
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            } else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices[i] = vertex;
        }
        // Ahora recorremos las caras del mesh (una cara es un triangulo del mesh) y obtenemos el vector de indices
        // correspondiente.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            // Obtenemos todos los indices de la cara y los guardamos en el vector de indices.
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // Procesar materiales
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        /*// 2. specular maps
        //vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());*/

        // Devuelve un objeto mesh creado a partir de los datos extraidos.
        return Mesh(vertices, indices, textures);
    }

    // Revisa todas las texturas de un cierto tipo para un material dado y las carga si aun no fueron cargadas.
    // La informacion requerida es retornada como un struct de Texture
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type) {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            // Chequea si la textura ya fue cargada y si lo fue continua con la siguiente iteracion,
            // saltea cargar una nueva textura.
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++) {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // Una textura con la misma ruta de archivo ya fue cargada,
                                 // continua con la siguiente (optimizacion)
                    break;
                }
            }
            if (!skip) { // Si la textura no fue cargada, se carga
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture); // La recuerda como textura guardada para el modelo entero,
                                                    // y asi asegurarse de no cargar innecesariamente texturas
                                                    // duplicadas
            }
        }
        return textures;
    }
};

GLuint TextureFromFile(const char* path, const string& directory) {
    string filename = string(path);
    filename = directory + '/' + filename;

    const char* aux = filename.c_str();

    // CARGAR IMAGEN
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(aux);
    FIBITMAP* bitmap = FreeImage_Load(fif, aux);
    bitmap = FreeImage_ConvertTo24Bits(bitmap);
    int w = FreeImage_GetWidth(bitmap);
    int h = FreeImage_GetHeight(bitmap);
    void* datos = FreeImage_GetBits(bitmap);
    // FIN CARGAR IMAGEN

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, datos);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // FIN TEXTURA

    return textureID;
}
#endif
