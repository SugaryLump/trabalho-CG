#pragma once

#include <string>
#include <tuple>
#include <vector>

class Vector3 {
    public:
        float x;
        float y;
        float z;

        Vector3() = default;
        Vector3(float newx, float newy, float newz);
        Vector3(float newx, float newz);

        static Vector3 fromSpherical(float alpha, float beta, float radius);
        void applyVector(Vector3 vector);
};

class Spherical {
    public:
        float radius;
        float alpha;
        float beta;

        Spherical() = default;
        Spherical(float radius, float alpha, float beta);
        Spherical(float radius, float alpha);
        Vector3 toVector3(float centerX, float centerY, float centerZ);
};

class Triangle {
    public:
        Vector3 p1;
        Vector3 p2;
        Vector3 p3;

        Triangle() = default;
        Triangle(Vector3 v1, Vector3 v2, Vector3 v3);
};

class Model {
   public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    /**
     * @brief Cria um modelo vazio
     */
    Model() = default;
    /**
     * @brief Gera um modelo a partir de um ficheiro .3d
     * @param path Localização do ficheiro
     */
    explicit Model(std::string const &path);

    void toFile(std::string const &path);
    void addVertex(Vector3 vertex);
    void addFace(unsigned int v1, unsigned int v2, unsigned int v3);
    static Model generatePlane(float length, int subdivisions);
    static Model generateBox(float length, int subdivisions);
    static Model generateSphere(float radius, int slices, int stacks);
    static Model generateCone(float radius, float height, int slices, int stacks);
    static Model generateTorus(float radius, float tubeRadius, int hSlices, int vSlices);
};

int calcBoxInnerVertexes(int x, int y, int z, int subdivisions);
int calcBoxVertexIndex(int x, int y, int z, int subdivisions);