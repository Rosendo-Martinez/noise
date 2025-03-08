#include "Noise.h"
#include "Image.h"


int main()
{
    int WIDTH = 50;
    int HEIGHT = 50;
    Image_Grayscale image(WIDTH, HEIGHT);

    int SAMPLE_INTERVAL_WIDTH = 1;
    int SAMPLE_INTERVAL_HEIGHT = 1;


    float dx = ((float) SAMPLE_INTERVAL_WIDTH) / ((float) WIDTH);
    float dy = ((float) SAMPLE_INTERVAL_HEIGHT) / ((float) HEIGHT);


    float pixel_00_x = dx * 0.5f;
    float pixel_00_y = dy * 0.5f;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            float pixel_x = pixel_00_x + (x * dx);
            float pixel_y = pixel_00_y + (y * dy);

            image.setPixel(x,y, Noise2D::sample_value_bilinear(pixel_x, pixel_y));
        }
    }

    image.save("noise.bmp");
}