#include "shapes.h"

struct Vector3 {
    float x;
    float y;
    float z;
};

struct Triangle {
    Vector3 p1;
    Vector3 p2;
    Vector3 p3;
};

struct Model {
    Triangle triangles[];
};

Vector3 NewVector3(float x, float y, float z) {
    return {.x = x, .y = y, .z = z};
}

Vector3 NewVector3(float x, float z) {
    return {.x = x, .y = 0, .z = z};
}