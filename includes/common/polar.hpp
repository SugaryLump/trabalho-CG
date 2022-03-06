#ifndef POLAR_HPP_
#define POLAR_HPP_

#include "common/shapes.hpp"

struct Polar {
    float radius;
    float alpha;
    float beta;
};

Vector3 polarToVector(Polar polar);

#endif  // POLAR_HPP_