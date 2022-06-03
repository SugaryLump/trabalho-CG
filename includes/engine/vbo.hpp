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

void initBuffers(std::map<std::string, Model> modelTable);
void initTextures(std::vector<std::string> textureNames);

class Buffers {
   public:
    GLuint* coordBufferIndex;
    GLuint* normalBufferIndex;
    GLuint* textureBufferIndex;
    GLuint* indexBufferIndex;
    
    unsigned int indexCount;

    Buffers() = default;
    Buffers(Model model);
};

class VBO {
   private:
    Buffers* buffers;

    unsigned int* textureIndex;

    ColorData colorData;

   public:
    VBO() = default;
    explicit VBO(ModelContainer model);

    void draw();
};

class VBOGroup {
   private:
    std::vector<std::shared_ptr<Transform>> transformations;
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