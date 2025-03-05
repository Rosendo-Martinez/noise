#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class LineRenderer
{
public:
    LineRenderer();

    void setProjection(const glm::mat4& projection);
    void draw(const glm::vec3& color, const glm::vec3& start, const glm::vec3& end, float width);

private:
    unsigned int VAO;
    Shader shader;
    
    void init();
};