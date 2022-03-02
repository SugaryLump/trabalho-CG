#include <iostream>
#include <fstream>
#include <tuple>
#include "common/shapes.hpp"

using namespace std;

void writeModel(const char* filename, Model model) {
    ofstream stream;
    stream.open(filename);
    stream << "# Vertexes\n";
    for(long unsigned int v = 0; v < model.vertexes.size(); v++) {
        Vector3 vertex = model.vertexes[v];
        stream << "v " << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
    }

    stream << "\n# Faces\n";
    for(long unsigned int f = 0; f < model.faces.size(); f++) {
        tuple<int,int,int> face = model.faces[f];
        stream << "f " << get<0>(face) << " " << get<1>(face) << " " << get<2>(face) << "\n";
    }
    stream.close();
}