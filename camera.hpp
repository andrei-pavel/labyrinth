#pragma once
#include "vector-3d.hpp"

class Camera {
public:
    Vector3D forward;
    Vector3D up;
    Vector3D right;
    Vector3D position;

    Camera();
    ~Camera();

    void render();

    void translateForward(float distance);
    void translateUp(float distance);
    void translateRight(float distance);

    void rotateFPS_OY(float angle);
    void rotateFPS_OX(float angle);
    void rotateFPS_OZ(float angle);
    void rotateTPS_OY(float angle, float distanceToInterest);
    void rotateTPS_OX(float angle, float distanceToInterest);
    void rotateTPS_OZ(float angle, float distanceToInterest);
};
