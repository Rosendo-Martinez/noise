#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class CameraOrthographic
{
public:
    CameraOrthographic(float left, float right, float bottom, float top, float zNear, float zFar);

    const glm::mat4& get_projection_view_matrix();

private:
    glm::mat4 projection;

};


class CameraPerspective
{
public:
    CameraPerspective(glm::vec3 pos, glm::vec3 look_at, float fov, float width, float height);

    const glm::mat4 get_projection_view_matrix();
    void move(glm::vec3 new_pos);

private:
    glm::vec3 pos;
    glm::vec3 look_at;
    glm::vec3 up;
    glm::mat4 projection;
};