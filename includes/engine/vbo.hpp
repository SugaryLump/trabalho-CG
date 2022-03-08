#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>

#include "common/shapes.hpp"

class VBOController {
   private:
    GLuint* buffers;
    GLuint* vertices;

   public:
    VBOController() = default;
    explicit VBOController(std::vector<Model> models);

    void drawModels();
};