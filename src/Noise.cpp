#include "Noise.h"
#include <cassert>
#include <cmath>
#include <iostream>


#define PI 3.14159265


enum class Interpolate
{
    Linear, Cubic, Hermite
};


struct Cubic
{
    float v0, v1, v2, v3;

    Cubic(float v0, float v1, float v2, float v3) : v0(v0), v1(v1), v2(v2), v3(v3) {}
};


struct Vector
{
    float x, y;

    Vector() : x(0.0f), y(0.0f) {}
    Vector(float x, float y) : x(x), y(y) {}
};


// Helper functions ---------------------------------------------------------------------------------------


void split_float(float x, int& x_int, float& x_dec)
{
    double x_int_double;
    double x_dec_double = modf(x, &x_int_double);
    
    x_int = (int) x_int_double;
    x_dec = (float) x_dec_double;
}


float linear_interpolate(float from, float to, float dx)
{
    assert(dx >= 0.0f && dx <= 1.0f);

    return ((1 - dx) * from) + (dx * to);
}


float bilinear_interpolate(float bottom_left, float bottom_right, float top_left, float top_right, float dx, float dy)
{
    float bottom = linear_interpolate(bottom_left,bottom_right,dx);
    float top = linear_interpolate(top_left,top_right,dx);

    return linear_interpolate(bottom, top, dy);
}


float hermite_interpolate(float from, float to, float dx)
{
    assert(dx >= 0.0f && dx <= 1.0f);

    return (2.0f * pow(dx,3) - 3.0f * pow(dx,2) + 1.0f) * from + (-2.0f * pow(dx,3) + 3.0f * pow(dx,2)) * to;
}


float cubic_interpolate(const Cubic& cubic, float x)
{
    assert(x >= 0.0f && x <= 1.0f);

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


// [-1.0, 1.0]
float random_float(int x, int y) {
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}


Vector random_gradient(int x, int y)
{
    float theta = random_float(x,y) * PI;
    assert(theta >= -PI && theta <= PI);

    Vector gradient;
    gradient.x = cos(theta);
    gradient.y = sin(theta);

    double length = sqrt(pow(gradient.x, 2) + pow(gradient.y, 2));
    assert(1 - length < 0.0001 && 1 - length > -0.0001);

    return gradient;
}


float dot(const Vector& v1, const Vector& v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y);
}


// v1 - v2
Vector subtract(const Vector& v1, const Vector& v2)
{
    return Vector(v1.x - v2.x, v1.y - v2.y);
}


// Cubic used to interpolate at x's fractional part.
Cubic get_cubic(int x_int_part)
{
    return Cubic(random_float(x_int_part - 1), random_float(x_int_part), random_float(x_int_part + 1), random_float(x_int_part + 2));
}


Cubic get_horizontal_cubic(int x, int y)
{
    return Cubic(random_float(x - 1, y), random_float(x, y), random_float(x + 1, y), random_float(x + 2, y));
}


float octave(float x, bool isValue, Interpolate interpolate_type)
{
    int octaves = 6;
    int frequency = 2;
    float amplitude = 0.5f;

    float total = 0;
    for (int i = 0; i < octaves; i++)
    {
        if (isValue) // value noise
        {
            if (interpolate_type == Interpolate::Cubic)
            {
                total += Noise1D::sample_value_cubic(x * pow(frequency, i)) * pow(amplitude, i);
            }
            else
            {
                total += Noise1D::sample_value_linear(x * pow(frequency, i)) * pow(amplitude, i);
            }
        }
        else // perlin / gradient noise
        {
            total += Noise1D::sample_perlin_hermite(x * pow(frequency, i)) * pow(amplitude, i);
        }
    }

    return total;
}


// Noise 1D ----------------------------------------------------------------------------------------


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


float Noise1D::sample_value_cubic(float x)
{
    int x_int;
    float x_dec;
    split_float(x, x_int, x_dec);

    return cubic_interpolate(get_cubic(x_int), x_dec);
}


float Noise1D::sample_value_octave_cubic(float x)
{
    return octave(x, true, Interpolate::Cubic);
}


float Noise1D::sample_perlin_hermite(float x)
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


float Noise1D::sample_perlin_octave_hermite(float x)
{
    return octave(x, false, Interpolate::Hermite);
}


float Noise1D::sample_value_linear(float x)
{
    int x_int;
    float x_dec;
    split_float(x, x_int, x_dec);

    float left = random_float(x_int);
    float right = random_float(x_int + 1);

    return linear_interpolate(left, right, x_dec);  
}


float Noise1D::sample_value_octave_linear(float x)
{
    return octave(x, true, Interpolate::Linear);
}


// Noise 2D -------------------------------------------------------------------------


float Noise2D::sample_value_bilinear(float x, float y)
{
    int x_int, y_int;
    float x_dec, y_dec;
    split_float(x, x_int, x_dec);
    split_float(y, y_int, y_dec);

    return bilinear_interpolate(
        random_float(x_int, y_int), 
        random_float(x_int + 1, y_int),
        random_float(x_int, y_int + 1), 
        random_float(x_int + 1, y_int + 1),
        x_dec, y_dec
    );
}


float Noise2D::sample_value_bicubic(float x, float y)
{
    int x_int, y_int;
    float x_dec, y_dec;
    split_float(x, x_int, x_dec);
    split_float(y, y_int, y_dec);

    Cubic c0 = get_horizontal_cubic(x, y - 1);
    Cubic c1 = get_horizontal_cubic(x, y);
    Cubic c2 = get_horizontal_cubic(x, y + 1);
    Cubic c3 = get_horizontal_cubic(x, y+ 2);

    float v0 = cubic_interpolate(c0, x_dec);
    float v1 = cubic_interpolate(c1, x_dec);
    float v2 = cubic_interpolate(c2, x_dec);
    float v3 = cubic_interpolate(c3, x_dec);

    Cubic vertical_cubic (v0, v1, v2, v3);

    return cubic_interpolate(vertical_cubic, y_dec);
}


float Noise2D::sample_perlin_hermite(float x, float y)
{
    int x_int, y_int;
    float x_dec, y_dec;
    split_float(x, x_int, x_dec);
    split_float(y, y_int, y_dec);

    Vector g_tl = random_gradient(x_int, y_int + 1);
    Vector g_tr = random_gradient(x_int + 1, y_int + 1);
    Vector g_br = random_gradient(x_int + 1, y_int);
    Vector g_bl = random_gradient(x_int, y_int);

    Vector dx (x_dec, y_dec);
    Vector dx_tl = subtract(dx, Vector(0.0f, 1.0f));
    Vector dx_tr = subtract(dx, Vector(1.0f, 1.0f));
    Vector dx_br = subtract(dx, Vector(1.0f, 0.0f));
    Vector dx_bl = dx;

    float dot_tl = dot(g_tl, dx_tl);
    float dot_tr = dot(g_tr, dx_tr);
    float dot_br = dot(g_br, dx_br);
    float dot_bl = dot(g_bl, dx_bl);

    return hermite_interpolate(hermite_interpolate(dot_bl, dot_br, x_dec), hermite_interpolate(dot_tl, dot_tr, x_dec), y_dec);
}