#include "common/geometry.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

//Auxiliary
void buildRotationMatrix(float* x, float* y, float* z, float* matrix) {
    matrix[0] = x[0]; matrix[1] = x[1]; matrix[2] = x[2]; matrix[3] = 0;
	matrix[4] = y[0]; matrix[5] = y[1]; matrix[6] = y[2]; matrix[7] = 0;
	matrix[8] = z[0]; matrix[9] = z[1]; matrix[10] = z[2]; matrix[11] = 0;
	matrix[12] = 0; matrix[13] = 0; matrix[14] = 0; matrix[15] = 1;
}

void normalizeVector(float* vector) {
    float l = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
	vector[0] = vector[0]/l;
	vector[1] = vector[1]/l;
	vector[2] = vector[2]/l;
}

void crossVectors (float* v1, float* v2, float* result) {
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
	result[1] = v1[2] * v2[0] - v1[0] * v2[2];
	result[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void multiplyMatrixes (float* m1, int cols1, int lines1, float* m2, int cols2, float* result) {
    for (int l = 0; l < lines1; l++) {
        for (int c = 0; c < cols2; c++) {
            int index = l * cols2 + c;
            result[index] = 0;
            for (int i = 0; i < cols1; i++) {
                result[index] += m1[l * cols1 + i] * m2[i * cols2 + c];
            }
        }
    }
}

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

//Transform
Translate::Translate(Vector3 vector) {
    this->vector = vector;
}

Rotate::Rotate(Vector3 axis, float angle) {
    this->axis = axis;
    this->angle = angle;
}

Scale::Scale(Vector3 vector) {
    this->vector = vector;
}

Curve::Curve(std::vector<std::vector<float>> points, int pointCount, float seconds, bool align) {
    this->points = points;
    this->pointCount = pointCount;
    this->seconds = seconds;
    this->align = align;
    std::vector<float> previousY;
    previousY.push_back(0);
    previousY.push_back(1);
    previousY.push_back(0);
    this->previousY = previousY;
}
void Curve::getCatmullRomPoint(float time, std::vector<float> p0, std::vector<float> p1, std::vector<float> p2, std::vector<float> p3, float* pos, float* deriv) {
	// catmull-rom matrix
	float m[16] = {		-0.5f,  1.5f, -1.5f,  0.5f,
						 1.0f, -2.5f,  2.0f, -0.5f,
						-0.5f,  0.0f,  0.5f,  0.0f,
						 0.0f,  1.0f,  0.0f,  0.0f};

	for (int i = 0; i < 3; i++) {
		float a[4];
		float p[4] = {p0[i], p1[i], p2[i], p3[i]};
        for (int j = 0; j < 4; ++j) {
            a[j] = 0;
            for (int k = 0; k < 4; ++k) {
                a[j] += p[k] * m[j * 4 + k];
		    }
        }
		pos[i] = pow(time, 3) * a[0] + pow(time, 2) * a[1] + time * a[2] + a[3];
		deriv[i] = 3 * pow(time, 2) * a[0] + 2 * time * a[1] + a[2];
    }
}

void Curve::getCurrentPoint(float globalTime, float* pos, float* deriv) {
    globalTime = globalTime / seconds;
	float time = globalTime * pointCount;
	int index = floor(time);
	time = time - index;

	// indices store the points
	int indices[4]; 
	indices[0] = (index + pointCount - 1) % pointCount;	
	indices[1] = (indices[0] + 1) % pointCount;
	indices[2] = (indices[1] + 1) % pointCount; 
	indices[3] = (indices[2] + 1) % pointCount;

	this->getCatmullRomPoint(time, points[indices[0]], points[indices[1]], points[indices[2]], points[indices[3]], pos, deriv);
}

TimedRotate::TimedRotate(Vector3 axis, float seconds) {
    this->axis = axis;
    this->seconds = seconds;
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

//PatchData
PatchData::PatchData(const string& patchFileName) {
    int totalPatches;
    int totalPoints;

    string line;
    ifstream file(patchFileName);

    //Reading the control point indices
    getline(file, line);
    sscanf(line.c_str(), "%d\n", &totalPatches);
    for (int p = 0; p < totalPatches; p++) {
        getline(file, line);
        stringstream stream = stringstream(line);
        vector<int> indices;
        for (int v = 0; v < 16; v++) {
            string indexString;
            int index;
            getline(stream, indexString, ',');
            sscanf(indexString.c_str(), "%d", &index);
            indices.push_back(index);
        }
        patchesIndices.push_back(indices);
    }

    //Reading the control points
    getline(file, line);
    sscanf(line.c_str(), "%d\n", &totalPoints);
    for (int v = 0; v < totalPoints; v++) {
        vector<float> point;
        float x, y, z;
        getline(file, line);
        sscanf(line.c_str(), "%f, %f, %f", &x, &y, &z);
        point.push_back(x);
        point.push_back(y);
        point.push_back(z);
        points.push_back(point);
    }
}

int PatchData::getPatchCount() {
    return patchesIndices.size();
}

void PatchData::addPoint(Vector3 point) {
    vector<float> newPoint;
    newPoint.push_back(point.x);
    newPoint.push_back(point.y);
    newPoint.push_back(point.z);
    points.push_back(newPoint);
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
            float beta = ts * (2 * M_PI / tSlices);
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
            } else {
                v2 = ts;
            }

            if (ts < tSlices - 1) {
                v3 = v1 + 1;
            } else {
                v3 = ps * tSlices;
            }

            if (ps < pSlices - 1) {
                v4 = v3 + tSlices;
            } else if (ts < tSlices - 1) {
                v4 = v2 + 1;
            } else {
                v4 = 0;
            }

            torus.addFace(v1, v4, v3);
            torus.addFace(v1, v2, v4);
        }
    }
    return torus;
}

Model Model::generateCylinder(float bRadius, float tRadius, float height, int slices, int stacks) {
    Model cylinder = Model();

    float sliceAngle = 2 * M_PI / slices;

    //Vertexes
    //Base and top
    Vector3 baseCenter = Vector3(0, -height / 2, 0);
    Vector3 topCenter = Vector3(0, height / 2, 0);
    cylinder.addVertex(baseCenter);
    cylinder.addVertex(topCenter);

    //Body
    for (int st = 0; st <= stacks; st++) {
        float bodyRadius = bRadius -  st * ((bRadius - tRadius) / stacks);
        float bodyHeight = -height / 2 + st * (height / stacks);
        for(int sl = 0; sl < slices; sl++) {
            float alpha = sl * sliceAngle;
            Vector3 bodyV = Vector3::fromSpherical(alpha, 0, bodyRadius);
            bodyV.applyVector(Vector3(0, bodyHeight, 0));
            cylinder.addVertex(bodyV);
        }
    }

    //Faces
    for (int sl = 0; sl < slices; sl++) {
        if (sl < slices - 1) {
            //Bottom
            cylinder.addFace(0, sl + 3, sl + 2);
            //Top
            cylinder.addFace(1, slices * (stacks) + 2 + sl, slices * (stacks) + 3 + sl);
        }
        else {
            //Bottom
            cylinder.addFace(0, 2, sl + 2);
            //Top
            cylinder.addFace(1, slices * (stacks) + 2 + sl, slices * (stacks) + 2);
        }
        for (int st = 0; st < stacks; st++) {
            int v0 = st * slices + 2 + sl;
            int v1;
            if (sl < slices - 1) {
                v1 = v0 + 1;  
            }
            else {
                v1 = st * slices + 2;
            }
            int v2 = v0 + slices;
            int v3 = v1 + slices;
            cylinder.addFace(v0, v1, v2);
            cylinder.addFace(v1, v3, v2);
        }
    }

    return cylinder;
}

Model Model::generateBezierPatch(PatchData patchData, int tessellation) {
    Model patch = Model();

    float m[16] = { -1.0f,  3.0f, -3.0f,  1.0f,
                     3.0f, -6.0f,  3.0f,  0.0f,
                    -3.0f,  3.0f,  0.0f,  0.0f,
                     1.0f,  0.0f,  0.0f,  0.0f };
    //Generating the patches
    for (int pi = 0; pi < patchData.getPatchCount(); pi++) {
        //Generating the points
        for(int udiv = 0; udiv <= tessellation; udiv++) {
            for (int vdiv = 0; vdiv <= tessellation; vdiv++) {
                float u = 1.0f / tessellation * udiv;
                float v = 1.0f / tessellation * vdiv;
                float um[4] = {powf(u, 3), powf(u, 2), u, 1};
                float vm[4] = {powf(v, 3), powf(v, 2), v, 1};

                float point[3];
                for (int c = 0; c < 3; c++) {
                    vector<int> inds = patchData.patchesIndices[pi];
                    float p[16] = { patchData.points[inds[0]][c], patchData.points[inds[1]][c], patchData.points[inds[2]][c], patchData.points[inds[3]][c],
                                    patchData.points[inds[4]][c], patchData.points[inds[5]][c], patchData.points[inds[6]][c], patchData.points[inds[7]][c],
                                    patchData.points[inds[8]][c], patchData.points[inds[9]][c], patchData.points[inds[10]][c], patchData.points[inds[11]][c],
                                    patchData.points[inds[12]][c], patchData.points[inds[13]][c], patchData.points[inds[14]][c], patchData.points[inds[15]][c]};
                    float tmp1[4], tmp2[4], tmp3[4];
                    float result;
                    multiplyMatrixes(um, 4, 1, m, 4, tmp1);
                    multiplyMatrixes(tmp1, 4, 1, p, 4, tmp2);
                    multiplyMatrixes(tmp2, 4, 1, m, 4, tmp3);
                    multiplyMatrixes(tmp3, 4, 1, vm, 1, &result);
                    point[c] = result;
                }

                patch.addVertex(Vector3(point[0], point[1], point[2]));
            }
        }

        //Generating the faces
        for(int udiv = 0; udiv < tessellation; udiv++) {
            for (int vdiv = 0; vdiv < tessellation; vdiv++) {
                int patchPointsStart = (tessellation + 1) * (tessellation +1) * pi;
                int v1 = udiv * (tessellation + 1) + vdiv + patchPointsStart;
                int v2 = v1 + 1;
                int v3 = v1 + tessellation + 1;
                int v4 = v3 + 1;
                patch.addFace(v1, v2, v3);
                patch.addFace(v2, v4, v3);
            }
        }
    }

    return patch;
}

Model Model::generateComet(float radius, int randomness, int tessellation) {
    PatchData patchData = PatchData();
    srand(time(0));

    patchData.addPoint(Vector3(0, -radius, 0));
    patchData.addPoint(Vector3(0, radius, 0));
    for (int st = 0; st < 5; st++) {
        for (int sl = 0; sl < 12; sl++) {
            float alpha = sl * 2 * M_PI / 12;
            float beta = -M_PI_2 + (st + 1) * M_PI / 6;
            float height = radius * ( 1 + (rand() % randomness - (randomness - 1) / 2.0f) / 10);
            Vector3 point = Vector3::fromSpherical(alpha, beta, height);
            patchData.addPoint(point);
        }
    }

    for (int p = 0; p < 8; p++) {
        vector<int> indices;
        if (p < 4) {
            for (int i = 0; i < 4; i++) {
                indices.push_back(0);
            }
            for (int c = 0; c < 3; c++) {
                indices.push_back(2 + p * 3 + c * 12);
                indices.push_back(3 + p * 3 + c * 12);
                indices.push_back(4 + p * 3 + c * 12);
                if (p != 3) {
                    indices.push_back(5 + p * 3 + c * 12);
                }
                else {
                    indices.push_back(2 + c * 12);
                }
            }
        }
        else {
            for (int i = 0; i < 4; i++) {
                indices.push_back(1);
            }
            for (int c = 0; c < 3; c++) {
                if (p != 7) {
                    indices.push_back(53 + (p - 4) * 3 - c * 12);
                }
                else {
                    indices.push_back(50 - c * 12);
                }
                indices.push_back(52 + (p - 4) * 3 - c * 12);
                indices.push_back(51 + (p - 4) * 3 - c * 12);
                indices.push_back(50 + (p - 4) * 3 - c * 12);
            }
        }
        patchData.patchesIndices.push_back(indices);
    }

    return Model::generateBezierPatch(patchData, tessellation);
}

ModelGroup::ModelGroup(std::vector<Model> models, std::vector<std::shared_ptr<Transform>> transforms) {
    rootModels = models;
    transformations = transforms;
}

void ModelGroup::addChildGroup(ModelGroup childGroup) {
    childModels.push_back(childGroup);
}