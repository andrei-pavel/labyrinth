#include "maze.hpp"
#include "headers-and-defines.hpp"

unsigned long mix(unsigned long a, unsigned long b, unsigned long c) {
    a = a - b;
    a = a - c;
    a = a ^ (c >> 13);
    b = b - c;
    b = b - a;
    b = b ^ (a << 8);
    c = c - a;
    c = c - b;
    c = c ^ (b >> 13);
    a = a - b;
    a = a - c;
    a = a ^ (c >> 12);
    b = b - c;
    b = b - a;
    b = b ^ (a << 16);
    c = c - a;
    c = c - b;
    c = c ^ (b >> 5);
    a = a - b;
    a = a - c;
    a = a ^ (c >> 3);
    b = b - c;
    b = b - a;
    b = b ^ (a << 10);
    c = c - a;
    c = c - b;
    c = c ^ (b >> 15);
    return c;
}

Maze::Maze() {
    for (int i = 0; i < LABYRINTH_SIZE; i += 1)
        for (int j = 0; j < LABYRINTH_SIZE; j += 1) {
            labyrinthMatrix[i][j] = false;
        }
}

Maze::~Maze() {
}

void Maze::generate() {
    // Start at cell ( 0, 0 ).
    int i = 0, j = 0, state = RIGHT_DOWN;
    srand(time(NULL));
    // Get neighbour right x% or neighbour bottom ( 100 - x )%
    while (i < LABYRINTH_SIZE - 1) {
        labyrinthMatrix[i][j] = true;
        if (rand() % 100 < PERCENTAGE) {
            if (state == RIGHT_DOWN)
                j += 1;  // Right.
            else
                j -= 1;
        } else
            i += 1;  // Down.
        if (j == 0)
            state = RIGHT_DOWN;
        if (j == LABYRINTH_SIZE - 1)
            state = LEFT_DOWN;
    }
    for (int k = j; k < LABYRINTH_SIZE; k += 1) {
        labyrinthMatrix[LABYRINTH_SIZE - 1][k] = true;
    }
    for (int i = 0; i < LABYRINTH_SIZE; i += 5) {
        for (int j = 0; j < LABYRINTH_SIZE - 1; j += 1) {
            if (!labyrinthMatrix[i][j + 1])
                labyrinthMatrix[i][j] = true;
        }
    }
    for (int i = 0; i < LABYRINTH_SIZE - 1; i += 1) {
        for (int j = 0; j < LABYRINTH_SIZE; j += 5) {
            if (!labyrinthMatrix[i + 1][j])
                labyrinthMatrix[i][j] = true;
        }
    }
    for (int i = 0; i < LABYRINTH_SIZE - 3; i += 1) {
        if (!labyrinthMatrix[i][1] && !labyrinthMatrix[i + 1][1] &&
            !labyrinthMatrix[i + 2][1])
            for (int j = 0; j < LABYRINTH_SIZE; j += 1)
                labyrinthMatrix[i + 1][j] = true;
    }
}

void Maze::draw() {
    glPushMatrix();
    glColor3f(LIGHT_BLUE_COLOR);
    glPushMatrix();
    glTranslatef(-0.5, 0, -0.5);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.5, 0, LABYRINTH_SIZE + 0.5);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(LABYRINTH_SIZE + 0.5, 0, -0.5);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(LABYRINTH_SIZE + 0.5, 0, LABYRINTH_SIZE + 0.5);
    glutSolidCube(1);
    glPopMatrix();
    for (int i = -1; i < LABYRINTH_SIZE + 1; i += 1) {
        for (int j = -1; j < LABYRINTH_SIZE + 1; j += 1) {
            int di = LABYRINTH_SIZE / 2 - i;
            int dj = (LABYRINTH_SIZE - 1) / 2 - j;
            if (i == -1 || j == -1 || i == LABYRINTH_SIZE ||
                j == LABYRINTH_SIZE) {
                glPushMatrix();
                glTranslatef(i + 0.5, 0, j + 0.5);
                glutSolidCube(1);
                glPopMatrix();
            } else if (!labyrinthMatrix[i][j]) {
                glPushMatrix();
                glTranslatef(i + 0.5, 0, j + 0.5);
                glutSolidCube(1);
                glPopMatrix();
            }
        }
    }
    glPopMatrix();
}
