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

VBO::VBO() {}

VBO::VBO(Model model) {
    colorBufferIndex = (GLuint*) malloc(sizeof(GLuint));
    vertexBufferIndex = (GLuint*) malloc(sizeof(GLuint));
    indexBufferIndex = (GLuint*) malloc(sizeof(GLuint));

    // Generate colors
    std::vector<float> colors;
    float r = 0.3;
    float g = 0.3;
    float b = 1;
    int colorSectionSize = model.vertices.size() / 18;
    for (int c = 0; c < (int) model.vertices.size() / 3; c++) {
        if (c < colorSectionSize) {
            r += 0.7 / colorSectionSize;
        } else if (c < 2 * colorSectionSize) {
            b -= 0.7 / colorSectionSize;
        } else if (c < 3 * colorSectionSize) {
            g += 0.7 / colorSectionSize;
        } else if (c < 4 * colorSectionSize) {
            r -= 0.7 / colorSectionSize;
        } else if (c < 5 * colorSectionSize) {
            b += 0.7 / colorSectionSize;
        } else {
            g -= 0.7 / colorSectionSize;
        }
        colors.push_back(r);
        colors.push_back(g);
        colors.push_back(b);
    }

    glGenBuffers(1, colorBufferIndex);
    glBindBuffer(GL_ARRAY_BUFFER, *colorBufferIndex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), colors.data(), GL_STATIC_DRAW);

    glGenBuffers(1, vertexBufferIndex);
    glBindBuffer(GL_ARRAY_BUFFER, *vertexBufferIndex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model.vertices.size(), model.vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, indexBufferIndex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexBufferIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * model.indices.size(), model.indices.data(),
                 GL_STATIC_DRAW);

    indexCount = model.indices.size();
}

void VBO::draw() {
    glBindBuffer(GL_ARRAY_BUFFER, *vertexBufferIndex);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, *colorBufferIndex);
    glColorPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexBufferIndex);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

VBOGroup::VBOGroup(ModelGroup modelGroup) {
    transformations = modelGroup.transformations;
    for (Model model : modelGroup.rootModels) {
        rootVBOs.emplace_back(model);
    }
    for (ModelGroup childGroup : modelGroup.childModels) {
        childVBOs.emplace_back(childGroup);
    }
}

void VBOGroup::draw() {
    glPushMatrix();

    for (Transform transform : transformations) {
        switch(transform.type) {
            case Transform::TRANSLATE:
                glTranslatef(transform.vector.x, transform.vector.y, transform.vector.z);
                break;
            case Transform::SCALE:
                glScalef(transform.vector.x, transform.vector.y, transform.vector.z);
                break;
            case Transform::ROTATE:
                glRotatef(transform.angle, transform.vector.x, transform.vector.y, transform.vector.z);
                break;
        }
    }

    for (VBO vbo : rootVBOs) {
        vbo.draw();
    }
    
    for (VBOGroup childGroup : childVBOs) {
        childGroup.draw();
    }

    glPopMatrix();
}

VBOController::VBOController(const std::vector<ModelGroup>& models) {
    for (const ModelGroup& group : models) { rootGroups.emplace_back(group); }
}

void VBOController::drawVBOs() {
    for (VBOGroup group : rootGroups) { group.draw(); }
}

void VBOController::drawVBOGroup(int index) {
    rootGroups[index].draw();
}