#ifndef PLANE_H_
#define PLANE_H_
#include "figure.hpp"

class Plane : public Figure {
private:
  double length;

public:
  // const static std::string help_message;
  Plane(int argc, char **args);
  ~Plane(){};
};

#endif // PLANE_H_
