#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class CuboidRenderer
{
public:
    CuboidRenderer();

    void setProjectionView(const glm::mat4& projection_view);
    void draw(const glm::vec3& color, const glm::vec3& translate, const glm::vec3& scale);

private:
    unsigned int VAO;
    Shader shader;

    void init();
};