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

class VBOGroup {
   private:
    std::vector<Transform> transformations;
    std::vector<VBO> rootVBOs;
    std::vector<VBOGroup> childVBOs;

   public:
    VBOGroup();
    explicit VBOGroup(ModelGroup modelGroup);

    void draw(float time);
};

class VBOController {
   private:
    std::vector<VBOGroup> rootGroups;

   public:
    VBOController() = default;
    explicit VBOController(const std::vector<ModelGroup>& models);

    void drawVBOs(float time);
    void drawVBOGroup(int index, float time);
};