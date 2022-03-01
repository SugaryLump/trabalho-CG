#ifndef CAMERA_HPP_
#define CAMERA_HPP_

class Camera {

private:
  float positionX, positionY, positionZ;
  float upX, upY, upZ;
  float lookX, lookY, lookZ;

public:
  Camera();
  float getXPosition();
  float getYPosition();
  float getZPosition();
  float getXLook();
  float getYLook();
  float getZLook();
  float getXUp();
  float getYUp();
  float getZUp();
  void posInitialCamera();
};

#endif // CAMERA_HPP_
