#pragma once

#include <string>

#include "figure.hpp"

class Plane : public Figure {
private:
  static const std::string help_message;
  double length;

public:
  // const static std::string help_message;
  Plane(int argc, char **args);
  ~Plane(){};
};

