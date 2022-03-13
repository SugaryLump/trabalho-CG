#include <fmt/core.h>

#include <argparse/argparse.hpp>
#include <iostream>
#include <map>

#include "common/geometry.hpp"

void printError(const std::string &error) {
    std::cerr << error;
    std::exit(1);
}

int main(int argc, char *argv[]) {
    argparse::ArgumentParser program(argv[0]);
    program.add_argument("filename").help("The file path to write to.");
    program.add_argument("shape").help("[plane|box|sphere|cone|torus]");
    program.add_argument("parameters")
        .help(
            "Plane: [length] [subdivisions]\n\t\tBox: [length] [subdivisions]\n\t\tSphere: [radius] [slices] "
            "[stacks]\n\t\tCone: [radius] [height] [slices] [stacks]\n\t\tTorus: [radius] [tube radius] [toroidal "
            "slices] [poloidal slices]")
        .remaining()
        .scan<'f', float>();

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << program;
        std::exit(1);
    }

    auto filename = program.get<std::string>("filename");
    auto shape = program.get<std::string>("shape");
    auto params = program.get<std::vector<float>>("parameters");
    Model model;

    if (shape == "plane") {
        if (params.size() != 2) {
            std::cerr << program;
            std::exit(1);
        } else {
            model = Model::generatePlane(params[0], (int) params[1]);
        }
    } else if (shape == "box") {
        if (params.size() != 2) {
            std::cerr << program;
            std::exit(1);
        } else {
            model = Model::generateBox(params[0], (int) params[1]);
        }
    } else if (shape == "sphere") {
        if (params.size() != 3) {
            std::cerr << program;
            std::exit(1);
        } else {
            model = Model::generateSphere(params[0], (int) params[1], (int) params[2]);
        }
    } else if (shape == "cone") {
        if (params.size() != 4) {
            std::cerr << program;
            std::exit(1);
        } else {
            model = Model::generateCone(params[0], params[1], (int) params[2], (int) params[3]);
        }
    } else if (shape == "torus") {
        if (params.size() != 4) {
            std::cerr << program;
            std::exit(1);
        } else {
            model = Model::generateTorus(params[0], params[1], (int) params[2], (int) params[3]);
        }
    }

    model.toFile(filename);

    return 0;
}
