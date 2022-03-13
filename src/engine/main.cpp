#include <fmt/core.h>

#include <argparse/argparse.hpp>
#include <iostream>

#include "engine/config.hpp"
#include "engine/render.hpp"
#include "engine/xmlParser.hpp"

using namespace std;

static const char *PROGRAM_NAME;
void printHelpPage() {
    fmt::print(stderr,
               "HELP PAGE:\n"
               "Usage: {} filename\n",
               PROGRAM_NAME);

    fmt::print(stderr, "\t-help\n\t-h\n");
}

int main(int argc, char *argv[]) {
    argparse::ArgumentParser program(argv[0]);
    program.add_argument("config").help("XML scene config file");

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << program;
        std::exit(1);
    }

    auto filename = program.get<std::string>("config");

    // chamar o parser
    optional<Config> config = Parser::parser(filename);
    if (config.has_value()) {
        Render::render(argc, argv, config.value());
    } else {
        fmt::print(stderr, "Error parsing config\n");
        return 1;
    }

    return 0;
}
