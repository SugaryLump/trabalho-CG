#include <fmt/core.h>

#include <argparse/argparse.hpp>
#include <iostream>
#include <map>
#include <string>

#include "common/geometry.hpp"

void printError(const std::string &error) {
    std::cerr << error;
    std::exit(1);
}

int main(int argc, char *argv[]) {
    argparse::ArgumentParser program(argv[0]);
    program.add_argument("filename").help("The file path to write to.");
    program.add_argument("shape").help("[plane|box|sphere|cone|torus|cylinder|bezier|comet]");
    program.add_argument("parameters")
        .help(
            "Plane: [length] [subdivisions]\n\t\tBox: [length] [subdivisions]\n\t\tSphere: [radius] [slices] "
            "[stacks]\n\t\tCone: [radius] [height] [slices] [stacks]\n\t\tTorus: [radius] [tube radius] [toroidal "
            "slices] [poloidal slices]\n\t\tCylinder: [base radius] [top radius] [height] [slices] [stacks]\n\t\t"
            "Bezier Patches: [patch file path] [tessellation]\n\t\tComet: [radius] [randomness] [tessellation]")
        .remaining();

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << program;
        std::exit(1);
    }

    auto filename = program.get<std::string>("filename");
    auto shape = program.get<std::string>("shape");
    auto params = program.get<std::vector<std::string>>("parameters");
    Model model;

    if (shape == "plane") {
        if (params.size() != 2) {
            std::cerr << program;
            std::exit(1);
        } else {
            model = Model::generatePlane(stof(params[0]), stoi(params[1]));
        }
    } else if (shape == "box") {
        if (params.size() != 2) {
            std::cerr << program;
            std::exit(1);
        } else {
            model = Model::generateBox(stof(params[0]), stoi(params[1]));
        }
    } else if (shape == "sphere") {
        if (params.size() != 3) {
            std::cerr << program;
            std::exit(1);
        } else {
            model = Model::generateSphere(stof(params[0]), stoi(params[1]), stoi(params[2]));
        }
    } else if (shape == "cone") {
        if (params.size() != 4) {
            std::cerr << program;
            std::exit(1);
        } else {
            model = Model::generateCone(stof(params[0]), stof(params[1]), stoi(params[2]), stoi(params[3]));
        }
    } else if (shape == "torus") {
        if (params.size() != 4) {
            std::cerr << program;
            std::exit(1);
        } else {
            model = Model::generateTorus(stof(params[0]), stof(params[1]), stoi(params[2]), stoi(params[3]));
        }
    } else if (shape == "cylinder") {
        if (params.size() != 5) {
            std::cerr << program;
            std::exit(1);
        } else {
            model = Model::generateCylinder(stof(params[0]), stof(params[1]), stof(params[2]), stoi(params[3]), stoi(params[4]));
        }
    } else if (shape == "bezier") {
        if (params.size() != 2) {
            std::cerr << program;
            std::exit(1);
        } else {
            PatchData patchData = PatchData(params[0]);
            model = Model::generateBezierPatch(patchData, stoi(params[1]));
        }
    } else if (shape == "comet") {
        if (params.size() != 3) {
            std::cerr << program;
            std::exit(1);
        } else {
            model = Model::generateComet(stof(params[0]), stoi(params[1]), stoi(params[2]));
        }
    }

    model.toFile(filename);

    return 0;
}
