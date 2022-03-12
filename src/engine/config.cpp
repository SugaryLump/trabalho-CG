#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "engine/config.hpp"
#include "engine/render.hpp"

Config::Config() {
    camera = Camera();
}

void Config::drawModels() {
    for (Model const& model : models) { drawModel(model); }
}

void Config::drawModel(Model const& model) {
    // GLuint vertices, verticeCount;
    std::vector<float> vec;

    GLuint* vbo = &Render::buffer;

    for (float value : model.vertices) { vec.push_back(value); }

    Render::_n_indices = model.indices.size();
    Render::indices = model.indices;

    // criar o vbo
    glGenBuffers(1, vbo);
    // copiar o vector para a memória gráfica
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *vbo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Render::indices.size(), &Render::indices[0],
                 GL_STATIC_DRAW);
}