#include "generator/plane.hpp"
#include <stdexcept>

using namespace std;

const string Plane::help_message =
    "Plane requires a length's number and a divisions along each axis's number";

Plane::Plane(int argc, char **args) {
  if (argc < 2) {
    throw invalid_argument("Not enough arguments");
  }
  length = stod(*args);
}
