#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "common/geometry.hpp"
#include "engine/input.hpp"

enum CameraType {
    FPS,
    FOLLOW,
};

struct Camera {
   private:
    void normalizeAlphaBeta();
    float near, far, fov;

   public:
    CameraType currentType;

    Vector3 position;
    Vector3 up;
    Vector3 look;
    float alpha, beta;
    float radius;

    Camera() { posInitialCamera(); }
    Camera(float near, float far, float fov);
    Camera(Vector3 newLook, Vector3 newUp, Vector3 newPosition, float near, float far, float fov);
    float getNear();
    float getFar();
    float getFov();
    void setup(Vector3 newLook, Vector3 newUp, Vector3 newPosition);
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
