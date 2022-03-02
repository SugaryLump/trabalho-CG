#include "common/shapes.hpp"
#include <vector>
#include <tuple>

using namespace std;

Model newPlane(float length, int subdivisions) {
  Model plane = Model();
  float triangleSide = length / subdivisions;
  float start = -length / 2;

  for (int x = 0; x <= subdivisions; x++) {
    for (int z = 0; z <= subdivisions; z++) {
      float xpos = start + (x * triangleSide);
      float zpos = start + (z * triangleSide);
      plane.addVertex(Vector3(xpos, zpos));
    }
  }

  for (int x = 0; x < subdivisions; x++) {
    for (int z = 0; z < subdivisions; z++) {
      int topleft = x * (subdivisions + 1) + z;
      int topright = topleft + subdivisions + 1;
      int botleft = topleft + 1;
      int botright = topright + 1;
      plane.addFace(topleft, botleft, botright);
      plane.addFace(topright, topleft, botright);
    }
  }


  return plane;
}
