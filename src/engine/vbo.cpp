#include "engine/vbo.hpp"

#include <vector>

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