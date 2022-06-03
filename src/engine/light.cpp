#include "engine/light.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


PointLight::PointLight(Vector3 position) {
    this->position = position;
}

DirectionalLight::DirectionalLight(Vector3 direction) {
    this->direction = direction;
}

Spotlight::Spotlight(Vector3 position, Vector3 direction, float cutoff) {
    this->position = position;
    this->direction = direction;
    this->cutoff = cutoff;
}

void PointLight::setupLight(int index) {
    float param[4];
    param[0] = position.x;
    param[1] = position.y;
    param[2] = position.z;
    param[3] = 0.0f;
    glLightfv(0x4000 + 0x0001 * index, GL_POSITION, param);
}

void DirectionalLight::setupLight(int index) {
    float param[3];
    param[0] = direction.x;
    param[1] = direction.y;
    param[2] = direction.z;
    glLightfv(0x4000 + 0x0001 * index, GL_SPOT_DIRECTION, param);
}

void Spotlight::setupLight(int index) {
    float pos[4];
    pos[0] = position.x;
    pos[1] = position.y;
    pos[2] = position.z;
    pos[3] = 0.0f;
    glLightfv(0x4000 + 0x0001 * index, GL_POSITION, pos);
    float dir[3];
    dir[0] = direction.x;
    dir[1] = direction.y;
    dir[2] = direction.z;
    glLightfv(0x4000 + 0x0001 * index, GL_SPOT_DIRECTION, dir);
}