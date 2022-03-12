#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <cstdlib>
#include <vector>

#include "engine/render.hpp"
#include "engine/vbo.hpp"

VBOController::VBOController(std::vector<Model> models) {
    // Load the VBOController's vertices and indices arrays
    int points = 0;
    for (Model model : models) {
        for (float vertexes : model.vertices) {
            // buffers->push_back(vertexes);
        }
        for (unsigned int elem : model.indices) {
            // vertices->push_back(points +  elem);
        }
        // points = buffers->size() / 3;
    }
}

void VBOController::drawModels() {
    // Draw the VBOController's vertex and index arrays
    // glBindBuffer(GL_ARRAY_BUFFER,buffers);
    // glVertexPointer(3,GL_FLOAT,0,0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertices);
    // glDrawElements(GL_TRIANGLES,
    // vertices.size(), // número de índices a desenhar
    // GL_UNSIGNED_INT, // tipo de dados dos índices
    // 0);// parâmetro não utilizado
}

GLuint vertices, verticeCount;

namespace VBO {

void drawModels(std::vector<Model> const& models) {
    for (Model const& model : models) { drawModel(model); }
}

void drawModel(Model const& model) {
    // GLuint vertices, verticeCount;
    std::vector<float> vec;

    for (int f = 0; f < (int) model.indices.size(); f++) {
        // if (f % 3 == 0) {
        //     glColor3f(0.0, 0.185, 0.252);
        //     // glColor3f(0.2f, 0.44f, 0.54f);
        // } else if (f % 2 == 0) {
        //     glColor3f(0.183, 0.26, 0.231);
        // } else {
        //     glColor3f(0.126, 0.14, 0.189);
        // }
        vec.push_back(model.vertices[model.indices[f] * 3]);
        vec.push_back(model.vertices[model.indices[f] * 3 + 1]);
        vec.push_back(model.vertices[model.indices[f] * 3 + 2]);
    }
    // Render::_n_vertices = vec.size() / 3;
    GLuint vbo = Render::buffer;
    // criar o vbo
    glGenBuffers(1, &vbo);
    // copiar o vector para a memória gráfica
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vec.size(), vec.data(), GL_STATIC_DRAW);
}
}  // namespace VBO