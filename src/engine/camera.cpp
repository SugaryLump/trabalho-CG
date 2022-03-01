#include "engine/camera.hpp"
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

Camera::Camera() { posInitialCamera(); }

float Camera::getXPosition() { return positionX; }
float Camera::getYPosition() { return positionY; }
float Camera::getZPosition() { return positionZ; }
float Camera::getXLook() { return upX; }
float Camera::getYLook() { return upY; }
float Camera::getZLook() { return upZ; }
float Camera::getXUp() { return positionX + lookX; }
float Camera::getYUp() { return positionY + lookY; }
float Camera::getZUp() { return positionZ + lookZ; }

void Camera::posInitialCamera() {
  float radius = 10.0;

  lookX = 0.0;
  lookY = 0.0;
  lookZ = 0.0;

  positionX = lookX + radius;
  positionY = lookY + radius;
  positionZ = lookZ + radius;

  upX = 0.0f;
  upY = 0.0f;
  upZ = 1.0f;
}
