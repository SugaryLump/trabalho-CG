#include <fmt/core.h>

#include <iostream>

#include "engine/config.hpp"
#include "engine/render.hpp"
#include "engine/xmlParser.hpp"

using namespace std;

static char *PROGRAM_NAME;
void printHelpPage() {
    fmt::print(stderr,
               "HELP PAGE:\n"
               "Usage: {} filename\n",
               PROGRAM_NAME);

    fmt::print(stderr, "\t-help\n\t-h\n");
}

int main(int argc, char *argv[]) {
    PROGRAM_NAME = argv[0];

    if (argc < 2 || string(argv[1]) == "-h" || string(argv[1]) == "-help") {
        printHelpPage();
        return 1;
    }

    char *filename = argv[1];

    // chamar o parser
    Config config = Parser::parser(filename);
    // Config config;

    Render::render(argc, argv, config);

    return 0;
}
