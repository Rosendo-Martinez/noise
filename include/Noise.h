#include <vector>
#include <glm/vec2.hpp>


namespace Value_Noise_1D
{
    float sample(float x);
    std::vector<glm::vec2> sample(int start, int end, int count);
    float sample_octave(float x);
    float sample_perlin(float x);
}