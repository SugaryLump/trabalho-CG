#include "engine/render.hpp"
#include "engine/xmlParser.hpp"

int main(int argc, char *argv[]) {
    // chamar o parser
     Parser::parser();

    Render::render(argc, argv);

    return 0;
}
