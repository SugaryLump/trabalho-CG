#include "common/geometry.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

// Vector3
Vector3::Vector3(float x_, float y_, float z_) {
    x = x_;
    y = y_;
    z = z_;
}

Vector3::Vector3(float x_, float z_) {
    x = x_;
    y = 0;
    z = z_;
}

Vector3 Vector3::fromSpherical(float alpha, float beta, float radius) {
    float x = cos(beta) * sin(alpha) * radius;
    float y = sin(beta) * radius;
    float z = cos(beta) * cos(alpha) * radius;
    return Vector3(x, y, z);
}

void Vector3::applyVector(Vector3 vector) {
    x += vector.x;
    y += vector.y;
    z += vector.z;
}

// Spherical
Spherical::Spherical(float radius_, float alpha_, float beta_) {
    radius = radius_;
    alpha = alpha_;
    beta = beta_;
}

Spherical::Spherical(float radius_, float alpha_) {
    radius = radius_;
    alpha = alpha_;
    beta = 0;
}

Vector3 Spherical::toVector3(float centerX, float centerY, float centerZ) {
    float x = centerX + cos(beta) * sin(alpha) * radius;
    float y = centerY + sin(beta) * radius;
    float z = centerZ + cos(beta) * cos(alpha) * radius;
    return Vector3(x, y, z);
}

// Triangle
Triangle::Triangle(Vector3 v1, Vector3 v2, Vector3 v3) {
    p1 = v1;
    p2 = v2;
    p3 = v3;
}

// Model
Model::Model(const string& path) {
    string line;
    ifstream file(path);

    while (getline(file, line)) {
        switch (line[0]) {
            case '\n':
                break;
            case '#':
            case '\000':
                break;
            case 'v': {
                float n1, n2, n3;
                // TODO: Ver queixa do clang-tidy
                if (sscanf(line.c_str(), "v %f %f %f\n", &n1, &n2, &n3) != 3) {
                    cerr << "Error reading line: " << line << endl;
                    break;
                }
                this->addVertex(Vector3(n1, n2, n3));

                break;
            }
            case 'f': {
                int n1, n2, n3;
                // TODO: Ver queixa do clang-tidy
                if (sscanf(line.c_str(), "f %d %d %d\n", &n1, &n2, &n3) != 3) {
                    cerr << "Error reading line: " << line << endl;
                    break;
                }

                this->addFace(n1, n2, n3);
                break;
            }
            default:
                cerr << "error: invalid .3d file" << endl;
                // TODO: Abortar
                break;
        }
    }

    file.close();
}

void Model::toFile(std::string const& path) {
    ofstream stream;
    stream.open(path);
    stream << "# Vertexes\n";
    for (long unsigned int v = 0; v + 2 < vertices.size(); v += 3) {
        stream << "v " << vertices[v] << " " << vertices[v + 1] << " " << vertices[v + 2] << "\n";
    }

    stream << "\n# Faces\n";
    for (long unsigned int f = 0; f + 2 < indices.size(); f += 3) {
        stream << "f " << indices[f] << " " << indices[f + 1] << " " << indices[f + 2] << "\n";
    }
    stream.close();
}

void Model::addVertex(Vector3 vertex) {
    vertices.push_back(vertex.x);
    vertices.push_back(vertex.y);
    vertices.push_back(vertex.z);
}

void Model::addFace(unsigned int v1, unsigned int v2, unsigned int v3) {
    indices.push_back(v1);
    indices.push_back(v2);
    indices.push_back(v3);
}

// length and subdivisions must be bigger than 0
Model Model::generatePlane(float length, int subdivisions) {
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

// length and subdivisions must be bigger than 0
Model Model::generateBox(float length, int subdivisions) {
    Model box = Model();
    float triangleSide = length / subdivisions;

    // Plane vertices
    float start = -length / 2;
    for (int x = 0; x <= subdivisions; x++) {
        for (int y = 0; y <= subdivisions; y++) {
            for (int z = 0; z <= subdivisions; z++) {
                if (z == 0 || x == 0 || y == 0 || z == subdivisions || x == subdivisions || y == subdivisions) {
                    box.addVertex(
                        Vector3(start + (x * triangleSide), start + (y * triangleSide), start + (z * triangleSide)));
                } else {
                    // Skip to right before the next plane
                    z += subdivisions - 2;
                }
            }
        }
    }

    for (int i = 0; i < subdivisions; i++) {
        for (int j = 0; j < subdivisions; j++) {
            int v1, v2, v3, v4;
            // Back plane
            // v2--v1
            //| / |
            // v3--v4
            //  v3 has coordinates (i,j,0)
            v1 = calcBoxVertexIndex(i + 1, j + 1, 0, subdivisions);
            v2 = calcBoxVertexIndex(i, j + 1, 0, subdivisions);
            v3 = calcBoxVertexIndex(i, j, 0, subdivisions);
            v4 = calcBoxVertexIndex(i + 1, j, 0, subdivisions);
            box.addFace(v3, v2, v1);
            box.addFace(v3, v1, v4);

            // Front plane
            // v2--v1
            //| / |
            // v3--v4
            //  v3 has coordinates (i,j,subdivisions)
            v1 = calcBoxVertexIndex(i + 1, j + 1, subdivisions, subdivisions);
            v2 = calcBoxVertexIndex(i, j + 1, subdivisions, subdivisions);
            v3 = calcBoxVertexIndex(i, j, subdivisions, subdivisions);
            v4 = calcBoxVertexIndex(i + 1, j, subdivisions, subdivisions);
            box.addFace(v3, v1, v2);
            box.addFace(v3, v4, v1);

            // Left plane
            // v2-_
            //| / v1
            // v3-_|
            //     v4
            //  v3 has coordinates (0,i,j)
            v1 = calcBoxVertexIndex(0, i + 1, j + 1, subdivisions);
            v2 = calcBoxVertexIndex(0, i + 1, j, subdivisions);
            v3 = calcBoxVertexIndex(0, i, j, subdivisions);
            v4 = calcBoxVertexIndex(0, i, j + 1, subdivisions);
            box.addFace(v3, v1, v2);
            box.addFace(v3, v4, v1);

            // Right plane
            // v2-_
            //| / v1
            // v3-_|
            //     v4
            //  v3 has coordinates (subdivisions,i,j)
            v1 = calcBoxVertexIndex(subdivisions, i + 1, j + 1, subdivisions);
            v2 = calcBoxVertexIndex(subdivisions, i + 1, j, subdivisions);
            v3 = calcBoxVertexIndex(subdivisions, i, j, subdivisions);
            v4 = calcBoxVertexIndex(subdivisions, i, j + 1, subdivisions);
            box.addFace(v3, v2, v1);
            box.addFace(v3, v1, v4);

            // Bottom plane
            // v3--v2
            //| \ |
            // v4--v1
            //  v3 has coordinates (i,0,j)
            v1 = calcBoxVertexIndex(i + 1, 0, j + 1, subdivisions);
            v2 = calcBoxVertexIndex(i + 1, 0, j, subdivisions);
            v3 = calcBoxVertexIndex(i, 0, j, subdivisions);
            v4 = calcBoxVertexIndex(i, 0, j + 1, subdivisions);
            box.addFace(v3, v1, v4);
            box.addFace(v3, v2, v1);

            // Top plane
            // v3--v2
            //| \ |
            // v4--v1
            //  v3 has coordinates (i,subdivisions,j)
            v1 = calcBoxVertexIndex(i + 1, subdivisions, j + 1, subdivisions);
            v2 = calcBoxVertexIndex(i + 1, subdivisions, j, subdivisions);
            v3 = calcBoxVertexIndex(i, subdivisions, j, subdivisions);
            v4 = calcBoxVertexIndex(i, subdivisions, j + 1, subdivisions);
            box.addFace(v3, v4, v1);
            box.addFace(v3, v1, v2);
        }
    }

    return box;
}

int calcBoxInnerVertexes(int x, int y, int z, int subdivisions) {
    int total = 0;
    if (x > 1) { total += (x - 1) * pow(subdivisions - 1, 2); }
    if (x > 0 && x < subdivisions && y > 1) { total += (y - 1) * (subdivisions - 1); }
    if (x > 0 && x < subdivisions && y > 0 && y < subdivisions && z > 1) { total += z - 1; }
    return total;
}

int calcBoxVertexIndex(int x, int y, int z, int subdivisions) {
    return z + (y * (subdivisions + 1)) + (x * pow(subdivisions + 1, 2)) - calcBoxInnerVertexes(x, y, z, subdivisions);
}

// radius must be bigger than 0
// slices must be bigger than 2
// stacks must be bigger than 1
Model Model::generateSphere(float radius, int slices, int stacks) {
    Model sphere = Model();
    sphere.addVertex(Vector3(0, -radius, 0));
    sphere.addVertex(Vector3(0, radius, 0));

    for (int st = 0; st < stacks - 1; st++) {
        for (int sl = 0; sl < slices; sl++) {
            float alpha = sl * 2 * M_PI / slices;
            float beta = -M_PI_2 + (st + 1) * M_PI / stacks;
            sphere.addVertex(Vector3::fromSpherical(alpha, beta, radius));
        }
    }

    // Bottom and top stacks;
    for (int sl = 0; sl < slices; sl++) {
        if (sl < slices - 1) {
            sphere.addFace(0, sl + 3, sl + 2);
            sphere.addFace(1, (stacks - 2) * slices + 2 + sl, (stacks - 2) * slices + 3 + sl);
        } else {
            sphere.addFace(0, 2, sl + 2);
            sphere.addFace(1, (stacks - 2) * slices + 2 + sl, (stacks - 2) * slices + 2);
        }
    }

    // Middle stacks;
    for (int st = 0; st < stacks - 2; st++) {
        for (int sl = 0; sl < slices; sl++) {
            int v1, v2, v3, v4;
            if (sl < slices - 1) {
                v1 = st * slices + 2 + sl;
                v2 = v1 + 1;
                v3 = v1 + slices;
                v4 = v3 + 1;
            } else {
                v1 = st * slices + 2 + sl;
                v2 = st * slices + 2;
                v3 = v1 + slices;
                v4 = v2 + slices;
            }
            sphere.addFace(v1, v2, v4);
            sphere.addFace(v1, v4, v3);
        }
    }

    return sphere;
}

// radius, height and stacks must be bigger than 0
// slices must be bigger than 2
Model Model::generateCone(float radius, float height, int slices, int stacks) {
    Model cone = Model();

    // Vertexes
    cone.addVertex(Vector3(0, 0, 0));
    cone.addVertex(Vector3(0, height, 0));
    for (int st = 0; st < stacks; st++) {
        float alpha = 0;
        for (int sl = 0; sl < slices; sl++) {
            Vector3 vertex = Vector3::fromSpherical(alpha, 0, radius - (radius / stacks * st));
            vertex.applyVector(Vector3(0, (float) height / stacks * st, 0));
            cone.addVertex(vertex);
            alpha += 2 * M_PI / slices;
        }
    }

    // Faces
    for (int st = 0; st <= stacks; st++) {
        for (int sl = 0; sl < slices; sl++) {
            // Base
            if (st == 0) {
                if (sl < slices - 1) {
                    cone.addFace(0, sl + 3, sl + 2);
                } else {
                    cone.addFace(0, 2, sl + 2);
                }
            }
            // Top stack
            else if (st == stacks) {
                if (sl < slices - 1) {
                    cone.addFace(1, (st - 1) * slices + sl + 2, (st - 1) * slices + sl + 3);
                } else {
                    cone.addFace(1, (st - 1) * slices + sl + 2, (st - 1) * slices + 2);
                }
            }
            // Middle stacks
            else {
                if (sl < slices - 1) {
                    int v1 = (st - 1) * slices + sl + 2;
                    int v2 = (st - 1) * slices + sl + 3;
                    int v3 = st * slices + sl + 2;
                    int v4 = st * slices + sl + 3;
                    cone.addFace(v2, v3, v1);
                    cone.addFace(v2, v4, v3);
                } else {
                    int v1 = (st - 1) * slices + sl + 2;
                    int v2 = (st - 1) * slices + 2;
                    int v3 = st * slices + sl + 2;
                    int v4 = st * slices + 2;
                    cone.addFace(v2, v3, v1);
                    cone.addFace(v2, v4, v3);
                }
            }
        }
    }

    return cone;
}

Model Model::generateTorus(float radius, float tubeRadius, int tSlices, int pSlices) {
    Model torus = Model();

    // Vertexes
    for (int ps = 0; ps < pSlices; ps++) {
        float alpha = ps * (2 * M_PI / pSlices);
        Vector3 tubeCenter = Vector3::fromSpherical(alpha, 0, radius);
        for (int ts = 0; ts < tSlices; ts++) {
            float beta = ts * (2*M_PI / tSlices);
            Vector3 vertex = Vector3::fromSpherical(alpha, beta, tubeRadius);
            vertex.applyVector(tubeCenter);
            torus.addVertex(vertex);
        }
    }

    // Faces
    for (int ps = 0; ps < pSlices; ps++) {
        for (int ts = 0; ts < tSlices; ts++) {
            int v1, v2, v3, v4;
            v1 = ps * tSlices + ts;
            if (ps < pSlices - 1) {
                v2 = v1 + tSlices;
            }
            else {
                v2 = ts;
            }
            
            if (ts < tSlices - 1) {
                v3 = v1 + 1;
            }
            else {
                v3 = ps * tSlices;
            }

            if (ps < pSlices - 1) {
                v4 = v3 + tSlices;
            }
            else if (ts < tSlices - 1) {
                v4 = v2 + 1;
            }
            else {
                v4 = 0;
            }

            torus.addFace(v1, v4, v3);
            torus.addFace(v1, v2, v4);
        }
    }
    return torus;
}