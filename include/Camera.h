#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class CameraOrthographic
{
public:
    CameraOrthographic(float left, float right, float bottom, float top, float zNear, float zFar);

    const glm::mat4& get();

private:
    glm::mat4 projection;

};