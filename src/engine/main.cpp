#include <iostream>

#include "engine/render.hpp"
//#include "engine/xmlParser.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    // chamar o parser
    // Parser::parser();

    Render::render(argc, argv);

    return 0;
}
