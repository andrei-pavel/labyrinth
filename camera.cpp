#pragma once
#include "camera.hpp"

Camera::Camera() {
    position = Vector3D(0, 0, 0);
    forward = Vector3D(0, 0, 1);
    up = Vector3D(0, 1, 0);
    right = Vector3D(1, 0, 0);
}

Camera::~Camera() {
}

void Camera::render() {
    Vector3D center = position + forward;
    gluLookAt(position.x, position.y, position.z, center.x, center.y, center.z,
              up.x, up.y, up.z);
}

void Camera::translateForward(float distance) {
    position += forward * distance;
}

void Camera::translateUp(float distance) {
    position += up * distance;
}

void Camera::translateRight(float distance) {
    position += right * distance;
}

void Camera::rotateFPS_OX(float angle) {
    Vector3D newForward = Vector3D(forward.x, forward.y, forward.z);
    newForward = forward * cos(angle) + up * sin(angle);
    up = forward.CrossProduct(right);
    forward = newForward;
}

void Camera::rotateFPS_OY(float angle) {
    Vector3D newForward = Vector3D(forward.x, forward.y, forward.z);
    newForward = forward * cos(angle) + right * sin(angle);
    right = forward.CrossProduct(up);
    forward = newForward;
}

void Camera::rotateFPS_OZ(float angle) {
    Vector3D newUp = up * cos(angle) + right * sin(angle);
    right = up.CrossProduct(forward);
    up = newUp;
}

void Camera::rotateTPS_OX(float angle, float distanceToInterest) {
    Vector3D newForward = forward * cos(angle) + up * sin(angle);
    up = forward.CrossProduct(right);
    position += forward * distanceToInterest;
    position -= newForward * distanceToInterest;
    forward = newForward;
    forward.Normalize();
}

void Camera::rotateTPS_OY(float angle, float distanceToInterest) {
    Vector3D newForward = forward * cos(angle) + right * sin(angle);
    right = forward.CrossProduct(up);
    position += forward * distanceToInterest;
    position -= newForward * distanceToInterest;
    forward = newForward;
    forward.Normalize();
}

void Camera::rotateTPS_OZ(float angle, float distanceToInterest) {
    Vector3D newUp = up * cos(angle) + right * sin(angle);
    right = forward.CrossProduct(up);
    position += up * distanceToInterest;
    position -= newUp * distanceToInterest;
    up = newUp;
    up.Normalize();
}
