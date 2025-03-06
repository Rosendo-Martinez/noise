#include <vector>
#include <glm/vec2.hpp>


namespace Noise1D
{
    std::vector<glm::vec2> sample(int start, int end, int count);

    float sample_value(float x);
    float sample_perlin(float x);
    float sample_value_octave(float x);
    float sample_perlin_octave(float x);
}