#pragma once

#include <vector>
#include <tuple>

struct Vector3 {
  float x;
  float y;
  float z;

  Vector3() {}

  Vector3(float newx, float newy, float newz) {
    x = newx;
    y = newy;
    z = newz;
  }

  Vector3(float newx, float newz) {
    x = newx;
    y = 0;
    z = newz;
  }
};

struct Triangle {
  Vector3 p1;
  Vector3 p2;
  Vector3 p3;

  Triangle(Vector3 v1, Vector3 v2, Vector3 v3) {
    p1 = v1;
    p2 = v2;
    p3 = v3;
  }
};

struct Model {
  std::vector<Vector3> vertexes;
  std::vector<std::tuple<int,int,int>> faces;

  Model() {}

  void addVertex(Vector3 vertex) {
    vertexes.push_back(vertex);
  }

  void addFace(int v1, int v2, int v3) {
    std::tuple<int,int,int> face = {v1, v2, v3};
    faces.push_back(face);
  }
};
 
Model newPlane(float length, int subdivisions);