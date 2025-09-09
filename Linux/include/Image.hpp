#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

public:
    Image();
    // the texture is not copied so use RefreshTexture after this one
    Image &operator=(const Image &other);
    Image(std::string filename, SDL_Renderer *renderer);
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
    inline float *GetDistributor() { return distributor; }
    inline std::string &GetImageName() { return sourceImageName; }

    void SaveImage();
    void SaveImageAs(std::string filename);
    void SaveImageAs(std::string path, char *filename);
    void SetSourceImage(std::string filename, SDL_Renderer *renderer);
    void ClearImage();

    inline bool NoSurface() { return surface == nullptr; }
    inline bool NoTexture() { return texture == nullptr; }

    void RefreshPixelValuesArrays();
    void RefreshTexture(SDL_Renderer *renderer);

    Pixel GetPixel(int x, int y);
    void SetPixel(int x, int y, Pixel pix);

    inline void LockImage() {SDL_LockSurface(surface);}
    inline void UnlockImage() {SDL_UnlockSurface(surface);}

    void Copy(Image& other);

private:
    SDL_Surface *surface = nullptr;
    SDL_Texture *texture = nullptr;
    int width = 0;
    int height = 0;
    float lightValues[MAX_VAL];
    float valuesR[MAX_VAL];
    float valuesG[MAX_VAL];
    float valuesB[MAX_VAL];
    float distributor[MAX_VAL];
    std::string sourceImageName = "";
};

#endif
