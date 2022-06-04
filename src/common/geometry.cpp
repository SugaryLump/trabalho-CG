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

//Vertex
Vertex::Vertex(Vector3 position, Vector3 normal, Vector3 texture) {
    this->position = position;
    this->normal = normal;
    this->texture = texture;
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
                float p1, p2, p3, n1, n2, n3, t1, t2;
                // TODO: Ver queixa do clang-tidy
                if (sscanf(line.c_str(), "v %f %f %f %f %f %f %f %f\n", &p1, &p2, &p3, &n1, &n2, &n3, &t1, &t2) != 8) {
                    cerr << "Error reading line: " << line << endl;
                    break;
                }
                this->addVertex(Vector3(p1, p2, p3), Vector3(n1, n2, n3), Vector3(t1, t2, 0));

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
    for (long unsigned int v = 0; v + 2 < vCoords.size(); v += 3) {
        stream << "v " << vCoords[v] << " " << vCoords[v + 1] << " " << vCoords[v + 2];
        stream << vNormals[v] << " " << vNormals[v + 1] << " " << vNormals[v + 2];
        stream << vTextureCoords[v] << " " << vTextureCoords[v + 1] << "\n";
    }

    stream << "\n# Faces\n";
    for (long unsigned int f = 0; f + 2 < indices.size(); f += 3) {
        stream << "f " << indices[f] << " " << indices[f + 1] << " " << indices[f + 2] << "\n";
    }
    stream.close();
}

void Model::addVertex(Vector3 pos, Vector3 normal, Vector3 texCoords) {
    vCoords.push_back(pos.x);
    vCoords.push_back(pos.y);
    vCoords.push_back(pos.z);

    vNormals.push_back(normal.x);
    vNormals.push_back(normal.y);
    vNormals.push_back(normal.z);

    vTextureCoords.push_back(texCoords.x);
    vTextureCoords.push_back(texCoords.y);
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
            Vector3 pos = Vector3(xpos, zpos);
            float xtex = 1.0f / subdivisions * x;
            float ytex = 1.0f / subdivisions * z;
            Vector3 tex = Vector3(xtex, ytex, 0);
            Vector3 normal = Vector3(0, 1, 0);
            plane.addVertex(pos, normal, tex);
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
    float texSide = 1.0f / subdivisions;

    // Plane vertices
    float start = -length / 2;
    for (int u = 0; u <= subdivisions; u++) {
        for (int v = 0; v <= subdivisions; v++) {
            //Y- (u = x, v = z)
            Vector3 pos1 = Vector3(start + triangleSide * u,  start, start + triangleSide * v);
            Vector3 nor1 = Vector3(0, -1, 0);
            Vector3 tex1 = Vector3(texSide * u, texSide * v, 0);
            box.addVertex(pos1, tex1, nor1);

            //Y+ (u = x, v = z)
            Vector3 pos2 = Vector3(start + triangleSide * u, -start, start + triangleSide * v);
            Vector3 nor2 = Vector3(0, 1, 0);
            Vector3 tex2 = Vector3(texSide * u, texSide * v, 0);
            box.addVertex(pos2, nor2, tex2);

            //Z- (u = x, v = y)
            Vector3 pos3 = Vector3(start + triangleSide * u, start + triangleSide * v, start);
            Vector3 nor3 = Vector3(0, 0, -1);
            Vector3 tex3 = Vector3(texSide * u, texSide * v, 0);
            box.addVertex(pos3, nor3, tex3);

            //Z+ (u = x, v = y)
            Vector3 pos4 = Vector3(start + triangleSide * u, start + triangleSide * v, -start);
            Vector3 nor4 = Vector3(0, 0, 1);
            Vector3 tex4 = Vector3(texSide * u, texSide * v, 0);
            box.addVertex(pos4, nor4, tex4);

            //X- (u = y, v = z)
            Vector3 pos5 = Vector3(start, start + triangleSide * u, start + triangleSide * v);
            Vector3 nor5 = Vector3(-1, 0, 0);
            Vector3 tex5 = Vector3(texSide * u, texSide * v, 0);
            box.addVertex(pos5, nor5, tex5);

            //X+ (u = y, v = z)
            Vector3 pos6 = Vector3(-start, start + triangleSide * u, start + triangleSide * v);
            Vector3 nor6 = Vector3(1, 0, 0);
            Vector3 tex6 = Vector3(texSide * u, texSide * v, 0);
            box.addVertex(pos6, nor6, tex6);
        }
    }

    for (int u = 0; u < subdivisions; u++) {
        for (int v = 0; v < subdivisions; v++) {
            int v1, v2, v3, v4;
            //Y- (u = x, v = z)
            v1 = u * (subdivisions + 1) * 6 + 6 * v;
            v2 = v1 + 6;
            v3 = v1 + (subdivisions + 1) * 6;
            v4 = v3 + 6;
            box.addFace(v2, v1, v3);
            box.addFace(v2, v3, v4);

            //Y+ (u = x, v = z)
            v1 = 1 + u * (subdivisions + 1) * 6 + 6 * v;
            v2 = v1 + 6;
            v3 = v1 + (subdivisions + 1) * 6;
            v4 = v3 + 6;
            box.addFace(v2, v3, v1);
            box.addFace(v2, v4, v3);

            //Z- (u = x, v = y)
            v1 = 2 + u * (subdivisions + 1) * 6 + 6 * v;
            v2 = v1 + 6;
            v3 = v1 + (subdivisions + 1) * 6;
            v4 = v3 + 6;
            box.addFace(v2, v1, v3);
            box.addFace(v2, v3, v4);

            //Z+ (u = x, v = y)
            v1 = 3 + u * (subdivisions + 1) * 6 + 6 * v;
            v2 = v1 + 6;
            v3 = v1 + (subdivisions + 1) * 6;
            v4 = v3 + 6;
            box.addFace(v2, v3, v1);
            box.addFace(v2, v4, v3);

            //X- (u = y, v = z)
            v1 = 4 + u * (subdivisions + 1) * 6 + 6 * v;
            v2 = v1 + 6;
            v3 = v1 + (subdivisions + 1) * 6;
            v4 = v3 + 6;
            box.addFace(v2, v1, v3);
            box.addFace(v2, v3, v4);

            //X+ (u = y, v = z)
            v1 = 5 + u * (subdivisions + 1) * 6 + 6 * v;
            v2 = v1 + 6;
            v3 = v1 + (subdivisions + 1) * 6;
            v4 = v3 + 6;
            box.addFace(v2, v3, v1);
            box.addFace(v2, v4, v3);
        }
    }

    return box;
}

// radius must be bigger than 0
// slices must be bigger than 2
// stacks must be bigger than 1
Model Model::generateSphere(float radius, int slices, int stacks) {
    Model sphere = Model();

    for (int st = 0; st <= stacks; st++) {
        for (int sl = 0; sl <= slices; sl++) {
            float alpha = sl * 2 * M_PI / slices;
            float beta = -M_PI_2 + st * M_PI / stacks;
            Vector3 pos = Vector3::fromSpherical(alpha, beta, radius);
            Vector3 nor = Vector3::fromSpherical(alpha, beta, 1);
            Vector3 tex = Vector3(1.0f / slices * sl, 1.0f / stacks * st);
            sphere.addVertex(pos, nor, tex);
        }
    }

    // Bottom and top stacks;
    for (int sl = 0; sl < slices; sl++) {
        int v1, v2, v3;
        v1 = (stacks + 1) * sl;
        v2 = v1 + 1;
        v3 = v2 + stacks + 1; 
        sphere.addFace(v1, v3, v2);

        v1 = stacks + (stacks + 1) * sl;
        v2 = v1 - 1;
        v3 = v2 + stacks + 1;
        sphere.addFace(v1, v2, v3);
    }

    // Middle stacks;
    for (int st = 1; st < stacks - 1; st++) {
        for (int sl = 0; sl < slices; sl++) {
            int v1, v2, v3, v4;
            v1 = st + sl * (stacks + 1);
            v2 = v1 + stacks + 1;
            v3 = v1 + 1;
            v4 = v3 + stacks + 1;
            sphere.addFace(v1, v2, v3);
            sphere.addFace(v2, v4, v3);
        }
    }

    return sphere;
}

// radius, height and stacks must be bigger than 0
// slices must be bigger than 2
Model Model::generateCone(float radius, float height, int slices, int stacks) {
    Model cone = Model();

    // Vertexes
    //Base
    cone.addVertex(Vector3(0, 0, 0), Vector3(0, -1, 0), Vector3(0.5, 0.5, 0));
    for (int sl = 0; sl <= slices; sl++) {
        Vector3 pos = Vector3::fromSpherical(sl * 2*M_PI / slices, 0, radius);
        Vector3 nor = Vector3(0, -1, 0);
        Vector3 tex = Vector3::fromSpherical(0, sl * 2*M_PI / slices, 0.5);
        tex.applyVector(Vector3(0.5, 0.5, 0));
        cone.addVertex(pos, nor, tex);
    }

    //Cone
    for (int st = 0; st <= stacks; st++) {
        for (int sl = 0; sl <= slices; sl++) {
            Vector3 pos = Vector3::fromSpherical(sl * 2*M_PI / slices, 0, radius - (st * radius / stacks));
            pos.y = st * height / stacks;
            float normalAngle = atan(radius / height);
            Vector3 nor = Vector3::fromSpherical(sl * 2*M_PI / slices, normalAngle, 1);
            Vector3 tex = Vector3(1.0f / slices * sl, 1.0f / stacks * st);
            cone.addVertex(pos, nor, tex);
        }
    }

    // Faces
    //Base and cone tip
    for (int sl = 0; sl < slices; sl++) {
        int v1, v2, v3;
        v1 = 0;
        v2 = sl + 1;
        v3 = v2 + 1;
        cone.addFace(v1, v3, v2);

        v1 = (slices + 1) * stacks + 1;
        v2 = v1 - slices - 1;
        v3 = v2 + 1;
        cone.addFace(v1, v2, v3);
    }

    //Cone bottom
    for (int st = 0; st < stacks - 1; st++) {
        for (int sl = 0; sl < slices; sl++) {
            int v1, v2, v3, v4;
            v1 = 1 + st * (slices + 1) + sl;
            v2 = v1 + 1;
            v3 = v1 + slices + 1;
            v4 = v3 + 1;
            cone.addFace(v1, v2, v3);
            cone.addFace(v2, v4, v3);
        }
    }

    return cone;
}

Model Model::generateTorus(float radius, float tubeRadius, int tSlices, int pSlices) {
    Model torus = Model();/*

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
    }*/
    return torus;
}

Model Model::generateCylinder(float bRadius, float tRadius, float height, int slices, int stacks) {
    Model cylinder = Model();/*

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
    }*/

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
                float udm[4] = {3 * powf(u, 2), 2 * u, 1, 0};
                float vdm[4] = {3 * powf(v, 2), 2 * v, 1, 0};

                float point[3], derivu[3], derivv[3];
                for (int c = 0; c < 3; c++) {
                    vector<int> inds = patchData.patchesIndices[pi];
                    float p[16] = { patchData.points[inds[0]][c], patchData.points[inds[1]][c], patchData.points[inds[2]][c], patchData.points[inds[3]][c],
                                    patchData.points[inds[4]][c], patchData.points[inds[5]][c], patchData.points[inds[6]][c], patchData.points[inds[7]][c],
                                    patchData.points[inds[8]][c], patchData.points[inds[9]][c], patchData.points[inds[10]][c], patchData.points[inds[11]][c],
                                    patchData.points[inds[12]][c], patchData.points[inds[13]][c], patchData.points[inds[14]][c], patchData.points[inds[15]][c]};
                    float result, resultdu, resultdv;
                    //Point
                    float tmp1[4], tmp2[4], tmp3[4];
                    multiplyMatrixes(um, 4, 1, m, 4, tmp1);
                    multiplyMatrixes(tmp1, 4, 1, p, 4, tmp2);
                    multiplyMatrixes(tmp2, 4, 1, m, 4, tmp3);
                    multiplyMatrixes(tmp3, 4, 1, vm, 1, &result);
                    point[c] = result;
                    //Tangent vector (u)
                    float tmpdu1[4], tmpdu2[4], tmpdu3[4];
                    multiplyMatrixes(udm, 4, 1, m, 4, tmpdu1);
                    multiplyMatrixes(tmpdu1, 4, 1, p, 4, tmpdu2);
                    multiplyMatrixes(tmpdu2, 4, 1, m, 4, tmpdu3);
                    multiplyMatrixes(tmpdu3, 4, 1, vm, 1, &resultdu);
                    derivu[c] = resultdu;
                    //Tangent vector (v)
                    multiplyMatrixes(tmp3, 4, 1, vdm, 1, &resultdv);
                    derivv[c] = resultdv;
                }
                //Normal vector
                float normal[3];
                crossVectors(derivu, derivv, normal);
                normalizeVector(normal);
                Vector3 normalVector = Vector3(normal[0], normal[1], normal[2]);
                int tx = 1.0f / tessellation * u;
                int ty = 1.0f / tessellation * v;
                Vector3 texVector = Vector3(tx, ty, 0);

                patch.addVertex(Vector3(point[0], point[1], point[2]), normalVector, texVector);
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

ColorData::ColorData(Vector3 diffuse, Vector3 specular, Vector3 emissive, Vector3 ambient, float shininess) {
    this->diffuse = diffuse;
    this->specular = specular;
    this->emissive = emissive;
    this->ambient = ambient;
    this->shininess = shininess;
}

float* ColorData::getDiffuse() {
    float* param = (float*)malloc(sizeof(float) * 4);
    param[0] = diffuse.x;
    param[1] = diffuse.y;
    param[2] = diffuse.z;
    param[3] = 1.0f;

    return param;
}

float* ColorData::getSpecular() {
    float* param = (float*)malloc(sizeof(float) * 4);
    param[0] = specular.x;
    param[1] = specular.y;
    param[2] = specular.z;
    param[3] = 1.0f;

    return param;
}

float* ColorData::getEmissive() {
    float* param = (float*)malloc(sizeof(float) * 4);
    param[0] = emissive.x;
    param[1] = emissive.y;
    param[2] = emissive.z;
    param[3] = 1.0f;

    return param;
}

float* ColorData::getAmbient() {
    float* param = (float*)malloc(sizeof(float) * 4);
    param[0] = ambient.x;
    param[1] = ambient.y;
    param[2] = ambient.z;
    param[3] = 1.0f;

    return param;
}

ModelContainer::ModelContainer(std::string modelName, std::string textureName, ColorData colorData) {
    this->modelName = modelName;
    this->textureName = textureName;
    this->colorData = colorData;
}

ModelGroup::ModelGroup(std::vector<ModelContainer> rootModels, std::vector<std::shared_ptr<Transform>> transforms) {
    this->rootModels = rootModels;
    transformations = transforms;
}

void ModelGroup::addChildGroup(ModelGroup childGroup) {
    childModels.push_back(childGroup);
}