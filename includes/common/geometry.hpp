#pragma once

#include <string>
#include <tuple>
#include <vector>
#include <memory>

void buildRotationMatrix(float* x, float* y, float* z, float* matrix);
void normalizeVector(float* vector);
void crossVectors (float* v1, float* v2, float* result);

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

class Transform {
   public:
    Transform() = default;
    virtual void dummy() {}
};

class Translate:public Transform {
   public:
    Vector3 vector; 

    Translate(Vector3 vector);
};

class Rotate:public Transform {
   public:
    Vector3 axis;
    float angle; 

    Rotate(Vector3 axis, float angle);
};

class Scale:public Transform {
   public:
    Vector3 vector;

    Scale(Vector3 vector);
};

class Curve:public Transform {
   public:
    std::vector<std::vector<float>> points;
    int pointCount;
    float seconds;
    bool align;
    std::vector<float> previousY;

    Curve(std::vector<std::vector<float>> points, int pointCount, float seconds, bool align);
    void getCatmullRomPoint(float time, std::vector<float> p0, std::vector<float> p1, std::vector<float> p2, std::vector<float> p3, float* pos, float* deriv);
    void getCurrentPoint(float globalTime, float* pos, float* deriv);
};

class TimedRotate:public Transform {
   public:
    Vector3 axis;
    float seconds;

    TimedRotate(Vector3 axis, float seconds);
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
    static Model generateCylinder(float bRadius, float tRadius, float height, int slices, int stacks);
    static Model generateBezierPatch(std::string controlPointsFileName, int tessellation);
};

class ModelGroup {
   public:
    std::vector<std::shared_ptr<Transform>> transformations;
    std::vector<Model> rootModels;
    std::vector<ModelGroup> childModels;

    /**
     * @brief Cria um grupos de modelos vazio
     */
    ModelGroup() = default;
    ModelGroup(std::vector<Model> models, std::vector<std::shared_ptr<Transform>> transforms);

    void addChildGroup(ModelGroup childGroup);
};

int calcBoxInnerVertexes(int x, int y, int z, int subdivisions);
int calcBoxVertexIndex(int x, int y, int z, int subdivisions);