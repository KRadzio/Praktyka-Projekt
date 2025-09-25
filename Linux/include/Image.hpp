#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <filesystem>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Renderer.hpp"

#define MAX_VAL 256
#define BLACK 0
#define WHITE 255


class Image
{
public:
    struct Pixel
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        int brightnes; // set auto
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
    Image(const Image &other);
    Image operator=(const Image &other);
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
    inline int GetPixelCount() { return width * height; }

    inline std::filesystem::path GetImagePath() { return filePath; }
    inline std::string GetExtension() { return filePath.extension(); }

    void CopyBrightnessHistogram(float *dst);
    void CopyNormalisedBrightnessHistogram(float *dst);

    void SaveImage();
    void SaveImageAs(std::filesystem::path path);
    void SaveImageAs(std::filesystem::path dirPath, char *filename, int extension);
    int SetSourceImage(std::filesystem::path path);
    void ClearImage();

    inline bool NoSurface() { return surface == nullptr; }
    inline bool NoTexture() { return texture == nullptr; }

    void RefreshPixelValuesArrays();
    void RefreshTexture();

    Pixel GetPixel(int x, int y);
    void SetPixel(int x, int y, Pixel pix);

    Pixel GetPixelNoLock(int x, int y);
    void SetPixelNoLock(int x, int y, Pixel pix);

    void SetPixelWhite(int x, int y);
    void SetPixelWhiteNoLock(int x, int y);

    void SetPixelBlack(int x, int y);
    void SetPixelBlackNoLock(int x, int y);

    void SetPixelByBrightness(int x, int y, uint8_t bright);

    // unused
    inline void LockImage() { SDL_LockSurface(surface); }
    // unused
    inline void UnlockImage() { SDL_UnlockSurface(surface); }

    void Copy(Image &other);

    // use only if pixels and size info is needed
    void CopyOnlySurfaceAndSize(Image &other);

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
    std::filesystem::path filePath = "";
};

#endif
