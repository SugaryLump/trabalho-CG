#include "common/shapes.hpp"
#include <vector>
#include <tuple>
#include <tgmath.h>

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

Model newBox(float length, int subdivisions) {
  Model box = Model();
  float triangleSide = length / subdivisions;
  
  //Plane vertexes
  float start = -length / 2;
  for (int x = 0; x <= subdivisions; x++) {
    for (int y = 0; y <= subdivisions; y++) {
      for (int z = 0; z <= subdivisions; z++) {
        if (z == 0 || x == 0 || y == 0 || z == subdivisions || x == subdivisions || y == subdivisions) {
          box.addVertex(Vector3(start + (x * triangleSide), start+ ( y * triangleSide), start + (z * triangleSide)));
        }
        else {
          //Skip to right before the next plane
          z += subdivisions - 2;
        }
      }  
    }
  }

  for (int x = 0; x < subdivisions; x++) {
    for (int y = 0; y < subdivisions; y++) {
      for (int z = 0; z < subdivisions; z++) {
        //Back plane
        //v2--v1
        //| / |
        //v3--v4
        // v3 has coordinates (x,y,z)
        if (z == 0) {
          int v1 = calcBoxVertexIndex(x + 1, y + 1, z, subdivisions);
          int v2 = calcBoxVertexIndex(x, y + 1, z, subdivisions);
          int v3 = calcBoxVertexIndex(x, y, z, subdivisions);
          int v4 = calcBoxVertexIndex(x + 1, y, z, subdivisions);
          box.addFace(v3, v2, v1);
          box.addFace(v3, v1, v4);
        }

        //Front plane
        //v2--v1
        //| / |
        //v3--v4
        // v3 has coordinates (x,y,z)
        if (z == subdivisions - 1) {
          int v1 = calcBoxVertexIndex(x + 1, y + 1, z, subdivisions);
          int v2 = calcBoxVertexIndex(x, y + 1, z, subdivisions);
          int v3 = calcBoxVertexIndex(x, y, z, subdivisions);
          int v4 = calcBoxVertexIndex(x + 1, y, z, subdivisions);
          box.addFace(v3, v1, v2);
          box.addFace(v3, v4, v1);
        }

        //Left plane
        //v2-_
        //| / v1
        //v3-_|
        //    v4
        // v3 has coordinates (x,y,z)
        if (x == 0) {
          int v1 = calcBoxVertexIndex(x, y + 1, z + 1, subdivisions);
          int v2 = calcBoxVertexIndex(x, y + 1, z, subdivisions);
          int v3 = calcBoxVertexIndex(x, y, z, subdivisions);
          int v4 = calcBoxVertexIndex(x, y, z + 1, subdivisions);
          box.addFace(v3, v1, v2);
          box.addFace(v3, v4, v1);
        }

        //Right plane
        //v2-_
        //| / v1
        //v3-_|
        //    v4
        // v3 has coordinates (x,y,z)
        if (x == subdivisions - 1) {
          int v1 = calcBoxVertexIndex(x, y + 1, z + 1, subdivisions);
          int v2 = calcBoxVertexIndex(x, y + 1, z, subdivisions);
          int v3 = calcBoxVertexIndex(x, y, z, subdivisions);
          int v4 = calcBoxVertexIndex(x, y, z + 1, subdivisions);
          box.addFace(v3, v2, v1);
          box.addFace(v3, v1, v4);
        }

        //Bottom plane
        //v3--v2
        //| \ |
        //v4--v1
        // v3 has coordinates (x,y,z)
        if (y == 0) {
          int v1 = calcBoxVertexIndex(x + 1, y, z + 1, subdivisions);
          int v2 = calcBoxVertexIndex(x + 1, y, z, subdivisions);
          int v3 = calcBoxVertexIndex(x, y, z, subdivisions);
          int v4 = calcBoxVertexIndex(x, y, z + 1, subdivisions);
          box.addFace(v3, v1, v4);
          box.addFace(v3, v2, v1);
        }

        //Top plane
        //v3--v2
        //| \ |
        //v4--v1
        // v3 has coordinates (x,y,z)
        if (y == subdivisions - 1) {
          int v1 = calcBoxVertexIndex(x + 1, y, z + 1, subdivisions);
          int v2 = calcBoxVertexIndex(x + 1, y, z, subdivisions);
          int v3 = calcBoxVertexIndex(x, y, z, subdivisions);
          int v4 = calcBoxVertexIndex(x, y, z + 1, subdivisions);
          box.addFace(v3, v4, v1);
          box.addFace(v3, v1, v2);
        }
      }
    }
  }

  return box;
}

int calcBoxInnerVertexes(int x, int y, int z, int subdivisions) {
  int total = 0;
  if (x > 1) {
    total += (x - 1) * pow(subdivisions - 1, 2);
  }
  if (x > 0 && x < subdivisions && y > 1) {
    total += (y - 1) * (subdivisions - 1);
  }
  if (x > 0 && x < subdivisions && y > 0 && y < subdivisions && z > 1) {
    total += z - 1;
  }
  return total;
}


int calcBoxVertexIndex(int x, int y, int z, int subdivisions) {
  return z + (y * (subdivisions + 1)) + (x * pow(subdivisions + 1, 2)) - calcBoxInnerVertexes(x, y, z, subdivisions);
}