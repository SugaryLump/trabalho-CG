#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "common/shapes.hpp"
#include "engine/input.hpp"

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

    Camera() : near(1), far(1000), fov(90) { 
      posInitialCamera(); 
      }
    Camera(float near, float far, float fov) : near(near), far(far), fov(fov) { posInitialCamera(); }
    void posInitialCamera();
    void changeTypeCamera(CameraType newType);
    void setFollowPoint(Vector3 point);
    void setNextTypeCamera();
    void update(InputState* input);

    // FOLLOW
    void updateFollow(InputState* input);
    void followMoveUp();
    void followMoveDown();
    void followMoveLefth();
    void followMoveRight();
    void followMoveIn();
    void followMoveOut();
    void followProcessSpecialKeys(int key, int _x, int _y);

    // FPS
    void updateFPS(InputState* input);
};

#endif  // CAMERA_HPP_
