#include <fmt/core.h>

#include <iostream>
#include <map>

#include "common/shapes.hpp"
#include "generator/figure.hpp"
#include "generator/io.hpp"
#include "generator/plane.hpp"

using namespace std;

static char *PROGRAM_NAME;

// aqui no 1 elemento podia tar associado uma funcao, q seria chamada depois
// a string  seria o erro
static std::map<std::string, pair<int, string>> mapOptionsValues;

void printHelpPage() {
    fmt::print(stderr,
               "HELP PAGE:\n"
               "Usage: {} filename option [args...]\n"
               "Options:\n",
               PROGRAM_NAME);

    for (auto & mapOptionsValue : mapOptionsValues) {
        fmt::print(stderr, "\t{}\n", mapOptionsValue.first);
    }

    fmt::print(stderr,
               "\t-help\n\t-h");
}

void initializeOptionsValues(int argc, char *argv[]) {
    int i = 1;
    mapOptionsValues["plane"] = {i, "PLANE!!"};
    mapOptionsValues["box"] = {++i, "BOX!!"};
    mapOptionsValues["sphere"] = {++i, "SPHERE!!"};
    mapOptionsValues["cone"] = {++i, "CONE!!!"};
}

int main(int argc, char *argv[]) {
    initializeOptionsValues(argc - 3, argv + 3);
    PROGRAM_NAME = argv[0];

    if (argc < 3 || string(argv[1]) == "-h" || string(argv[1]) == "-help") {
        printHelpPage();
        return 1;
    }

    const std::string option = argv[2];

    if (mapOptionsValues.find(option) == mapOptionsValues.end()) {
        printHelpPage();
        return 1;
    }

    fmt::print(stderr, "{}\n", mapOptionsValues[option].second);
    // Box Generation
    Model box = newBox(2, 2);
    writeModel("box.3d", box);

    /*
    //Plane Generation
    Model plane = newBox(3, 3);
    writeModel("plane.3d", plane);
    */

    return 0;
}
