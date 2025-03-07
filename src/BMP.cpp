#include <fstream>
#include <cassert>
#include <iostream>
#include "BMP.h"

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

// Function ---------------------------------------------------------------------------------------------


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


// int main()
// {
//     const unsigned char raw_data[] = {
//         0x0, 0xFF, 0x0, 0xFF, 0xFF, 0xFF,
//         0xFF, 0x0, 0x0, 0x0, 0x0, 0xFF,
//     };

//     create_bmp("my_first_bmp.bmp", raw_data, 2, 2);
// }