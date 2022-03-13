#include "engine/vbo.hpp"

#include <cstdlib>
#include <vector>

#include "common/geometry.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

VBOController::VBOController() {}

VBOController::VBOController(std::vector<Model> models) {
    for (Model model : models) { vbos.push_back(VBO(model)); }
}

void VBOController::drawVBOs() {
    for (VBO vbo : vbos) { vbo.draw(); }
}

void VBOController::drawVBO(int index) {
    vbos[index].draw();
}

VBO::VBO() {}

VBO::VBO(Model model) {
    vertexBufferIndex = (GLuint*)malloc(sizeof(GLuint));
    indexBufferIndex = (GLuint*)malloc(sizeof(GLuint));

    glGenBuffers(1, vertexBufferIndex);
    glBindBuffer(GL_ARRAY_BUFFER, *vertexBufferIndex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model.vertices.size(), model.vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, indexBufferIndex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexBufferIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * model.indices.size(), model.indices.data(), GL_STATIC_DRAW);

    indexCount = model.indices.size();
}

void VBO::draw() {
    glBindBuffer(GL_ARRAY_BUFFER, *vertexBufferIndex);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexBufferIndex);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}