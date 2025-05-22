#include "apple.h"
#include "block.h"
#include "exit.h"
#include "explosive.h"
#include "spike.h"
#include "point.h"
#include "variables.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#ifndef MAP_H
#define MAP_H

using namespace tinyxml2;

class Map {
  private:
    /* Componentes del mapa: bloques, manzanas, exit (por ahora) */
    int lvl;
    std::vector<Block> blocks;
    std::vector<Apple> apples;
    std::vector<Explosive> explosives;
    std::vector<Spike> spikes;
    Exit exit;
    Point spawn; // Punto de spawn

    // Esto no se si ira finalmente aca o en otro archivo de utilities
    // Para cargar las posiciones de los componentes del nivel (bloques, manzanas, pinchos, ...)
    std::vector<Point> cargarUbicaciones(const std::string& filename);

  public:
    Map(int nivel);

    int get_distance_to_ground(Point p);

    bool is_block_in_point(Point p);

    bool is_apple_in_point(Point p);

    bool is_exit_in_point(Point p);

    bool is_explosive_in_point(Point p);

    bool is_spike_in_point(Point p);

    void remove_apple(Point p);

    void draw();

    int apple_quantity();

    int level_number();

    Point get_spawn();

    Point get_exit();

    bool check_explosives();
};

Map::Map(int nivel) {
    std::string load_level = "../Dependencias/levels/level" + std::to_string(nivel) + ".xml";
    cargarUbicaciones(load_level);
}

int Map::get_distance_to_ground(Point p) {
    int distance = MAX_MAP_HIGH;
    int distance_aux;
    Point object_position;
    for (auto& block : blocks) {
        object_position = block.get_position();
        if (object_position.x == p.x && object_position.y < p.y) {
            distance_aux = p.y - object_position.y;
            if (distance > distance_aux) {
                distance = distance_aux;
            }
        }
    }
    for (auto& apple : apples) {
        object_position = apple.get_position();
        if (object_position.x == p.x && object_position.y < p.y) {
            distance_aux = p.y - object_position.y;
            if (distance > distance_aux) {
                distance = distance_aux;
            }
        }
    }
    object_position = exit.get_position();
    if (object_position.x == p.x && object_position.y < p.y) {
        distance_aux = p.y - object_position.y;
        if (distance > distance_aux) {
            distance = distance_aux;
        }
    }
    for (auto& explosive : explosives) {
        object_position = explosive.get_position();
        if (object_position.x == p.x && object_position.y < p.y) {
            distance_aux = p.y - object_position.y;
            if (distance > distance_aux) {
                distance = distance_aux;
            }
        }
    }
    return distance;
}

bool Map::is_block_in_point(Point p) {
    for (auto& block : blocks) {
        if (block.is_in(p)) {
            return true;
        }
    }
    return false;
}

bool Map::is_apple_in_point(Point p) {
    for (auto& apple : apples) {
        if (apple.is_in(p)) {
            return true;
        }
    }
    return false;
}

bool Map::is_exit_in_point(Point p) {
    if (exit.is_in(p)) {
        return true;
    }
    return false;
}

bool Map::is_explosive_in_point(Point p) {
    for (auto& explosive : explosives) {
        if (explosive.is_in(p)) {
            return true;
        }
    }
    return false;
}

void Map::remove_apple(Point p) {
    for (auto it = apples.begin(); it != apples.end(); ++it) {
        if (it->is_in(p)) {
            apples.erase(it);
            return;
        }
    }
}

void Map::draw() {
    for (auto& block : blocks) {
        block.draw(true);
    }
    for (auto& apple : apples) {
        apple.draw();
    }
    for (auto& explosive : explosives) {
        explosive.draw();
    }
    exit.draw();
    for (auto& spike : spikes) {
        spike.draw();
    }
}

/*
std::vector<Point> Map::cargarUbicaciones(const std::string& nombreArchivo) {
    std::ifstream file(nombreArchivo);
    std::vector<Point> ubicaciones;
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << nombreArchivo << std::endl;
        return ubicaciones;
    }

    std::string linea;
    // Voy a ir leyendo linea por linea y lo guardo en el vector de puntos
    while (std::getline(file, linea) && !linea.empty()) {
        std::istringstream iss(linea);
        char caracter;
        iss >> caracter;

        if (caracter == 'B') { // Bloques
            float x, y;
            iss >> x >> y;
            blocks.push_back(Block({x, y}));
        } else if (caracter == 'M') { // Manzanas
            float x, y;
            iss >> x >> y;
            apples.push_back(Apple({x, y}));
        } else if (caracter == 'E') { // Exit
            float x, y;
            iss >> x >> y;
            exit.set_position({x, y});
        } else if (caracter == 'S') { // Posicion de spawn del gusano
            float x, y;
            iss >> x >> y;
            spawn = {x,y};
        } else if (caracter == 'L') { // Indicador de Level Number
            float x;
            iss >> x;
            this->lvl = x;
        }
        else if (caracter == 'T') {
            float x, y;
            iss >> x >> y;
            explosives.push_back(Explosive({ x, y }));
        }
    }

    file.close();
    return ubicaciones;
}
*/

std::vector<Point> Map::cargarUbicaciones(const std::string& nombreArchivo) {
    std::vector<Point> ubicaciones;

    XMLDocument doc;
    XMLError result = doc.LoadFile(nombreArchivo.c_str());
    if (result != XML_SUCCESS) {
        std::cerr << "No se pudo abrir el archivo XML: " << nombreArchivo << std::endl;
        return ubicaciones;
    }

    XMLElement* root = doc.FirstChildElement("level");
    if (!root) {
        std::cerr << "No se encontró el elemento <level> en el archivo XML." << std::endl;
        return ubicaciones;
    }

    // Cargar número de nivel
    root->QueryIntAttribute("number", &this->lvl);

    // Bloques
    XMLElement* block = root->FirstChildElement("blocks") ? root->FirstChildElement("blocks")->FirstChildElement("block") : nullptr;
    while (block) {
        float x = block->FloatAttribute("x");
        float y = block->FloatAttribute("y");
        blocks.push_back(Block({x, y}));
        block = block->NextSiblingElement("block");
    }

    // Manzanas
    XMLElement* apple = root->FirstChildElement("apples") ? root->FirstChildElement("apples")->FirstChildElement("apple") : nullptr;
    while (apple) {
        float x = apple->FloatAttribute("x");
        float y = apple->FloatAttribute("y");
        apples.push_back(Apple({x, y}));
        apple = apple->NextSiblingElement("apple");
    }

    // Pinchos
    XMLElement* spike = root->FirstChildElement("spikes") ? root->FirstChildElement("spikes")->FirstChildElement("spike") : nullptr;
    while (spike) {
        float x = spike->FloatAttribute("x");
        float y = spike->FloatAttribute("y");
        spikes.push_back(Spike({x, y}));
        spike = spike->NextSiblingElement("spike");
    }

    // Entrada (posición del gusano al iniciar)
    XMLElement* spawn = root->FirstChildElement("spawn");
    if (spawn) {
        float x = spawn->FloatAttribute("x");
        float y = spawn->FloatAttribute("y");
        this->spawn = {x, y};
    }

    // Salida (meta del nivel)
    XMLElement* exitElement = root->FirstChildElement("exit");
    if (exitElement) {
        float x = exitElement->FloatAttribute("x");
        float y = exitElement->FloatAttribute("y");
        exit.set_position({x, y});
    }

    return ubicaciones;
}

bool Map::is_spike_in_point(Point p) {
    for (auto& spike : spikes) {
        if (spike.is_in(p)) {
            return true;
        }
    }
    return false;
}

Point Map::get_spawn() {
    return spawn;
}

Point Map::get_exit() {
    return exit.get_position();
}

int Map::apple_quantity() {
    return apples.size();
}

int Map::level_number() {
    return this->lvl;
}

bool Map::check_explosives() {
    bool contact = false;
    return false;
}

#endif