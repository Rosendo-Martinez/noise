class Image_Grayscale
{
public:
    Image_Grayscale(int width, int height);
    ~Image_Grayscale();

    float getPixel(int x, int y);
    void  setPixel(int x, int y, float value);
    void  save(const char* filename);
    void  normalize();

    void print();

private:
    float** image;
    int width;
    int height;
};