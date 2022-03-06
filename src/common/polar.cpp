#include "common/polar.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

Vector3 polarToVector(Polar polar) {
    const float x_ = polar.radius * cos(polar.beta) * sin(polar.alpha);
    const float y_ = polar.radius * sin(polar.beta);
    const float z_ = polar.radius * cos(polar.beta) * cos(polar.alpha);
    return Vector3{x_, y_, z_};
}