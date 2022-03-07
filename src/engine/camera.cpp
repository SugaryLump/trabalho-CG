#include "engine/camera.hpp"
#include "engine/input.hpp"
#include <iostream>

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

    if (beta > M_PI_2) {
        beta = M_PI_2 - 0.01;
    } else if (beta < -M_PI_2) {
        beta = -M_PI_2 + 0.01;
    }
}

void Camera::posInitialCamera() {
    currentType = FOLLOW;

    radius = 10.0;
    alpha = 0;
    beta = 0;

    look.x = 0.0;
    look.y = 0.0;
    look.z = 0.0;

    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    position.x = 0;
    position.z = 10;
    position.y = 0;
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


void Camera::update(InputState* input) {
    if (input->keyTapped('f')) {
        setNextTypeCamera();
    }

    switch(currentType) {
        case FOLLOW:
            updateFollow(input);
            break;
        case FPS:
            updateFPS(input);
            break;
        default:
            break;
    }
}
/*** FOLLOW *****/

void Camera::updateFollow(InputState* input) {
    alpha += 0.05 * (input->specialKeyAxisDirection(GLUT_KEY_LEFT, GLUT_KEY_RIGHT));
    beta += 0.05 * (input->specialKeyAxisDirection(GLUT_KEY_DOWN, GLUT_KEY_UP));
    normalizeAlphaBeta();
    radius += 0.05 * (input->specialKeyAxisDirection(GLUT_KEY_F1, GLUT_KEY_F2));

    position.x = look.x + radius * cos(beta) * sin(alpha);
    position.z = look.z + radius * cos(beta) * cos(alpha);
    position.y = look.y + radius * sin(beta);
}

// FPS
void Camera::updateFPS(InputState* input) {
    // Mouse Handler
    alpha -= input->getMouseDeltaX() * 0.005;
    beta += input->getMouseDeltaY() * 0.005;
    normalizeAlphaBeta();
    Vector3 tmp = sphericalToCartesian(alpha + M_PI, -beta, 5);
    look.x = tmp.x + position.x;
    look.y = tmp.y + position.y;
    look.z = tmp.z + position.z;

    // Keyboard Handler
    int vMoveInput = input->keyAxisDirection('s', 'w');
    int hMoveInput = input->keyAxisDirection('a', 'd');
    
    Vector3 fVector = sphericalToCartesian(alpha + M_PI, -beta, 0.075);
    Vector3 sVector = sphericalToCartesian(alpha + M_PI_2, 0, 0.075);
    float vertical = 0.05 * input->keyAxisDirection('c', ' ');
    position.x += fVector.x * vMoveInput + sVector.x * hMoveInput;
    position.y += fVector.y * vMoveInput + sVector.y * hMoveInput + vertical;
    position.z += fVector.z * vMoveInput + sVector.z * hMoveInput;
    look.x += fVector.x * vMoveInput + sVector.x * hMoveInput;
    look.y += fVector.y * vMoveInput + sVector.y * hMoveInput;
    look.z += fVector.z * vMoveInput + sVector.z * hMoveInput;
}