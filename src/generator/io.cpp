#include <iostream>
#include <fstream>
#include <tuple>
#include "common/shapes.hpp"

using namespace std;

void writeModel(const char* filename, Model model) {
    ofstream stream;
    stream.open(filename);
    stream << "# Vertexes\n";
    for(long unsigned int v = 0; v + 2 < model.vertexes.size(); v += 3) {
        stream << "v " << model.vertexes[v] << " " << model.vertexes[v+1] << " " << model.vertexes[v+2] << "\n";
    }

    stream << "\n# Faces\n";
    for(long unsigned int f = 0; f + 2 < model.faces.size(); f += 3) {
        stream << "f " << model.faces[f] << " " << model.faces[f+1] << " " << model.faces[f+2] << "\n";
    }
    stream.close();
}