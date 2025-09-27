#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <filesystem>

#include <SDL.h>
#include <SDL_image.h>

#include "Renderer.hpp"

#define MAX_VAL 256 // the pixels can have a brightness in range (0,255)
#define BLACK 0     // brightness = 0 far r,g,b
#define WHITE 255   // brightness = 255 for r,g,b

// can have multiple instaces
class Image
{
public:
    struct Pixel
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        int brightnes; // auto set when using GetPixel methods
    };

    enum Extension
    {
        UNKNOWN = -1, // default
        PNG,
        JPG,
        BMP
    };

public:
    // create a new image with no surface and texture
    // arrays set to 0
    // width and height set to 0
    // filepath set to ""
    Image();
    // clears prev data
    // copy content and make a new texture
    // sets the same filepath as the other
    // (no checks if texture created)
    Image(const Image &other);
    // clears prev data
    // copy content and make a new texture
    // sets the same filepath as the other
    // (no checks if texture created)
    Image operator=(const Image &other);
    // clears prev data
    // create a new image and fill
    // data based on image
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
    inline std::string GetExtension() { return filePath.extension().string(); }

    // copies the brightness histogram to dst
    // dst == float[256]
    void CopyBrightnessHistogram(float *dst);
    // copies noralised brightness histogram to dst
    // dst == float[256]
    void CopyNormalisedBrightnessHistogram(float *dst);

    // save image as the current filename
    void SaveImage();
    // save image as specified filepath
    // filepath should have an extension
    void SaveImageAs(std::filesystem::path path);
    // save image to a specified irectory with specified filename and selected extension
    void SaveImageAs(std::filesystem::path dirPath, char *filename, int extension);
    // clears prev data
    // sets path to new path
    // creates new surface and texture
    // sets width and height based on surface
    // refreshes arrays
    int SetSourceImage(std::filesystem::path path);
    // frees the surface and texture
    // sets arrays to 0
    // sets width and height to 0
    // sets filepath to ""
    void ClearImage();

    // for safety checks
    inline bool NoSurface() { return surface == nullptr; }
    // for safety checks
    inline bool NoTexture() { return texture == nullptr; }

    // sets a white surface of specified dimensions
    // sets new width and height
    // sets the texture
    // does not change filename
    // refreshes arrays
    void SetBlankSurface(int width, int height);

    // to refresh arrays after surface was modified
    void RefreshPixelValuesArrays();
    // clears prev texture
    // to refresh texture after surface was modified
    void RefreshTexture();

    Pixel GetPixel(int x, int y);
    void SetPixel(int x, int y, Pixel pix);

    // r,g,b = 255
    void SetPixelWhite(int x, int y);
    // r,g,b = 255
    void SetPixelBlack(int x, int y);

    // same as copy constructor
    void Copy(Image &other);

    // use only if pixels and size info is needed
    // does not create texture
    // does not chenge filepath
    // does not refresh arrays
    void CopyOnlySurfaceAndSize(Image &other);

private:
    // the image itself
    SDL_Surface *surface = nullptr;
    // the texture to display
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
