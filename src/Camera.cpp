#include "Camera.h"


CameraOrthographic::CameraOrthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
    this->projection = glm::ortho(left, right, bottom, top, zNear, zFar);
}


// projection * view
const glm::mat4& CameraOrthographic::get_projection_view_matrix()
{
    return this->projection; // projection * view? No view matrix? 
}


// Camera Perspective -----------------------------------------------------------------------


CameraPerspective::CameraPerspective(glm::vec3 pos, glm::vec3 look_at, float fov, float width, float height)
{
    this->pos = pos;
    this->look_at = look_at;
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->projection = glm::perspectiveFov(fov, width, height, 0.01f, 100.0f);
}


// projection * view
const glm::mat4 CameraPerspective::get_projection_view_matrix()
{
    return this->projection * glm::lookAt(pos, look_at, up);
}


void CameraPerspective::move(glm::vec3 new_pos)
{
    this->pos = new_pos;
}