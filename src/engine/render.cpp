#include "engine/render.hpp"

#include <cstdlib>
#include <memory>

#include "common/geometry.hpp"
#include "engine/camera.hpp"
#include "engine/input.hpp"
#include "engine/vbo.hpp"
#include "engine/light.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string>

#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

namespace Render {

static std::unique_ptr<Camera> camera;
static std::unique_ptr<InputState> input;
static std::unique_ptr<VBOController> vboController;

static float SCALE = 1;
static bool SHOW_AXIS = false;
static int line = GL_FILL;
static int windowWidth = 800;
static int windowHeight = 800;
static bool mouseWarping = false;
static int timeBase, startTime;

static std::vector<std::shared_ptr<LightSource>> lights;

void changeSize(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    // prevent a divide by zero, when window is too short
    // (you can not make a window with zero width).
    if (h == 0) h = 1;
    // compute window's aspect ratio
    float ratio = w * 1.0f / h;
    // set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load the identity matrix
    glLoadIdentity();
    // set the perspective
    // gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    gluPerspective(camera->getFov(), ratio, camera->getNear(), camera->getFar());
    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
    // set the viewport to be the entire window
    glViewport(0, 0, w, h);
}

void renderScene() {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // set camera
    glLoadIdentity();

    gluLookAt(camera->position.x, camera->position.y, camera->position.z, camera->look.x, camera->look.y,
              camera->look.z, camera->up.x, camera->up.y, camera->up.z);

    if (SHOW_AXIS) {
        glDisable(GL_LIGHTING);
        // Axis
        glBegin(GL_LINES);
        // X axis in red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-100.0f, 0.0f, 0.0f);
        glVertex3f(100.0f, 0.0f, 0.0f);
        // Y axis in Green
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -100.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);
        // Z axis in Blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -100.0f);
        glVertex3f(0.0f, 0.0f, 100.0f);
        glEnd();
        glEnable(GL_LIGHTING);
    }



    glScalef(SCALE, SCALE, SCALE);

    for (int i = 0; i < (int)lights.size(); i++) {
        if (PointLight* light = dynamic_cast<PointLight*>(lights[i].get())) {
            light->setupLight(i);
        }
        else if (DirectionalLight* light = dynamic_cast<DirectionalLight*>(lights[i].get())) {
            light->setupLight(i);
        }
        else if (Spotlight* light = dynamic_cast<Spotlight*>(lights[i].get())) {
            light->setupLight(i);
        }
    }

    glPolygonMode(GL_FRONT_AND_BACK, line);

    vboController->drawVBOs((timeBase - startTime) / 1000.0f);
    // End of frame
    glutSwapBuffers();
}

void keyPressHandler(unsigned char key, int _x, int _y) {
    input->pressKey(key);
}

void specialKeyPressHandler(int key, int _x, int _y) {
    input->pressSpecialKey(key);
}

void keyReleaseHandler(unsigned char key, int x, int y) {
    input->releaseKey(key);
}

void specialKeyReleaseHandler(int key, int x, int y) {
    input->releaseSpecialKey(key);
}

void passiveMouseHandler(int x, int y) {
    if (camera->currentType == FPS) {
        if (!mouseWarping) {
            input->updateMouseDelta(x, y, windowWidth / 2, windowHeight / 2);
            mouseWarping = true;
            glutWarpPointer(windowWidth / 2, windowHeight / 2);
        } else {
            mouseWarping = false;
        }
    }
}

void update() {
    // Framerates
    int time = glutGet(GLUT_ELAPSED_TIME);
    float fps = 1000.0f / (time - timeBase);
    timeBase = time;
    float rateModifier = 60 / fps;

    // Handle inputs
    SCALE += input->keyAxisDirection('-', '+') * 0.01 * rateModifier;

    if (input->keyTapped('.')) { SHOW_AXIS = !SHOW_AXIS; }

    if (input->keyTapped(',')) {
        switch (line) {
            case GL_POINT:
                line = GL_LINE;
                break;
            case GL_FILL:
                line = GL_POINT;
                break;
            default:
                line = GL_FILL;
                break;
        }
    }

    if (input->keyTapped('q')) { exit(0); }

    // Updates
    camera->update(input.get(), rateModifier);
    if (camera->currentType == FPS) {
        glutSetCursor(GLUT_CURSOR_NONE);
    } else {
        glutSetCursor(GLUT_CURSOR_INHERIT);
    }

    // Finalize
    input->updatePreviousState();
    input->resetMouseDelta();
    renderScene();
}

void render(int argc, char **argv, Config &config) {
    camera = std::move(config.camera);
    lights = config.lights;
    input = std::make_unique<InputState>();

    // put GLUT init here
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG-Engine");
    timeBase = glutGet(GLUT_ELAPSED_TIME);
    startTime = timeBase;

    // put callback registry here
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(update);

    // Callback registration for keyboard processing
    glutKeyboardFunc(keyPressHandler);
    glutSpecialFunc(specialKeyPressHandler);
    glutKeyboardUpFunc(keyReleaseHandler);
    glutSpecialUpFunc(specialKeyReleaseHandler);
    glutPassiveMotionFunc(passiveMouseHandler);

    // put GLEW and VBO init here
    glewInit();

    // some OpenGL settings
    glPolygonMode(GL_FRONT, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    if ((int)lights.size() == 0) {
        glEnable(GL_LIGHT0);
        std::shared_ptr<AmbientLight> light = std::make_shared<AmbientLight>(Vector3(1,1,1));
        lights.push_back(light);
    }
    else {
        for (int i = 0; i < (int)lights.size(); i++) {
            glEnable(0x4000 + i * 0x0001);
        }
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //Initialize VBOs
    initBuffers(config.modelTable);
    initTextures(config.textureNames);
    vboController = std::make_unique<VBOController>(config.models);

    // enter GLUTs main cycle
    glutMainLoop();
}

}  // namespace Render
