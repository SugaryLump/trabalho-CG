#include <fmt/core.h>
#include <argparse/argparse.hpp>

#include <iostream>
#include <map>

#include "common/geometry.hpp"

static char *PROGRAM_NAME;

void printError(std::string error) {
    std::cerr << error;
    std::exit(1);
}

int main(int argc, char *argv[]) {
    PROGRAM_NAME = argv[0];
    argparse::ArgumentParser program(PROGRAM_NAME);
    program.add_argument("filename")
        .help("The file path to write to.");
    program.add_argument("shape")
        .help("[plane|box|sphere|cone|torus]");
    program.add_argument("parameters")
        .help("Plane: [length] [subdivisions]\n\t\tBox: [length] [subdivisions]\n\t\tSphere: [radius] [slices] [stacks]\n\t\tCone: [radius] [height] [slices] [stacks]\n\t\tTorus: [radius] [tube radius] [horizontal slices] [vertical slices]")
        .remaining()
        .scan<'f',float>();
    
    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << program;
        std::exit(1);
    }
    
    std::string filename = program.get<std::string>("filename");
    std::string shape = program.get<std::string>("shape");
    std::vector<float> params = program.get<std::vector<float>>("parameters");
    Model model;


    if(!shape.compare("plane")) {
        if (params.size() != 2) {
            std::cerr << program;
            std::exit(1);
        }
        else {
            model = Model::generatePlane(params[0], (int)params[1]);
        }
    }

    else if (!shape.compare("box")) {
        if (params.size() != 2) {
            std::cerr << program;
            std::exit(1);
        }
        else {
            model = Model::generateBox(params[0], (int)params[1]);
        }
    }

    else if (!shape.compare("sphere")) {
        if (params.size() != 3) {
            std::cerr << program;
            std::exit(1);
        }
        else {
            model = Model::generateSphere(params[0], (int)params[1], (int)params[2]);
        }
    }

    else if (!shape.compare("cone")) {
        if (params.size() != 4) {
            std::cerr << program;
            std::exit(1);
        }
        else {
            model = Model::generateCone(params[0], params[1], (int)params[2], (int)params[3]);
        }
    }

    else if (!shape.compare("torus")) {
        if (params.size() != 4) {
            std::cerr << program;
            std::exit(1);
        }
        else {
            model = Model::generateTorus(params[0], params[1], (int)params[2], (int)params[3]);
        }
    }

    model.toFile(filename);

    return 0;
}
