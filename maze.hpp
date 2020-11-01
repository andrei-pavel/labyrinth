#pragma once
#include <cstdlib>
#include <ctime>

#include "headers-and-defines.hpp"
#include "GL/glut.h"

class Maze {
public:
    Maze();
    ~Maze();

    void generate();
    void draw();

public:
    bool labyrinthMatrix[LABYRINTH_SIZE][LABYRINTH_SIZE];
};
