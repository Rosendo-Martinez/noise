#include <fstream>
#include <cassert>
#include <iostream>
#include "Image.h"


// Structs -----------------------------


#pragma pack(1)
struct BMP_HEADER // 14 bytes
{
    short ID = 0x4D42;
    int SIZE_OF_FILE;
    int UNUSED = 0;
    int OFFSET_TO_DATA;
};


struct BITMAP_INFO_HEADER // 40 bytes
{
    int SIZE_OF_DIB;
    int WIDTH_PIXELS;
    int HEIGHT_PIXELS;
    short COLOR_PLANES = 1;
    short BITS_PER_PIXEL = 24;
    int COMPRESSION = 0;
    int SIZE_OF_RAW_DATA;
    int PPM_HORIZONTAL = 2835;
    int PPM_VERTICAL = 2835;
    int PALETTE_COLORS = 0; 
    int IMPORTANT_COLORS = 0;
};
#pragma pack()

// Helper functions ---------------------------------------------------------------------------------------------


// 24-bits per pixel, and RGB interleaved
// raw data should be defined from bottom left pixel to top right pixel
void create_bmp(const char* filename, const unsigned char* raw_data, long width, long height)
{
    assert(sizeof(short) == 2);
    assert(sizeof(int) == 4);
    assert(sizeof(char) == 1);
    assert(sizeof(BMP_HEADER) == 14);
    assert(sizeof(BITMAP_INFO_HEADER) == 40);


    std::ofstream outfile;
    outfile.open(filename, std::ios::binary | std::ios::trunc | std::ios::out);


    int padding_bytes = 4 - ((width * 3) % 4);
    if (padding_bytes == 4)
    {
        // Already multiple of 4 bytes, so no padding needed
        padding_bytes = 0;
    }
    int SIZE_OF_RAW_DATA = (3 * width * height) + (padding_bytes * height);
    int SIZE_OF_DIB = sizeof(BITMAP_INFO_HEADER);
    int SIZE_OF_HEADERS = sizeof(BMP_HEADER) + sizeof(BITMAP_INFO_HEADER);
    int SIZE_OF_FILE = SIZE_OF_HEADERS + SIZE_OF_RAW_DATA;


    BMP_HEADER header;
    header.SIZE_OF_FILE = SIZE_OF_FILE;
    header.OFFSET_TO_DATA = SIZE_OF_HEADERS;

    BITMAP_INFO_HEADER bitmap_info_header;
    bitmap_info_header.SIZE_OF_DIB = SIZE_OF_DIB;
    bitmap_info_header.WIDTH_PIXELS = width;
    bitmap_info_header.HEIGHT_PIXELS = height;
    bitmap_info_header.SIZE_OF_RAW_DATA = SIZE_OF_RAW_DATA;

    
    outfile.write(((char*) &header), sizeof(BMP_HEADER));
    outfile.write(((char*) &bitmap_info_header), sizeof(BITMAP_INFO_HEADER));


    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int i = row * (width * 3) + col * 3;

            // write pixel to file in little-endian (BGR)
            unsigned char bgr[3];
            bgr[0] = raw_data[i + 2];
            bgr[1] = raw_data[i + 1];
            bgr[2] = raw_data[i];

            outfile.write(((char*) bgr), 3);
        }

        // pad row so its multiple of 4 bytes
        for (int j = 0; j < padding_bytes; j++)
        {
            char zero = 0;
            outfile.write(&zero, 1);
        }
    }

    outfile.close();
}


// [0,1] --> [0,255]
unsigned char grayscale_to_rgb(float gray)
{
    // 0.9961 * 255.999f ~= 255.01, so values greater then 0.9961 will get mapped to 255
    // instead of just doing 1.0 * 255 = 255, which only maps 1.0f to 255!

    int result = int(gray * 255.999f);
    assert(result >= 0 && result <= 255);

    return result;
}


// Image Grayscale ---------------------------------------------------------------------------------------


Image_Grayscale::Image_Grayscale(int width, int height)
    : width(width), height(height), image(nullptr)
{
    image = new float*[height];

    for (int row = 0; row < height; row++)
    {
        image[row] = new float[width];

        for (int col = 0; col < width; col++)
        {
            image[row][col] = 0.0f;
        }
    }
}


Image_Grayscale::~Image_Grayscale()
{
    for (int row = 0; row < height; row++)
    {
        delete[] image[row];
    };

    delete[] image;
};


// (0,0) is bottom left
// value can be non-normalized (i.e out of [0,1])
// will be automatically normalized before saving
void Image_Grayscale::setPixel(int x, int y, float value)
{
    assert(x < width && x >= 0);
    assert(y < height && y >= 0);

    image[y][x] = value;
}


void Image_Grayscale::save(const char* filename)
{
    normalize();

    unsigned char* raw_data = new unsigned char[width * height * 3];

    int i = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned char result = grayscale_to_rgb(image[y][x]);
            assert(result >= ((unsigned char) 0) && result <= ((unsigned char) 255));

            raw_data[i] = result; // R
            i++;
            raw_data[i] = result; // G
            i++;
            raw_data[i] = result; // B
            i++;
        }
    }

    create_bmp(filename, raw_data, width, height);
    delete[] raw_data;
}


// normalized to [0,1]
void Image_Grayscale::normalize()
{
    float min = image[0][0];
    float max = image[0][0];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (image[y][x] < min)
            {
                min = image[y][x];
            }

            if (image[y][x] > max)
            {
                max = image[y][x];
            }
        }
    }

    assert(min < max);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // NOTE: could optimize by precomputing 1/interval, and moving it out of
            //       double for loop
            float value = (image[y][x] - min) / (max - min);
            assert(value >= 0.0f && value <= 1.0f);
            image[y][x] = value;
        }
    }
}


// for debugging
void Image_Grayscale::print()
{
    std::cout << "Image\n";
    std::cout << "width: " << width << '\n';
    std::cout << "height: " << height << '\n';
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            std::cout << "(row, col) = " << row << ',' << col << " value = " << image[row][col] << '\n';
        }
    }
}


float Image_Grayscale::getPixel(int x, int y)
{
    return image[y][x];
}


// Main ---------------------------------------------------------------------------------------------------------------


// int main()
// {
//     float WHITE = 1.0f;
//     float BLACK = 0.0f;
//     int WIDTH = 100;
//     int HEIGHT = 100;

//     const char* FILE_NAME = "./my_bmp.bmp";

//     Image_Grayscale image(WIDTH, HEIGHT);

//     for (int x = 0; x < WIDTH; x++)
//     {
//         for (int y = 0; y < HEIGHT; y++)
//         {
//             if (y % 2 == 0)
//             {
//                 if (x % 2 == 0) 
//                 {
//                     image.setPixel(x,y, BLACK);
//                 }
//                 else
//                 {
//                     image.setPixel(x,y,WHITE);
//                 }
//             }
//             else
//             {
//                 if (x % 2 == 0) 
//                 {
//                     image.setPixel(x,y,WHITE);
//                 }
//                 else
//                 {
//                     image.setPixel(x,y, BLACK);
//                 }
//             }
//         }
//     }

//     image.save(FILE_NAME);
// }