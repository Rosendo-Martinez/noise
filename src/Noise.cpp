#include "Noise.h"
#include <cassert>
#include <cmath>
#include <iostream>


// Helper functions ---------------------------------------------------------------------------------------


float Cubic_Interpolate(float v0, float v1, float v2, float v3, float x)
{
    // CREDIT: Hugo Elias
    float P = (v3 - v2) - (v0 - v1);
    float Q = (v0 - v1) - P;
    float R = v2 - v0;
    float S = v1;

    return P * (x * x * x) + Q * (x * x) + R * x + S;
}


// [-1.0, 1.0]
float random_float(int x) 
{
    // TODO: seed should not be hardcoded

    // CREDIT: Hugo Elias
    x = (x << 13) ^ x;
    return (1.0f - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7FFFFFFF) / 1073741824.0f);
}


// Value Noise 1D ----------------------------------------------------------------------------------------


Value_Noise_1D::Value_Noise_1D() {}


float Value_Noise_1D::sample(float x)
{
    // PROBLEMS: x_fraction reduces accuracy as it gets bigger,
    //           waste of resources for user to iterate dx, by dx
    //           calling this function at each dx,
    // TODO: fix this in-efficient shit.
    // SOLUTION: retrun sample points on the interval [floor(x), floor(x) + 1]

    double x_intpart_double;
    double x_fraction = modf(x, &x_intpart_double);
    int x_intpart_int = (int) x_intpart_double;

    float v0 = random_float(x_intpart_int - 1);
    float v1 = random_float(x_intpart_int);
    float v2 = random_float(x_intpart_int + 1);
    float v3 = random_float(x_intpart_int + 2);

    return Cubic_Interpolate(v0, v1, v2, v3, x_fraction);
}


// Samples noise over interval [start, end] a count times.
std::vector<glm::vec2> Value_Noise_1D::sample(int start, int end, int count)
{
    // WARNING: this shit is too complicated, maybe do something about it?
    // WHY: the point at which we sample can be thought of as a int and a fractional part
    //      we can split the to up into a int and a float, instead of keeping them together in one float
    //      this should improve accuracy. It prevents mixing big floats with small floats (dx).
    //      Never mix big floats and small floats! EH, what do I know, it works atleast!

    assert(end > start);
    assert(count > 0);

    int length = end - start;
    float dx = ((float) length) / ((float) (count - 1));

    std::vector<glm::vec2> list (count);

    int i = 0;
    int x_int_part = start;
    float x_dec_part = 0;

    while (i < count)
    {
        float v0 = random_float(x_int_part - 1);
        float v1 = random_float(x_int_part);
        float v2 = random_float(x_int_part + 1);
        float v3 = random_float(x_int_part + 2);

        while (x_dec_part < 1.0f && i < count)
        {
            list[i] = glm::vec2(((float) x_int_part) + x_dec_part, Cubic_Interpolate(v0, v1, v2, v3, x_dec_part));

            x_dec_part += dx;
            i++;
        }

        x_dec_part = x_dec_part - 1.0f; // left over part
        x_int_part++;
    }

    assert(i == count);
    return list;
}