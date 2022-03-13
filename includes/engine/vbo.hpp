#pragma once

#include <cstdlib>
#include <memory>
#include <vector>

#include "common/geometry.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

class VBO {
   private:
    GLuint* vertexBufferIndex;
    GLuint* indexBufferIndex;
    GLuint* colorBufferIndex;

    unsigned int indexCount;

   public:
    VBO();
    explicit VBO(Model model);

    void draw();
};

class VBOController {
   private:
    std::vector<VBO> vbos;

   public:
    VBOController() = default;
    explicit VBOController(const std::vector<Model>& models);

    void drawVBOs();
    void drawVBO(int index);
};