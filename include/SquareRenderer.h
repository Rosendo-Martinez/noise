#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class SquareRenderer
{
private:
    unsigned int VAO;
    Shader shader;

    void init();
public:
    SquareRenderer();

    void setProjection(const glm::mat4& projection);
    void draw(const glm::vec3& center, const glm::vec2& scale, const glm::vec3& normal, float rotation, const glm::vec3& color);
};