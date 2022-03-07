#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "common/shapes.hpp"

enum CameraType {
    FPS,
    FOLLOW,
};

struct Camera {
   private:
    void normalizeAlphaBeta();

   public:
    CameraType currentType;

    const float near, far, fov;

    Vector3 position;
    Vector3 up;
    Vector3 look;
    float alpha, beta;
    float radius;

    short previousKeysState[256];
    short keysState[256];
    short specialKeysState[112];
    int prevMouseX;
    int prevMouseY;
    int mouseDeltaX;
    int mouseDeltaY;
    bool pointerWarp;

    Camera() : near(1), far(1000), fov(90) { 
      posInitialCamera(); 
      }
    Camera(float near, float far, float fov) : near(near), far(far), fov(fov) { posInitialCamera(); }
    void posInitialCamera();
    void changeTypeCamera(CameraType newType);
    void setFollowPoint(Vector3 point);

    void setNextTypeCamera();

    // FOLLOW
    void updateFollow();
    void followMoveUp();
    void followMoveDown();
    void followMoveLefth();
    void followMoveRight();
    void followMoveIn();
    void followMoveOut();
    void followProcessSpecialKeys(int key, int _x, int _y);

    // FPS
    void fpsProcessSpecialKeys(int key, int _x, int _y);
};

#endif  // CAMERA_HPP_
