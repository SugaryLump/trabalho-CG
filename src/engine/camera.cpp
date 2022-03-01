#include "engine/camera.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

namespace Camera {

static float radius = 5.0;

static float look_x = 0.0;
static float look_y = 0.0;
static float look_z = 0.0;

static float x = look_x + radius;
static float y = look_y + radius;
static float z = look_z + radius;

void place_camera() {
  // gluLookAt(x, y, z, x + look_x, y + look_y, z + look_z, 0.0f, 0.0f, 1.0f);

  gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0f, 0.0f, 1.0f);
}

} // namespace Camera
