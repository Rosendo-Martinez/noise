#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class CuboidRenderer
{
public:
    CuboidRenderer();

    void setProjectionView(const glm::mat4& projection_view);
    void setDirectionLight(const glm::vec3& direction_to_light);
    void draw(const glm::vec3& color, const glm::vec3& translate, const glm::vec3& scale);

    void update_instances(const glm::vec3* color, const glm::vec3* translate, const glm::vec3* scale, int size);
    void draw_instances();

private:
    Shader shader;
    unsigned int vao;
    int instances;
    unsigned int color_vbo;
    unsigned int translate_vbo;
    unsigned int scale_vbo;

    void init();
};