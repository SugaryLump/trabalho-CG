#pragma once

#include <tuple>
#include <vector>
#include <string>

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

class Model {
   public:
    std::vector<float> vertexes;
    std::vector<unsigned int> faces;

    /**
     * @brief Cria um modelo vazio
     */
    Model() = default;
    /**
     * @brief Gera um modelo a partir de um ficheiro .3d
     * @param path Localização do ficheiro
     */
    explicit Model(std::string const &path);

    void addVertex(Vector3 vertex);
    void addFace(unsigned int v1, unsigned int v2, unsigned int v3);
};

Model newPlane(float length, int subdivisions);
Model newBox(float length, int subdivisions);
int calcBoxInnerVertexes(int x, int y, int z, int subdivisions);
int calcBoxVertexIndex(int x, int y, int z, int subdivisions);
Model newSphere(float radius, int slices, int stacks);
Model newCone(float radius, float height, float slices, float stacks);