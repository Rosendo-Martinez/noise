#include "Camera.h"


CameraOrthographic::CameraOrthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
    this->projection = glm::ortho(left, right, bottom, top, zNear, zFar);
}


const glm::mat4& CameraOrthographic::get()
{
    return this->projection;
}