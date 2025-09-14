#include "App.hpp"

int Algorithms::CreateNegative(Image &inputImage, Image &outputImage)
{
    if (inputImage.NoSurface())
        return -1;

    outputImage = inputImage;
    SDL_Surface *surface = outputImage.GetSurface();

    SDL_LockSurface(surface);
    uint8_t *surfacePixels = (uint8_t *)surface->pixels;
    for (int i = 0; i < outputImage.GetWidth(); i++)
    {
        for (int j = 0; j < outputImage.GetHeight(); j++)
        {
            uint8_t b = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel];
            uint8_t g = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1];
            uint8_t r = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2];

            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel] = 255 - b;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1] = 255 - g;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2] = 255 - r;

            b = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel];
            g = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1];
            r = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2];
        }
    }
    SDL_UnlockSurface(surface);
    outputImage.RefreshPixelValuesArrays();
    outputImage.RefreshTexture();
    return 0;
}

int Algorithms::BrightenImage(Image &inputImage, Image &outputImage, ParametersStruct *params)
{
    if (inputImage.NoSurface())
        return -1;

    outputImage = inputImage;
    SDL_Surface *surface = outputImage.GetSurface();

    SDL_LockSurface(surface);
    uint8_t *surfacePixels = (uint8_t *)surface->pixels;
    for (int i = 0; i < outputImage.GetWidth(); i++)
    {
        for (int j = 0; j < outputImage.GetHeight(); j++)
        {
            uint8_t b = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel];
            uint8_t g = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1];
            uint8_t r = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2];

            if (b + params->value > 255)
                b = 255;
            else if (b + params->value < 0)
                b = 0;
            else
                b += params->value;

            if (g + params->value > 255)
                g = 255;
            else if (g + params->value < 0)
                g = 0;
            else
                g += params->value;

            if (r + params->value > 255)
                r = 255;
            else if (r + params->value < 0)
                r = 0;
            else
                r += params->value;

            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel] = b;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1] = g;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2] = r;
        }
    }
    SDL_UnlockSurface(surface);

    outputImage.RefreshPixelValuesArrays();
    outputImage.RefreshTexture();
    return 0;
}

int Algorithms::Contrast(Image &inputImage, Image &outputImage, ParametersStruct *params)
{
    if (inputImage.NoSurface())
        return -1;

    outputImage = inputImage;
    SDL_Surface *surface = outputImage.GetSurface();

    SDL_LockSurface(surface);
    uint8_t *surfacePixels = (uint8_t *)surface->pixels;
    for (int i = 0; i < outputImage.GetWidth(); i++)
    {
        for (int j = 0; j < outputImage.GetHeight(); j++)
        {
            uint8_t b = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel];
            uint8_t g = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1];
            uint8_t r = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2];

            if (b * params->contrast > 255)
                b = 255;
            else
                b *= params->contrast;

            if (g * params->contrast > 255)
                g = 255;
            else
                g *= params->contrast;

            if (r * params->contrast > 255)
                r = 255;
            else
                r *= params->contrast;

            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel] = b;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1] = g;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2] = r;
        }
    }
    SDL_UnlockSurface(surface);

    outputImage.RefreshPixelValuesArrays();
    outputImage.RefreshTexture();
    return 0;
}

int Algorithms::Exponentiation(Image &inputImage, Image &outputImage, ParametersStruct *params)
{
    if (inputImage.NoSurface())
        return -1;

    int tab[256];
    for (int i = 0; i < 256; i++)
        tab[i] = 255.0 * pow((float)i / 255.0, params->alfa);

    outputImage = inputImage;
    SDL_Surface *surface = outputImage.GetSurface();

    SDL_LockSurface(surface);
    uint8_t *surfacePixels = (uint8_t *)surface->pixels;
    for (int i = 0; i < outputImage.GetWidth(); i++)
    {
        for (int j = 0; j < outputImage.GetHeight(); j++)
        {
            uint8_t b = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel];
            uint8_t g = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1];
            uint8_t r = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2];

            b = tab[b];
            g = tab[g];
            r = tab[r];

            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel] = b;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1] = g;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2] = r;
        }
    }
    SDL_UnlockSurface(surface);

    outputImage.RefreshPixelValuesArrays();
    outputImage.RefreshTexture();
    return 0;
}

int Algorithms::LevelHistogram(Image &inputImage, Image &outputImage)
{
    if (inputImage.NoSurface())
        return -1;

    outputImage = inputImage;
    float *distR = outputImage.GetDistributorR();
    float *distG = outputImage.GetDistributorG();
    float *distB = outputImage.GetDistributorB();

    outputImage.LockImage();
    for (int i = 0; i < outputImage.GetWidth(); i++)
    {
        for (int j = 0; j < outputImage.GetHeight(); j++)
        {
            Image::Pixel pix = outputImage.GetPixel(i, j);

            pix.b = distB[pix.b];
            pix.g = distG[pix.g];
            pix.r = distR[pix.r];

            outputImage.SetPixel(i, j, pix);
        }
    }
    outputImage.UnlockImage();

    outputImage.RefreshPixelValuesArrays();
    outputImage.RefreshTexture();
    return 0;
}