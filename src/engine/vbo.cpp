#include "engine/vbo.hpp"

#include <cstdlib>
#include <vector>
#include <map>
#include <unordered_set>
#include <string>
#include <math.h>

#include "common/geometry.hpp"
#include "engine/textures.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

static std::map<std::string, Buffers*> bufferTable;
static std::map<std::string, unsigned int*> textureTable;

void initBuffers(std::map<std::string, Model> modelTable) {
    std::map<std::string, Model>::iterator it = modelTable.begin();

    while (it != modelTable.end()) {
        bufferTable[it->first] = new Buffers(it->second);
        it++;
    }
}

void initTextures(std::unordered_set<std::string> textureNames) {
    std::unordered_set<std::string>::iterator it = textureNames.begin();

    while (it != textureNames.end()) {
        TextureData textureData = TextureData(it->data());

        unsigned int* textureIndex = (unsigned int*)malloc(sizeof(unsigned int));

        glGenTextures(1, textureIndex);
        glBindTexture(GL_TEXTURE_2D, *textureIndex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.imageData);
        glGenerateMipmap(GL_TEXTURE_2D);

        textureTable[it->data()] = textureIndex;
        it++;
    }
}

Buffers::Buffers(Model model) {
    coordBufferIndex = (GLuint*)malloc(sizeof(GLuint));
    normalBufferIndex = (GLuint*)malloc(sizeof(GLuint));
    textureBufferIndex = (GLuint*)malloc(sizeof(GLuint));
    indexBufferIndex = (GLuint*)malloc(sizeof(GLuint));

    indexCount = model.indices.size();

    glGenBuffers(1, coordBufferIndex);
    glBindBuffer(GL_ARRAY_BUFFER, *coordBufferIndex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model.vCoords.size(), model.vCoords.data(), GL_STATIC_DRAW);

    glGenBuffers(1, normalBufferIndex);
    glBindBuffer(GL_ARRAY_BUFFER, *normalBufferIndex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model.vNormals.size(), model.vNormals.data(), GL_STATIC_DRAW);

    glGenBuffers(1, textureBufferIndex);
    glBindBuffer(GL_ARRAY_BUFFER, *textureBufferIndex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model.vTextureCoords.size(), model.vTextureCoords.data(), GL_STATIC_DRAW);

    glGenBuffers(1, indexBufferIndex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexBufferIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * model.indices.size(), model.indices.data(), GL_STATIC_DRAW);
}

VBO::VBO(ModelContainer model) {
    this->buffers = bufferTable[model.modelName];
    this->textureIndex = textureTable[model.textureName];
    this->colorData = model.colorData;
}

void VBO::draw() {
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorData.getDiffuse());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colorData.getSpecular());
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, colorData.getEmissive());
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colorData.getAmbient());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(colorData.shininess));

    if (textureIndex) {
        glBindTexture(GL_TEXTURE_2D, *textureIndex);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, *(buffers->coordBufferIndex));
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, *(buffers->normalBufferIndex));
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, *(buffers->textureBufferIndex));
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(buffers->indexBufferIndex));
    glDrawElements(GL_TRIANGLES, buffers->indexCount, GL_UNSIGNED_INT, 0);
}

VBOGroup::VBOGroup(ModelGroup modelGroup) {
    transformations = modelGroup.transformations;
    for (ModelContainer model : modelGroup.rootModels) {
        rootVBOs.emplace_back(model);
    }
    for (ModelGroup childGroup : modelGroup.childModels) {
        childVBOs.emplace_back(childGroup);
    }
}

void VBOGroup::draw(float time) {
    glPushMatrix();

    for (std::shared_ptr<Transform> transform : transformations) {
        if (Translate* translate = dynamic_cast<Translate*>(transform.get())) {
            Vector3 vector = translate->vector;
            glTranslatef(vector.x, vector.y, vector.z);
        }
        else if (Rotate* rotate = dynamic_cast<Rotate*>(transform.get())) {
            Vector3 axis = rotate->axis;
            glRotatef(rotate->angle, axis.x, axis.y, axis.z);
        }
        else if (Scale* scale = dynamic_cast<Scale*>(transform.get())) {
            Vector3 vector = scale->vector;
            glScalef(vector.x, vector.y, vector.z);
        }
        else if (Curve* curve = dynamic_cast<Curve*>(transform.get())) {
            float pos[3], x[3];
            curve->getCurrentPoint(time, pos, x);
            glTranslatef(pos[0], pos[1], pos[2]);
            if (curve->align) {
                float m[16], y[3], z[3];
                normalizeVector(x);
                crossVectors(x, curve->previousY.data(), z);
                crossVectors(z, x, y);
                normalizeVector(y);
                normalizeVector(z);
                buildRotationMatrix(x, y, z, m);
                glMultMatrixf(m);
                curve->previousY[0] = y[0];
                curve->previousY[1] = y[1];
                curve->previousY[2] = y[2];
            }
        }
        else if (TimedRotate* timedRotate = dynamic_cast<TimedRotate*>(transform.get())) {
            Vector3 axis = timedRotate->axis;
            float progress = time / timedRotate->seconds;
            progress -= floor(progress);
            float angle = progress * 360;
            glRotatef(angle, axis.x, axis.y, axis.z);
        }
    }

    for (VBO vbo : rootVBOs) {
        vbo.draw();
    }
    
    for (VBOGroup childGroup : childVBOs) {
        childGroup.draw(time);
    }

    glPopMatrix();
}

VBOController::VBOController(const std::vector<ModelGroup>& models) {
    for (const ModelGroup& group : models) { rootGroups.emplace_back(group); }
}

void VBOController::drawVBOs(float time) {
    for (VBOGroup group : rootGroups) { group.draw(time); }
}

void VBOController::drawVBOGroup(int index, float time) {
    rootGroups[index].draw(time);
}