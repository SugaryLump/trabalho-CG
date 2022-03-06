#include "engine/camera.hpp"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void Camera::posInitialCamera() {
    currentType = FOLLOW;

    radius = 10.0;
    alpha = 0;
    beta = M_PI / 100;

    look.x = 0.0;
    look.y = 0.0;
    look.z = 0.0;

    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    updateFollow();
}

void Camera::changeTypeCamera(CameraType newType) {
    currentType = newType;
}

void Camera::setFollowPoint(Vector3 point) {
    look.x = point.x;
    look.y = point.y;
    look.z = point.z;
}

void Camera::setNextTypeCamera() {
    switch (currentType) {
        case FOLLOW:
            currentType = FPS;
            break;
        case FPS:
            currentType = FOLLOW;
            posInitialCamera();
            break;
        default:
            currentType = FOLLOW;
            break;
    }
}

/*** FOLLOW *****/

void Camera::updateFollow() {
    position.x = look.x + radius * cos(beta) * sin(alpha);
    position.z = look.z + radius * cos(beta) * cos(alpha);
    position.y = look.y + radius * sin(beta);
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
    if ((radius - 0.5) >= near) radius -= 0.5;
}

void Camera::followMoveOut() {
    if ((radius + 0.5) <= far) radius += 0.5;
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

void Camera::normalizeAlphaBeta() {
    if (alpha > 2 * M_PI) {
        alpha -= 2 * M_PI;
    } else if (alpha < 0) {
        alpha += 2 * M_PI;
    }

    if (beta > M_PI_2) {
        beta = M_PI_2 - 0.01;
    } else if (beta < -M_PI_2) {
        beta = -M_PI_2 + 0.01;
    }
}

// FPS
void Camera::fpsProcessSpecialKeys(int key, int _x, int _y) {
    short hMoveInput = keysState['d'] - keysState['a'];
    short vMoveInput = keysState['w'] - keysState['s'];
    short extraMoveInput = keysState[' '] - keysState['c'];

    // Mouse Handler
    alpha -= mouseDeltaX * 0.005;
    beta += mouseDeltaY * 0.005;
    normalizeAlphaBeta();
    Vector3 tmp = sphericalToCartesian(alpha + M_PI, -beta, 5);
    look.x = tmp.x + position.x;
    look.y = tmp.y + position.y;
    look.z = tmp.z + position.z;

    // Keyboard Handler
    Vector3 fVector = sphericalToCartesian(alpha + M_PI, -beta, 0.075);
    Vector3 sVector = sphericalToCartesian(alpha + M_PI_2, 0, 0.075);
    float vertical = 0.05 * extraMoveInput;
    position.x += fVector.x * vMoveInput + sVector.x * hMoveInput;
    position.y += fVector.y * vMoveInput + sVector.y * hMoveInput + vertical;
    position.z += fVector.z * vMoveInput + sVector.z * hMoveInput;
    look.x += fVector.x * vMoveInput + sVector.x * hMoveInput;
    look.y += fVector.y * vMoveInput + sVector.y * hMoveInput;
    look.z += fVector.z * vMoveInput + sVector.z * hMoveInput;

    for (int i = 0; i < 256; i++) { previousKeysState[i] = keysState[i]; }
    mouseDeltaX = 0;
    mouseDeltaY = 0;
}