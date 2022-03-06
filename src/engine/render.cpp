#include "engine/render.hpp"

#include "engine/camera.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

namespace Render {

static Camera *camera;

static float SCALE = 1;
static bool SHOW_AXIS = false;
static int line = GL_FILL;

void changeSize(int w, int h) {
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

    gluLookAt(camera->position.x, camera->position.y, camera->position.z, camera->look.x, camera->look.y,
              camera->look.z, camera->up.x, camera->up.y, camera->up.z);

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

    glColor3f(0.2f, 0.4f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glColor3f(0.2f, 0.4f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glColor3f(0.2f, 0.44f, 0.54f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);

    glColor3f(0.2f, 0.44f, 0.8f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

    glColor3f(0.2f, 0.44f, 0.54f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);

    glColor3f(0.2f, 0.44f, 0.8f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);

    glEnd();

    // End of frame
    glutSwapBuffers();
}

void renderKeybindings(unsigned char key, int _x, int _y) {
    switch (key) {
        case 'f':
            camera->setNextTypeCamera();
            break;

        case '+':
            SCALE += 0.1;
            break;
        case '-':
            SCALE -= 0.1;
            break;

        case '.':
            SHOW_AXIS = !SHOW_AXIS;
            break;

        case ',':
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
            break;

        case 'q':
            exit(0);
    }
}

void processSpecialKeys(int key, int _x, int _y) {
    if (key < 112) { camera->specialKeysState[key] = 1; }
    switch (key) {
        default:
            switch (camera->currentType) {
                case FOLLOW:
                    camera->followProcessSpecialKeys(key, _x, _y);
                    break;
                case FPS:
                    camera->fpsProcessSpecialKeys(key, _x, _y);
                    break;
            }
            break;
    }
}

void keyboardReleaseHandler(unsigned char key, int x, int y) {
    camera->keysState[key] = 0;
}

void specialKeyboardPressHandler(int key, int x, int y) {
    if (key < 112) { camera->specialKeysState[key] = 1; }
}

void specialKeyboardReleaseHandler(int key, int x, int y) {
    if (key < 112) { camera->specialKeysState[key] = 0; }
}

void passiveMouseHandler(int x, int y) {
    if (!camera->pointerWarp) {
        camera->mouseDeltaX = x - 540;
        camera->mouseDeltaY = y - 500;
        camera->pointerWarp = true;
        glutWarpPointer(540, 500);
    } else {
        camera->pointerWarp = false;
    }
}

void render(int argc, char **argv) {
    camera = new Camera();

    // put GLUT init here
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG-Engine");

    // some OpenGL settings
    glPolygonMode(GL_FRONT, GL_FILL);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // put callback registry here
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    // Callback registration for keyboard processing
    glutKeyboardFunc(renderKeybindings);
    glutSpecialFunc(processSpecialKeys);

    glutKeyboardUpFunc(keyboardReleaseHandler);
    glutSpecialUpFunc(specialKeyboardReleaseHandler);
    glutPassiveMotionFunc(passiveMouseHandler);

    // enter GLUTs main cycle
    glutMainLoop();
}

}  // namespace Render
