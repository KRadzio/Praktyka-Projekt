#include "Image.hpp"

Image::Image()
{
    for (int i = 0; i < MAX_VAL; i++)
    {
        lightValues[i] = 0;
        valuesR[i] = 0;
        valuesG[i] = 0;
        valuesB[i] = 0;
    }
}

Image &Image::operator=(const Image &other)
{
    ClearImage();
    surface = SDL_DuplicateSurface(other.surface);
    // texture = other.texture;
    width = other.width;
    height = other.height;
    for (int i = 0; i < MAX_VAL; i++)
    {
        lightValues[i] = other.lightValues[i];
        valuesR[i] = other.valuesR[i];
        valuesG[i] = other.valuesG[i];
        valuesB[i] = other.valuesB[i];
    }
    sourceImageName = other.sourceImageName;
    return *this;
}

Image::Image(std::string filename, SDL_Renderer *renderer)
{
    SetSourceImage(filename, renderer);
}

Image::~Image()
{
    SDL_FreeSurface(surface);
    if (texture != nullptr)
        SDL_DestroyTexture(texture);
}

void Image::SetSourceImage(std::string filename, SDL_Renderer *renderer)
{
    ClearImage();
    surface = SDL_LoadBMP(filename.c_str());
    if (surface == nullptr)
    {
        printf("%s\n", SDL_GetError());
        // make a call to display a gui error later
        for (int i = 0; i < MAX_VAL; i++)
        {
            lightValues[i] = 0;
            valuesR[i] = 0;
            valuesG[i] = 0;
            valuesB[i] = 0;
        }
    }
    else
    {
        sourceImageName = filename;
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        // handle it in some way
        if (texture == nullptr)
            printf("%s\n", SDL_GetError());
        width = surface->w;
        height = surface->h;
        SDL_LockSurface(surface);
        uint8_t *surfacePixels = (uint8_t *)surface->pixels;
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                uint8_t b = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel];
                uint8_t g = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1];
                uint8_t r = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2];
                int brightness = (b + g + r) / 3;
                lightValues[brightness]++;
                valuesB[b]++;
                valuesG[g]++;
                valuesR[r]++;
            }
        }
        SDL_UnlockSurface(surface);
    }
}

void Image::ClearImage()
{
    SDL_FreeSurface(surface);
    if (texture != nullptr)
        SDL_DestroyTexture(texture);
    surface = nullptr;
    texture = nullptr;
    for (int i = 0; i < MAX_VAL; i++)
    {
        lightValues[i] = 0;
        valuesB[i] = 0;
        valuesG[i] = 0;
        valuesR[i] = 0;
    }
    width = 0;
    height = 0;
    sourceImageName = "";
}

void Image::RefreshPixelValuesArrays()
{
    for (int i = 0; i < MAX_VAL; i++)
    {
        lightValues[i] = 0;
        valuesB[i] = 0;
        valuesG[i] = 0;
        valuesR[i] = 0;
    }

    SDL_LockSurface(surface);
    uint8_t *surfacePixels = (uint8_t *)surface->pixels;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            uint8_t b = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel];
            uint8_t g = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1];
            uint8_t r = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2];

            int brightness = (b + g + r) / 3;
            lightValues[brightness]++;
            valuesB[b]++;
            valuesG[g]++;
            valuesR[r]++;
        }
    }
    SDL_UnlockSurface(surface);
}

void Image::RefreshTexture(SDL_Renderer *renderer)
{
    texture = SDL_CreateTextureFromSurface(renderer, surface);
}

// void Image::CopySurface(SDL_Surface *newSurface)
// {
//     SDL_FreeSurface(surface);
//     surface = SDL_DuplicateSurface(newSurface);
// }

// Image Image::Copy()
// {
//     Image *copy = new Image();
//     *copy = *this;
//     return *copy;
// }
