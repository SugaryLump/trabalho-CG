#include "engine/camera.hpp"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

Camera::Camera() {
    posInitialCamera();
}

void Camera::posInitialCamera() {
    currentType = FOLLOW;

    radius = 10.0;
    alpha = 0;
    beta = M_PI / 100;

    lookX = 0.0;
    lookY = 0.0;
    lookZ = 0.0;

    upX = 0.0f;
    upY = 1.0f;
    upZ = 0.0f;

    updateFollow();
}

void Camera::changeTypeCamera(CameraType newType) {
    currentType = newType;
}

void Camera::setFollowPoint(Vector3 point) {
    lookX = point.x;
    lookY = point.y;
    lookZ = point.z;
}

void Camera::setNextTypeCamera() {
    switch (currentType) {
        case FOLLOW:
            currentType = FREE;
            break;
        case FREE:
            currentType = FOLLOW;
            break;
        default:
            currentType = FOLLOW;
            break;
    }
}

/*** FOLLOW *****/

void Camera::updateFollow() {
    positionX = lookX + radius * cos(beta) * sin(alpha);
    positionZ = lookZ + radius * cos(beta) * cos(alpha);
    positionY = lookY + radius * sin(beta);
}

void Camera::followMoveUp() {
    if (beta + 0.1 < M_PI / 2) beta += 0.1;
}

void Camera::followMoveDown() {
    if (beta - 0.1 > -M_PI / 2) beta -= 0.1;
}

void Camera::followMoveLefth() {
    alpha += 0.1;
}

void Camera::followMoveRight() {
    alpha -= 0.1;
}

void Camera::followMoveIn() {
    radius -= 0.5;
}

void Camera::followMoveOut() {
    radius += 0.5;
}

void Camera::followProcessSpecialKeys(int key, int _x, int _y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            followMoveLefth();
            break;
        case GLUT_KEY_RIGHT:
            followMoveRight();
            break;
        case GLUT_KEY_UP:
            followMoveUp();
            break;
        case GLUT_KEY_DOWN:
            followMoveDown();
            break;
        case GLUT_KEY_F1:
            followMoveIn();
            break;
        case GLUT_KEY_F2:
            followMoveOut();
            break;
    }

    updateFollow();
}