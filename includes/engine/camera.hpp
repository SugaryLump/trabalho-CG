#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "common/shapes.hpp"

enum CameraType {
    FREE,
    FOLLOW,
};

struct Camera {
   public:
    CameraType currentType;
    float positionX, positionY, positionZ;
    float upX, upY, upZ;
    float lookX, lookY, lookZ;
    float alpha, beta;
    float radius;
    Camera();
    // float getXPosition();
    // float getYPosition();
    // float getZPosition();
    // float getXLook();
    // float getYLook();
    // float getZLook();
    // float getXUp();
    // float getYUp();
    // float getZUp();
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
};

#endif  // CAMERA_HPP_
