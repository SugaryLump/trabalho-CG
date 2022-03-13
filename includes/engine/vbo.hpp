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
    VBO(Model model);

    void draw();
};

class VBOController {
   private:
    std::vector<VBO> vbos;

   public:
    VBOController();
    explicit VBOController(std::vector<Model> models);

    void drawVBOs();
    void drawVBO(int index);
};