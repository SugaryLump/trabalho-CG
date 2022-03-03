#include "common/shapes.hpp"
#include <vector>
#include <tuple>
#include <tgmath.h>

using namespace std;

Vector3 sphericalToCartesian(float alpha, float beta, float radius) {
  float x = radius * cos(beta) * sin(alpha);
  float y = radius * sin(beta);
  float z = radius * cos(beta) * cos(alpha);
  return Vector3(x, y, z);
}

Vector3 polarToCartesian(float alpha, float radius, float y) {
  float x = radius * sin(alpha);
  float z = radius * cos(alpha);
  return Vector3(x, y, z);
}

//length and subdivisions must be bigger than 0
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

//length and subdivisions must be bigger than 0
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

//radius must be bigger than 0
//slices must be bigger than 2
//stacks must be bigger than 1
Model newSphere(float radius, int slices, int stacks) {
  Model sphere = Model();
  float beta = -M_PI/2;

  //Vertexes
  for (int st = 0; st <= stacks; st++) {
    if (st == 0) {
      sphere.addVertex(sphericalToCartesian(0, beta, radius));
    }
    else {
      float alpha = 0;
      for (int sl = 0; sl < slices; sl++) {
        sphere.addVertex(sphericalToCartesian(alpha, beta, radius));
        alpha += (2 * M_PI) / slices;
      }
    }

    beta += M_PI / (stacks - 1);
  }

  //Faces
  for (int st = 0; st < stacks; st++) {
    for (int sl = 0; sl < slices; sl++) {
      //Bottom stack
      if (st == stacks - 1) {
        if (sl < slices - 1) {
          sphere.addFace(st * slices + 1, (st - 1) * slices + sl + 2, (st - 1) * slices + sl + 1);
        }
        else {
          sphere.addFace(st * slices + 1, (st - 1) * slices + 1, (st - 1) * slices + sl + 1);
        }
      }
      //Top stack
      else if (st == 0) {
        if (sl < slices - 1) {
          sphere.addFace(0, sl + 1, sl + 2);
        }
        else {
          sphere.addFace(0, slices, 1);
        }
      }
      //Middle stacks
      else {
        if (sl < slices - 1) {
          int v1 = (st - 1) * slices + 1 + sl;
          int v2 = st * slices + 1 + sl;
          int v3 = st * slices + 2 + sl;
          int v4 = (st - 1) * slices + 2 + sl;
          sphere.addFace(v1, v2, v3);
          sphere.addFace(v1, v3, v4);
        }
        else {
          int v1 = (st - 1) * slices + 1 + sl;
          int v2 = st * slices + 1 + sl;
          int v3 = st * slices + 1;
          int v4 = (st - 1) * slices + 1;
          sphere.addFace(v1, v2, v3);
          sphere.addFace(v1, v3, v4);
        }
      }
    }
  }

  return sphere;
}

//radius, height and stacks must be bigger than 0
//slices must be bigger than 2
Model newCone(float radius, float height, float slices, float stacks) {
  Model cone = Model();

  //Vertexes
  cone.addVertex(Vector3(0, 0, 0));
  cone.addVertex(Vector3(0, 0, height));
  for (int st = 0; st < stacks; st++) {
    float alpha = 0;
    for (int sl = 0; sl < slices; sl++) {
      cone.addVertex(polarToCartesian(alpha, radius - (radius / stacks * st), (height / stacks) * st));
    }
  }

  //Faces
  for (int st = 0; st <= stacks; st++) {
    for (int sl = 0; sl < slices; sl++) {
      //Base
      if (st == 0) {
        if (sl < slices - 1) {
          cone.addFace(0, sl + 3, sl + 2);
        }
        else {
          cone.addFace(0, 2, sl + 2);
        }
      }
      //Top stack
      else if (st == stacks) {
        if (sl < slices - 1) {
          cone.addFace(1, (st - 1) * slices + sl + 2, (st - 1) * slices + sl + 3);
        }
        else {
          cone.addFace(1, (st - 1) * slices + sl + 2, (st - 1) * slices + 2);
        }
      }
      //Middle stacks
      else {
        if (sl < slices - 1) {
          int v1 = (st - 1) * slices + sl + 2;
          int v2 = (st - 1) * slices + sl + 3;
          int v3 = st * slices + sl;
          int v4 = st * slices + sl + 3;
          cone.addFace(v1, v2, v3);
          cone.addFace(v1, v2, v4);
        }
        else {
          int v1 = (st - 1) * slices + sl + 2;
          int v2 = (st - 1) * slices + 2;
          int v3 = st * slices + sl;
          int v4 = st * slices + 2;
          cone.addFace(v1, v2, v3);
          cone.addFace(v1, v2, v4);
        }
      }
    }
  }

  return cone;
}