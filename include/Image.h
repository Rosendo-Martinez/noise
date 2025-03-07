class Image_Grayscale
{
public:
    Image_Grayscale(int width, int height);
    ~Image_Grayscale();

    void setPixel(int x, int y, float value);
    void save(const char* filename);

    void print();

private:
    float** image;
    int width;
    int height;
};