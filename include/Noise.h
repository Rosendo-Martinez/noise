#include <vector>
#include <glm/vec2.hpp>


namespace Noise1D
{
    std::vector<glm::vec2> sample(int start, int end, int count);

    float sample_value_linear(float x);
    float sample_value_cubic(float x);
    float sample_perlin_hermite(float x);
    
    float sample_value_octave_linear(float x);
    float sample_value_octave_cubic(float x);
    float sample_perlin_octave_hermite(float x);
}

namespace Noise2D
{
    float sample_value_bilinear(float x, float y);
    float sample_value_bicubic(float x, float y);
    float sample_perlin_hermite(float x, float y);
}