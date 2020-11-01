#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "headers-and-defines.hpp"
#include "camera.hpp"
#include "GL/glut.h"
#include "ground.hpp"
#include "maze.hpp"

// Global variables.
Camera topDown, firstPerson, thirdPerson, camera;
int global_argc, currentX, currentY;
char **global_argv;
bool depthEnabled = false, blendEnabled = false, lineSmoothEnabled = false,
     allDone = false, firstPerson1b = false, thirdPerson1b = false,
     topDown1b = true, canRotate = false, turnCone = false,
     textureEnabled = false, onX = true, onY = false, firstTime = false,
     collision1b = true;
Maze *maze;
float xSphere = 0.5, zSphere = 0.5, angle = 0, previousX, previousZ;
std::vector<std::pair<int, int>> points;

// Modul.
float absolute(float n) {
    if (n < 0)
        return -n;
    else
        return n;
}

// Func�ie blocant�.
void wait(clock_t seconds) {
    clock_t endwait;
    endwait = clock() + seconds * CLOCKS_PER_SEC;
    while (clock() < endwait) {
    }
}

bool collision(int direction, float distance) {
    std::pair<int, int> point;
    for (int i = 0; i < LABYRINTH_SIZE; i += 1) {
        for (int j = 0; j < LABYRINTH_SIZE; j += 1)
            if (maze->labyrinthMatrix[i][j] == false) {
                point.first = i;
                point.second = j;
                points.push_back(point);
            }
        point.first = -1;
        point.second = i;
        points.push_back(point);
        point.first = i;
        point.second = -1;
        points.push_back(point);
        point.first = LABYRINTH_SIZE;
        point.second = i;
        points.push_back(point);
        point.first = i;
        point.second = LABYRINTH_SIZE;
        points.push_back(point);
    }
    point.first = -1;
    point.second = -1;
    points.push_back(point);
    point.first = -1;
    point.second = LABYRINTH_SIZE;
    points.push_back(point);
    point.first = LABYRINTH_SIZE;
    point.second = -1;
    points.push_back(point);
    point.first = LABYRINTH_SIZE;
    point.second = LABYRINTH_SIZE;
    points.push_back(point);
    while (!points.empty()) {
        point = points.back();
        points.pop_back();
        int posX, posZ;
        if (direction == FORWARD) {
            posX = firstPerson.position.x + distance * firstPerson.forward.x;
            posZ = firstPerson.position.z + distance * firstPerson.forward.z;
        }
        if (direction == RIGHT) {
            posX = firstPerson.position.x + distance * firstPerson.right.x;
            posZ = firstPerson.position.z + distance * firstPerson.right.z;
        }
        if (point.first - 0.5 < posX && posX < point.first + 0.5 &&
            point.second - 0.5 < posZ && posZ < point.second + 0.5) {
            return false;
        }
    }
    return true;
}

void init(int argc, char **argv, int mode);

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    thirdPerson.position.x = firstPerson.position.x - firstPerson.forward.x;
    thirdPerson.position.y = firstPerson.position.y - firstPerson.forward.y + 1;
    thirdPerson.position.z = firstPerson.position.z - firstPerson.forward.z;
    thirdPerson.forward.x = firstPerson.forward.x;
    thirdPerson.forward.y = firstPerson.forward.y - 0.5;
    thirdPerson.forward.z = firstPerson.forward.z;
    thirdPerson.up.x = firstPerson.up.x;
    thirdPerson.up.y = firstPerson.up.y;
    thirdPerson.up.z = firstPerson.up.z;
    thirdPerson.right.x = firstPerson.right.x;
    thirdPerson.right.y = firstPerson.right.y;
    thirdPerson.right.z = firstPerson.right.z;
    camera.render();

    // Drawing ground.
    glColor3f(BLUE_COLOR);
    drawGround(LABYRINTH_SIZE, LABYRINTH_SIZE, -0.5f);

    maze->draw();

    // Torus portal.
    glPushMatrix();
    glColor3f(RED_COLOR);
    glTranslatef(LABYRINTH_SIZE - 0.5, 0,
                 LABYRINTH_SIZE - 1);  // put torus at end of labyrinth
    glutSolidTorus(0.5, 0.5, 2, 100);
    glPopMatrix();

    // Sphere character.
    if (!allDone) {
        glPushMatrix();
        xSphere = firstPerson.position.x;
        zSphere = firstPerson.position.z;
        glColor3f(GREEN_COLOR);
        glTranslatef(xSphere, 0.2,
                     zSphere);  // put sphere at beginning of labyrinth
        glutSolidSphere(0.2, 100, 10);
        for (int i = 0; i < 7; i += 1) {
            glTranslatef(0, -0.1, 0);
            glutSolidCube(0.1);
        }
        glColor3f(LIGHT_GREEN_COLOR);
        glRotatef(angle, 0, 1, 0);
        glutWireCone(0.05, 0.5, 100, 100);
        glPopMatrix();
    }

    if (xSphere > LABYRINTH_SIZE - 1 && zSphere > LABYRINTH_SIZE - 1) {
        allDone = true;
        glutPostRedisplay();
    }

    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, width / height, 0.1, 1000);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {

    case KEY_ESC:
        exit(0);
        break;

    case KEY_1:
        firstPerson1b = true;
        thirdPerson1b = false;
        topDown1b = false;
        break;

    case KEY_2:
        firstPerson1b = false;
        thirdPerson1b = true;
        topDown1b = false;
        break;

    case KEY_3:
        firstPerson1b = false;
        thirdPerson1b = false;
        topDown1b = true;
        break;

    case KEY_W:
    case KEY_W_CAPITAL:
        if (!allDone) {
            if (collision1b) {
                if (collision(FORWARD, 0.05f))
                    firstPerson.translateForward(0.05f);
            } else
                firstPerson.translateForward(0.05f);
        }
        break;

    case KEY_S:
    case KEY_S_CAPITAL:
        if (!allDone) {
            if (collision1b) {
                if (collision(FORWARD, -0.05f))
                    firstPerson.translateForward(-0.05f);
            } else
                firstPerson.translateForward(-0.05f);
        }
        break;

    case KEY_A:
    case KEY_A_CAPITAL:
        if (!allDone) {
            angle += 5;
            firstPerson.rotateFPS_OY(-5 * PI / 180);
        }
        break;

    case KEY_D:
    case KEY_D_CAPITAL:
        if (!allDone) {
            angle -= 5;
            firstPerson.rotateFPS_OY(5 * PI / 180);
        }
        break;

    case KEY_Q:
    case KEY_Q_CAPITAL:
        if (!allDone) {
            if (collision1b) {
                if (collision(RIGHT, -0.05f))
                    firstPerson.translateRight(-0.05f);
            } else
                firstPerson.translateRight(-0.05f);
        }
        break;

    case KEY_E:
    case KEY_E_CAPITAL:
        if (!allDone) {
            if (collision1b) {
                if (collision(RIGHT, 0.05f))
                    firstPerson.translateRight(0.05f);
            } else
                firstPerson.translateRight(0.05f);
        }
        break;

    default:
        break;
    }

    if (firstPerson1b)
        camera = firstPerson;
    if (thirdPerson1b)
        camera = thirdPerson;
    if (topDown1b)
        camera = topDown;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
        canRotate = true;

    // Update the saved mouse position.
    if (!firstTime) {
        currentX = x;
        currentY = y;
    }
}

void mouseMotion(int x, int y) {
    if (canRotate && !topDown1b) {
        if (onY) {
            if (!firstTime) {
                camera.rotateFPS_OX((currentY - y) * ROTATION_SPEED);
            }
            firstTime = false;
        }
        if (onX) {
            if (!firstTime) {
                camera.rotateFPS_OY(-(currentX - x) * ROTATION_SPEED);
            }
            firstTime = false;
        }
    }

    glutPostRedisplay();

    // Update the saved mouse position.
    if (!firstTime) {
        currentX = x;
        currentY = y;
    }
}

void menu(int value) {
    switch (value) {

    case MENU_ON_X:
        onX = true;
        onY = false;
        break;

    case MENU_ON_Y:
        onX = false;
        onY = true;
        break;

    case MENU_COLLISION:
        if (collision1b == true) {
            collision1b = false;
            std::cout << "Collision disabled." << std::endl;
        } else {
            collision1b = true;
            std::cout << "Collision enabled." << std::endl;
        }
        break;

    case MENU_DEPTH_TEST:
        if (depthEnabled) {
            glDisable(GL_DEPTH_TEST);
            depthEnabled = false;
            std::cout << "Depth test disabled." << std::endl;
        } else {
            glEnable(GL_DEPTH_TEST);
            depthEnabled = true;
            std::cout << "Depth test enabled." << std::endl;
        }
        break;

    case MENU_BLEND:
        if (blendEnabled) {
            glDisable(GL_BLEND);
            blendEnabled = false;
            std::cout << "Blend disabled." << std::endl;
        } else {
            glEnable(GL_BLEND);
            blendEnabled = true;
            std::cout << "Blend enabled." << std::endl;
        }
        break;

    case MENU_LINE_SMOOTH:
        if (lineSmoothEnabled) {
            glDisable(GL_LINE_SMOOTH);
            lineSmoothEnabled = false;
            std::cout << "Line smooth disabled." << std::endl;
        } else {
            glEnable(GL_LINE_SMOOTH);
            lineSmoothEnabled = true;
            std::cout << "Line smooth enabled." << std::endl;
        }
        break;

    case MENU_RESTART:
        xSphere = 0.5;
        zSphere = 0.5;
        maze = new Maze();
        maze->generate();
        break;

    case MENU_EXIT:
        exit(0);
        break;
    }

    firstTime = true;
    glutPostRedisplay();
}

void init(int argc, char **argv, int mode) {

    if (mode == ALL) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(800, 600);
        glutInitWindowPosition(100, 100);
        glutCreateWindow("Labyrinth");
    }

    // Callback functions.
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    glEnable(GL_DEPTH_TEST);
    depthEnabled = true;

    // Menu callback function.
    glutCreateMenu(menu);
    glutAddMenuEntry("Enable X-axis camera rotation", MENU_ON_X);
    glutAddMenuEntry("Enable Y-axis camera rotation", MENU_ON_Y);
    glutAddMenuEntry("Enable/Disable Collision", MENU_COLLISION);
    glutAddMenuEntry("Enable/Disable Depth Test", MENU_DEPTH_TEST);
    glutAddMenuEntry("Enable/Disable Blend", MENU_BLEND);
    glutAddMenuEntry("Enable/Disable Line Smooth", MENU_LINE_SMOOTH);
    glutAddMenuEntry("Restart", MENU_RESTART);
    glutAddMenuEntry("Exit", MENU_EXIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glClearColor(BACKGROUND_COLOR);

    firstPerson.translateForward(0.7);
    firstPerson.translateRight(0.5);

    topDown.position.x = LABYRINTH_SIZE / 2;
    topDown.position.y = LABYRINTH_SIZE / 2 + 2;
    topDown.position.z = LABYRINTH_SIZE / 2;
    topDown.forward.x = 0;
    topDown.forward.y = -1;
    topDown.forward.z = 0;
    topDown.up.x = 0;
    topDown.up.y = 0;
    topDown.up.z = 1;
    topDown.right.x = 0;
    topDown.right.y = 0;
    topDown.right.z = 1;

    maze = new Maze();
    maze->generate();

    camera = topDown;
}

int main(int argc, char **argv) {

    init(argc, argv, ALL);

    global_argc = argc;
    global_argv = argv;

    glutMainLoop();

    return 0;
}
