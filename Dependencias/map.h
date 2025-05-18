#include "apple.h"
#include "block.h"
#include "exit.h"
#include "point.h"
#include "variables.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#ifndef MAP_H
#define MAP_H

class Map {
  private:
    /* Componentes del mapa: bloques, manzanas, exit (por ahora) */
    int lvl;
    std::vector<Block> blocks;
    std::vector<Apple> apples;
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

    void remove_apple(Point p);

    void draw();

    int apple_quantity();

    int level_number();

    Point get_spawn();

    Point get_exit();
};

Map::Map(int nivel) {
    std::string load_level = "../Dependencias/level" + std::to_string(nivel) + ".txt";
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
        block.draw();
    }
    for (auto& apple : apples) {
        apple.draw();
    }
    exit.draw();
}

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
    }

    file.close();
    return ubicaciones;
}

Point Map::get_spawn(){
    return spawn;
}

Point Map::get_exit(){
    return exit.get_position();
}

int Map::apple_quantity() {
    return apples.size();
}

int Map::level_number(){
    return this->lvl;
}

#endif