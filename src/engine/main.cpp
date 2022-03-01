#include <iostream>

#include "engine/render.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  std::cout << "Hello World!";
  Render::render(argc, argv);
  return 0;
}
