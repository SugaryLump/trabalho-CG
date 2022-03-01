#include "common/shapes.h"
#include <stdlib.h>

struct Vector3 {
  float x;
  float y;
  float z;
};

struct Triangle {
  Vector3 *p1;
  Vector3 *p2;
  Vector3 *p3;
};

struct Model {
  Triangle **triangles;
};

Vector3 *newVector3(float x, float y, float z) {
  Vector3 *v = (Vector3 *)malloc(sizeof(Vector3));
  v->x = x;
  v->y = y;
  v->z = z;
  return v;
}

Vector3 *newVector3(float x, float z) {
  Vector3 *v = (Vector3 *)malloc(sizeof(Vector3));
  v->x = x;
  v->y = 0;
  v->z = z;
  return v;
}

Triangle *newTriangle(Vector3 *p1, Vector3 *p2, Vector3 *p3) {
  Triangle *t = (Triangle *)malloc(sizeof(Triangle));
  t->p1 = p1;
  t->p2 = p2;
  t->p3 = p3;
  return t;
}

Model *newModel(int triangles) {
  Model *m = (Model *)malloc(sizeof(Model));
  m->triangles = (Triangle **)malloc(sizeof(Triangle *) * triangles);
  return m;
}

Model *newModel(Triangle **triangles) {
  Model *m = (Model *)malloc(sizeof(Model));
  m->triangles = triangles;
  return m;
}

Model *newPlane(float length, int subdivisions) {
  Model *plane = newModel(subdivisions * subdivisions);
  float triangleSide = length / subdivisions;
  float start = -length / 2;
  int t = 0;

  for (int x = 0; x < subdivisions; x++) {
    for (int z = 0; z < subdivisions; z++) {
      float xLength = triangleSide * x;
      float zLength = triangleSide * z;
      float xMin, xMax, zMin, zMax;
      xMin = start + xLength;
      xMax = start + xLength + triangleSide;
      zMin = start + zLength;
      zMax = start + zLength + triangleSide;

      Vector3 *v1 = newVector3(xMax, zMin);
      Vector3 *v2 = newVector3(xMin, zMin);
      Vector3 *v3 = newVector3(xMin, zMax);
      Vector3 *v4 = newVector3(xMax, zMax);

      plane->triangles[t] = newTriangle(v1, v2, v4);
      plane->triangles[t + 1] = newTriangle(v2, v3, v4);
      t += 2;
    }
  }

  return plane;
}
