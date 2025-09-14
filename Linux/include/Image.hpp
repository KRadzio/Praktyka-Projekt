#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Renderer.hpp"

#define MAX_VAL 256

class Image
{
public:
    struct Pixel
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
    };

    enum Extension
    {
        UNKNOWN = -1,
        PNG,
        JPG,
        BMP
    };

public:
    Image();
    // the texture is not copied so use RefreshTexture after this one
    // IT CHANGES THE FILEPATH
    Image &operator=(const Image &other);
    Image(std::string filename);
    ~Image();

public:
    inline SDL_Surface *GetSurface() { return surface; }
    inline SDL_Texture *GetTexture() { return texture; }
    inline int GetWidth() { return width; }
    inline int GetHeight() { return height; }
    inline float *GetLightValues() { return lightValues; }
    inline float *GetRValues() { return valuesR; }
    inline float *GetGValues() { return valuesG; }
    inline float *GetBValues() { return valuesB; }
    inline float *GetDistributor() { return distributorLight; }
    inline float *GetDistributorR() { return distributorR; }
    inline float *GetDistributorG() { return distributorG; }
    inline float *GetDistributorB() { return distributorB; }

    inline std::string GetImageName() { return sourceImageName; }
    std::string GetExtension();

    void SaveImage();
    void SaveImageAs(std::string filename);
    void SaveImageAs(std::string path, char *filename, int extension);
    int SetSourceImage(std::string filename);
    void ClearImage();

    inline bool NoSurface() { return surface == nullptr; }
    inline bool NoTexture() { return texture == nullptr; }

    void RefreshPixelValuesArrays();
    void RefreshTexture();

    Pixel GetPixel(int x, int y);
    void SetPixel(int x, int y, Pixel pix);

    Pixel GetPixelNoLock(int x, int y);
    void SetPixelNoLock(int x, int y, Pixel pix);


    // unused
    inline void LockImage() { SDL_LockSurface(surface); }
    // unused
    inline void UnlockImage() { SDL_UnlockSurface(surface); }

    void Copy(Image &other);

private:
    SDL_Surface *surface = nullptr;
    SDL_Texture *texture = nullptr;
    int width = 0;
    int height = 0;
    float lightValues[MAX_VAL];
    float valuesR[MAX_VAL];
    float valuesG[MAX_VAL];
    float valuesB[MAX_VAL];
    float distributorLight[MAX_VAL];
    float distributorR[MAX_VAL];
    float distributorG[MAX_VAL];
    float distributorB[MAX_VAL];
    std::string sourceImageName = "";
    Extension ext = UNKNOWN;
};

#endif
