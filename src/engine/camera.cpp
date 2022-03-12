#include <cstdlib>
#include "engine/camera.hpp"
#include "common/geometry.hpp"

#include <iostream>

#include "engine/input.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void Camera::normalizeAlphaBeta() {
    if (alpha > 2 * M_PI) {
        alpha -= 2 * M_PI;
    } else if (alpha < 0) {
        alpha += 2 * M_PI;
    }

    if (beta > M_PI_2 - 0.01) {
        beta = M_PI_2 - 0.01;
    } else if (beta < -M_PI_2 + 0.01) {
        beta = -M_PI_2 + 0.01;
    }
}

Camera::Camera() { posInitialCamera(); }

Camera::Camera(float near, float far, float fov) {
    posInitialCamera();
    near = near;
    far = far;
    fov = fov;
}

Camera::Camera(Vector3 newLook, Vector3 newUp, Vector3 newPosition, float newNear, float newFar, float newFov) {
    currentType = FOLLOW;

    radius = 10.0;
    alpha = 0;
    beta = 0;

    look = newLook;
    up = newUp;
    position = newPosition;

    near = newNear;
    far = newFar;
    fov = newFov;
}

void Camera::posInitialCamera() {
    currentType = FOLLOW;

    radius = 10.0;
    alpha = 0;
    beta = 0;

    look = Vector3(0.0, 0.0, 0.0);
    up = Vector3(0.0f, 1.0f, 0.0f);
    position = Vector3(0.0, 10.0, 0.0);

    near = 1;
    far = 1000;
    fov = 90;
}

float Camera::getNear() {
    return near;
}

float Camera::getFar() {
    return far;
}

float Camera::getFov() {
    return fov;
}

void Camera::changeTypeCamera(CameraType newType) {
    currentType = newType;
}

void Camera::setFollowPoint(Vector3 point) {
    look = point;
}

void Camera::setup(Vector3 newLook, Vector3 newUp, Vector3 newPosition) {
    look = newLook;
    up = newUp;
    position = newPosition;
}

void Camera::setNextTypeCamera() {
    switch (currentType) {
        case FOLLOW:
            currentType = FPS;
            break;
        case FPS:
            currentType = FOLLOW;
            // posInitialCamera();
            break;
        default:
            currentType = FOLLOW;
            break;
    }
}

void Camera::update(InputState* input, float rateModifier) {
    if (input->keyTapped('f')) { setNextTypeCamera(); }

    switch (currentType) {
        case FOLLOW:
            updateFollow(input, rateModifier);
            break;
        case FPS:
            updateFPS(input, rateModifier);
            break;
        default:
            break;
    }
}
/*** FOLLOW *****/

void Camera::updateFollow(InputState* input, float rateModifier) {
    alpha += 0.03 * rateModifier * (input->specialKeyAxisDirection(GLUT_KEY_LEFT, GLUT_KEY_RIGHT));
    beta += 0.03 * rateModifier * (input->specialKeyAxisDirection(GLUT_KEY_DOWN, GLUT_KEY_UP));
    normalizeAlphaBeta();
    radius += 0.05 * rateModifier * (input->specialKeyAxisDirection(GLUT_KEY_F1, GLUT_KEY_F2));

    position.x = look.x + radius * cos(beta) * sin(alpha);
    position.z = look.z + radius * cos(beta) * cos(alpha);
    position.y = look.y + radius * sin(beta);
}

// FPS
void Camera::updateFPS(InputState* input, float rateModifier) {
    // Mouse Handler
    alpha -= input->getMouseDeltaX() * 0.003;
    beta += input->getMouseDeltaY() * 0.003;
    normalizeAlphaBeta();
    Vector3 tmp = Vector3::fromSpherical(alpha + M_PI, -beta, radius);
    look.x = tmp.x + position.x;
    look.y = tmp.y + position.y;
    look.z = tmp.z + position.z;

    // Keyboard Handler
    int vMoveInput = input->keyAxisDirection('s', 'w');
    int hMoveInput = input->keyAxisDirection('a', 'd');

    Vector3 fVector = Vector3::fromSpherical(alpha + M_PI, -beta, 0.075);
    Vector3 sVector = Vector3::fromSpherical(alpha + M_PI_2, 0, 0.075);
    float vertical = 0.05 * input->keyAxisDirection('c', ' ');
    position.x += (fVector.x * vMoveInput + sVector.x * hMoveInput) * rateModifier;
    position.y += (fVector.y * vMoveInput + sVector.y * hMoveInput + vertical) * rateModifier;
    position.z += (fVector.z * vMoveInput + sVector.z * hMoveInput) * rateModifier;
    look.x += (fVector.x * vMoveInput + sVector.x * hMoveInput) * rateModifier;
    look.y += (fVector.y * vMoveInput + sVector.y * hMoveInput) * rateModifier;
    look.z += (fVector.z * vMoveInput + sVector.z * hMoveInput) * rateModifier;
}