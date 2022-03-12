#include <cstdlib>
#include "engine/render.hpp"

#include "common/geometry.hpp"
#include "engine/camera.hpp"
#include "engine/input.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

namespace Render {

static Camera camera;
static std::vector<Model> models;
// FIXME delete
// static Model *model;
static InputState *input;

static float SCALE = 1;
static bool SHOW_AXIS = false;
static int line = GL_FILL;
static int windowWidth = 800;
static int windowHeight = 800;
static bool mouseWarping = false;
static int timebase;

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
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
    // set the viewport to be the entire window
    glViewport(0, 0, w, h);
}

void renderScene(void) {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // set camera
    glLoadIdentity();

    gluLookAt(camera.position.x, camera.position.y, camera.position.z, camera.look.x, camera.look.y, camera.look.z,
              camera.up.x, camera.up.y, camera.up.z);

    if (SHOW_AXIS) {
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
    }

    glScalef(SCALE, SCALE, SCALE);

    glPolygonMode(GL_FRONT_AND_BACK, line);

    glBegin(GL_TRIANGLES);

    for (Model model : models) {
        for (int f = 0; f < (int) model.indices.size(); f++) {
            float x = model.vertices[model.indices[f] * 3];
            float y = model.vertices[model.indices[f] * 3 + 1];
            float z = model.vertices[model.indices[f] * 3 + 2];
            glVertex3f(x, y, z);
        }
    }

    glEnd();

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
    if (camera.currentType == FPS) {
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
    float fps = 1000.0f / (time - timebase);
    timebase = time;
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
    camera.update(input, rateModifier);
    if (camera.currentType == FPS) {
        glutSetCursor(GLUT_CURSOR_NONE);
    } else {
        glutSetCursor(GLUT_CURSOR_INHERIT);
    }

    // Finalize
    input->updatePreviousState();
    input->resetMouseDelta();
    renderScene();
}

void render(int argc, char **argv, Config config) {
    camera = config.camera;
    // std::cout << camera.position.x << "\n"
    //           << camera.position.y << "\n"
    //           << camera.position.z << "\n"
    //           << camera.look.x << "\n"
    //           << camera.look.y << "\n"
    //           << camera.look.z << "\n"
    //           << camera.up.x << "\n"
    //           << camera.up.y << "\n"
    //           << camera.up.z << "\n";
    // camera = Camera();

    input = new InputState();
    // models.push_back(Model::generateCone(2, 5, 100, 100));
    models = config.models;

    // put GLUT init here
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG-Engine");
    timebase = glutGet(GLUT_ELAPSED_TIME);

    // some OpenGL settings
    glPolygonMode(GL_FRONT, GL_FILL);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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

    // enter GLUTs main cycle
    glutMainLoop();
}

}  // namespace Render
