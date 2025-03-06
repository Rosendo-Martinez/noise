#include <vector>
#include <glm/vec2.hpp>


namespace Value_Noise_1D
{
    float sample(float x, float frequency = 1.0f);
    std::vector<glm::vec2> sample(int start, int end, int count);
}