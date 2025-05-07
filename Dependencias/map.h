#include "apple.h"
#include "block.h"
#include "point.h"
#include <vector>

#ifndef MAP_H
#define MAP_H

class Map {
  private:
    std::vector<Block> blocks;
    std::vector<Apple> apples;

  public:
    Map() {
        // Initialize the map with blocks and apples
        blocks.push_back(Block({0, 0}));
        blocks.push_back(Block({1, 0}));

        apples.push_back(Apple({1, 1}));
        apples.push_back(Apple({1, 2}));
    }

    bool is_block_in_point(Point p);

    bool is_apple_in_point(Point p);

    void remove_apple(Point p);

    void draw();
};

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
}

#endif