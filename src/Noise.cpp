#include "Noise.h"
#include <cassert>
#include <cmath>
#include <iostream>


struct Cubic
{
    float v0, v1, v2, v3;

    Cubic(float v0, float v1, float v2, float v3) : v0(v0), v1(v1), v2(v2), v3(v3) {}
};


// Helper functions ---------------------------------------------------------------------------------------


void split_float(float x, int& x_int, float& x_dec)
{
    double x_int_double;
    double x_dec_double = modf(x, &x_int_double);
    
    x_int = (int) x_int_double;
    x_dec = (float) x_dec_double;
}


float hermite_interpolate(float l, float r, float dx)
{
    return (2.0f * pow(dx,3) - 3.0f * pow(dx,2) + 1.0f) * l + (-2.0f * pow(dx,3) + 3.0f * pow(dx,2)) * r;
}


float cubic_interpolate(const Cubic& cubic, float x)
{
    // CREDIT: Hugo Elias
    float P = (cubic.v3 - cubic.v2) - (cubic.v0 - cubic.v1);
    float Q = (cubic.v0 - cubic.v1) - P;
    float R = cubic.v2 - cubic.v0;
    float S = cubic.v1;

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


// Cubic used to interpolate at x's fractional part.
Cubic get_cubic(int x_int_part)
{
    return Cubic(random_float(x_int_part - 1), random_float(x_int_part), random_float(x_int_part + 1), random_float(x_int_part + 2));
}


float octave(float x, bool isValue)
{
    int octaves = 6;
    int frequency = 2;
    float amplitude = 0.5f;

    float total = 0;
    for (int i = 0; i < octaves; i++)
    {
        if (isValue)
        {
            total += Noise1D::sample_value(x * pow(frequency, i)) * pow(amplitude, i);
        }
        else // perlin
        {
            total += Noise1D::sample_perlin(x * pow(frequency, i)) * pow(amplitude, i);
        }
    }

    return total;
}


// Value Noise 1D ----------------------------------------------------------------------------------------


float Noise1D::sample_value(float x)
{
    int x_int;
    float x_dec;
    split_float(x, x_int, x_dec);

    return cubic_interpolate(get_cubic(x_int), x_dec);
}


// Samples noise over interval [start, end] a count times.
std::vector<glm::vec2> Noise1D::sample(int start, int end, int count)
{
    // WARNING: this shit is too complicated, maybe do something about it?
    // WHY: the point at which we sample can be thought of as a int and a fractional part
    //      we can split the to up into a int and a float, instead of keeping them together in one float
    //      this should improve accuracy. It prevents mixing big floats with small floats (dx).
    //      Never mix big floats and small floats! EH, what do I know, it works atleast!

    // TO DECIDE: DELETE? might delete this later. Its just too complicated, even though it seems to work.
    //            the other shit is much easier to work with and understand, not this.
    //            Yes it might be more optimized but its not worth the complexity.

    std::vector<glm::vec2> list (count);

    float dx = ((float) end - start) / ((float) (count - 1));
    int i = 0;
    int x_int_part = start;
    float x_dec_part = 0;

    while (i < count)
    {
        // Interpolated cubic
        Cubic cubic = get_cubic(x_int_part);

        while (x_dec_part < 1.0f && i < count)
        {
            list[i] = glm::vec2(((float) x_int_part) + x_dec_part, cubic_interpolate(cubic, x_dec_part));

            x_dec_part += dx;
            i++;
        }

        x_dec_part = x_dec_part - 1.0f; // left over part
        x_int_part++;
    }

    return list;
}


float Noise1D::sample_value_octave(float x)
{
    return octave(x, true);
}


float Noise1D::sample_perlin(float x)
{
    int x_int;
    float x_dec;
    split_float(x, x_int, x_dec);

    float slope_left = random_float(x_int);
    float slope_right = random_float(x_int + 1);

    float y_left = slope_left * x_dec;
    float y_right = slope_right * (x_dec - 1.0f);

    float amplitude = 2.0f;
    return hermite_interpolate(y_left, y_right, x_dec) * amplitude;
}


float Noise1D::sample_perlin_octave(float x)
{
    return octave(x, false);
}