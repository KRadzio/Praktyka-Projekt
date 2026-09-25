#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <filesystem>

#ifdef __linux__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#elif _WIN32

#include <SDL.h>
#include <SDL_image.h>

#endif

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
    // DO NOT USE IN A THREAD
    // USE CopyNoTexture
    // clears prev data
    // copy content and make a new texture
    // sets the same filepath as the other
    // (no checks if texture created)
    Image(const Image &other);
    // DO NOT USE IN A THREAD
    // USE CopyNoTexture
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
    // float[256]
    inline float *GetLightValues() { return lightValues; }
    // float[256]
    inline float *GetRValues() { return valuesR; }
    // float[256]
    inline float *GetGValues() { return valuesG; }
    // float[256]
    inline float *GetBValues() { return valuesB; }
    // float[256]
    inline float *GetDistributor() { return distributorLight; }
    // float[256]
    inline float *GetDistributorR() { return distributorR; }
    // float[256]
    inline float *GetDistributorG() { return distributorG; }
    // float[256]
    inline float *GetDistributorB() { return distributorB; }
    // float[256]
    inline int GetPixelCount() { return width * height; }

    inline std::filesystem::path GetImagePath() { return filePath; }

#ifdef __linux__

    inline std::string GetExtension() { return filePath.extension(); }

#elif _WIN32

    inline std::string GetExtension() { return filePath.extension().string(); }

#endif

    inline std::string GetError() { return error; }
    // for safety checks
    inline bool NoSurface() { return surface == nullptr; }
    // for safety checks
    inline bool NoTexture() { return texture == nullptr; }

    // each light value is devided by width * height
    // dst = float[256]
    void CopyNormalisedBrightnessHistogram(float *dst);

    // save image as the current filename
    // 0 - succes ; -1 - error
    int32_t SaveImage();
    // save image as specified filepath
    // filepath should have an extension
    // 0 - succes ; -1 - error
    int32_t SaveImageAs(std::filesystem::path path);
    // save image to a specified irectory with specified filename and selected extension

#ifdef __linux__
    // 0 - succes ; -1 - error
    int32_t SaveImageAs(std::filesystem::path dirPath, std::string filename, int extension);
#elif _WIN32
    // 0 - succes ; -1 - error
    int32_t SaveImageAs(std::filesystem::path dirPath, std::u8string filename, int extension);
#endif

    // DO NOT USE IN A THREAD
    // USE SetSourceImageNoTEXTURE
    // clears prev data
    // sets path to new path
    // creates new surface and texture
    // sets width and height based on surface
    // refreshes arrays
    // 0 - succes ; -1 - error
    int32_t SetSourceImage(std::filesystem::path path);

    // clears prev data
    // sets path to new path
    // creates new surface
    // sets width and height based on surface
    // refreshes arrays
    // use when extra image is needed
    // 0 - succes ; -1 - error
    int32_t SetSourceImageNoTEXTURE(std::filesystem::path path);

    // changes color pallete to gray scale
    void TurnToGrayScale();

    // DO NOT USE IN A THREAD
    // USE ClearImageNoTexture
    // frees the surface and texture
    // sets arrays to 0
    // sets width and height to 0
    // sets filepath to ""
    void ClearImage();

    // frees the surface (DOES NOT CLEAR TEXTURE)
    // sets arrays to 0
    // sets width and height to 0
    // sets filepath to ""
    void ClearImageNoTexture();

    // DO NOT USE IN THREAD
    // USE SetBlankSurfaceNoTexture
    // sets a white surface of specified dimensions
    // sets new width and height
    // sets the texture
    // does not change filename
    // refreshes arrays
    void SetBlankSurface(int width, int height);

    // sets a white surface of specified dimensions
    // sets new width and height
    // does not change filename
    // refreshes arrays
    void SetBlankSurfaceNoTexture(int width, int height);

    // to refresh arrays after surface was modified
    void RefreshPixelValuesArrays();

    // DO NOT USE IN A THREAD
    // clears prev texture
    // to refresh texture after surface was modified
    void RefreshTexture();

    // returns pixel at given coordinates
    Pixel GetPixel(int col, int row);
    // sets pixel at given coordinates
    void SetPixel(int col, int row, Pixel pix);

    // r,g,b = 255
    void SetPixelWhite(int col, int row);
    // r,g,b = 0
    void SetPixelBlack(int col, int row);

    // DO NOT USE IN A THREAD
    // USE CopyNoTexture
    // same as copy constructor
    void Copy(Image &other);

    // copys all data exept for texture
    void CopyNoTexture(Image &other);

private:
    // if color is saved on 1 byte AND has a custom pallete (ex. 2 colors)
    void ConvertFromPallete();

    // if color is saved on 1 byte insted of 3
    void NormalizeFormat();

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
    std::string error = "";
};

#endif
