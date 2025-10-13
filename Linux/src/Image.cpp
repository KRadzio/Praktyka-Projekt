#include "Image.hpp"

Image::Image()
{
    for (int i = 0; i < MAX_VAL; i++)
    {
        lightValues[i] = 0;
        valuesR[i] = 0;
        valuesG[i] = 0;
        valuesB[i] = 0;
        distributorLight[i] = 0;
        distributorR[i] = 0;
        distributorG[i] = 0;
        distributorB[i] = 0;
    }
}

Image::Image(const Image &other)
{
    ClearImage();
    surface = SDL_DuplicateSurface(other.surface);
    if (surface != nullptr)
        texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetRenderer(), surface);
    width = other.width;
    height = other.height;
    for (int i = 0; i < MAX_VAL; i++)
    {
        lightValues[i] = other.lightValues[i];
        valuesR[i] = other.valuesR[i];
        valuesG[i] = other.valuesG[i];
        valuesB[i] = other.valuesB[i];
        distributorLight[i] = other.distributorLight[i];
        distributorR[i] = other.distributorR[i];
        distributorG[i] = other.distributorG[i];
        distributorB[i] = other.distributorB[i];
    }
    filePath = other.filePath;
}

Image Image::operator=(const Image &other)
{
    ClearImage();
    surface = SDL_DuplicateSurface(other.surface);
    if (surface != nullptr)
        texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetRenderer(), surface);
    width = other.width;
    height = other.height;
    for (int i = 0; i < MAX_VAL; i++)
    {
        lightValues[i] = other.lightValues[i];
        valuesR[i] = other.valuesR[i];
        valuesG[i] = other.valuesG[i];
        valuesB[i] = other.valuesB[i];
        distributorLight[i] = other.distributorLight[i];
        distributorR[i] = other.distributorR[i];
        distributorG[i] = other.distributorG[i];
        distributorB[i] = other.distributorB[i];
    }
    filePath = other.filePath;
    return *this;
}

Image::Image(std::string filename)
{
    SetSourceImage(filename);
}

Image::~Image()
{
    SDL_FreeSurface(surface);
    if (texture != nullptr)
        SDL_DestroyTexture(texture);
}

void Image::CopyBrightnessHistogram(float *dst)
{
    for (int i = 0; i < MAX_VAL; i++)
        dst[i] = lightValues[i];
}

void Image::CopyNormalisedBrightnessHistogram(float *dst)
{
    for (int i = 0; i < MAX_VAL; i++)
    {
        dst[i] = lightValues[i];
        dst[i] /= (width * height);
    }
}

void Image::SaveImage()
{
    // no surface
    if (surface == nullptr)
        printf("Can not save \n");
    else
    {
        if (filePath.extension() == ".png")
            IMG_SavePNG(surface, filePath.c_str());
        else if (filePath.extension() == ".jpg" || filePath.extension() == "jpeg")
            IMG_SaveJPG(surface, filePath.c_str(), 100);
        else if (filePath.extension() == ".bmp")
            SDL_SaveBMP(surface, filePath.c_str());
        // default to png
        else
            IMG_SavePNG(surface, filePath.c_str());
    }
}

void Image::SaveImageAs(std::filesystem::path path)
{
    if (surface == nullptr)
        printf("Can not save \n");
    else
    {
        filePath = path;
        if (path.extension() == ".png")
            IMG_SavePNG(surface, filePath.c_str());
        else if (path.extension() == ".jpg" || path.extension() == "jpeg")
            IMG_SaveJPG(surface, filePath.c_str(), 100);
        else if (path.extension() == ".bmp")
            SDL_SaveBMP(surface, filePath.c_str());
        // default to png
        else
            IMG_SavePNG(surface, filePath.c_str());
    }
}

void Image::SaveImageAs(std::filesystem::path dirPath, char *filename, int extension)
{
    if (surface == nullptr)
        printf("Can not save \n");
    else
    {
        filePath = dirPath;
        filePath /= filename;
        if (extension == PNG)
        {
            filePath += ".png";
            IMG_SavePNG(surface, filePath.c_str());
        }
        else if (extension == JPG)
        {
            filePath += ".jpg";
            IMG_SaveJPG(surface, filePath.c_str(), 100);
        }
        else if (extension == BMP)
        {
            filePath += ".bmp";
            SDL_SaveBMP(surface, filePath.c_str());
        }
        // default to png
        else
        {
            filePath += ".png";
            IMG_SavePNG(surface, filePath.c_str());
        }
    }
}

int Image::SetSourceImage(std::filesystem::path path)
{
    ClearImage();
    surface = IMG_Load(path.c_str());
    if (surface == nullptr)
        return -1;
    else
    {
        width = surface->w;
        height = surface->h;
        if (surface->format->BytesPerPixel < 3)
        {
            // change the numbver of bytes per pixel
            SDL_Surface *newSurface = SDL_CreateRGBSurface(0, width, height, 24, 0, 0, 0, 0);
            SDL_LockSurface(surface);
            SDL_LockSurface(newSurface);
            uint8_t *surfacePixels = (uint8_t *)surface->pixels;
            uint8_t *newSurfacePixels = (uint8_t *)newSurface->pixels;
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    uint8_t br = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel];
                    newSurfacePixels[j * newSurface->pitch + i * newSurface->format->BytesPerPixel] = br;
                    newSurfacePixels[j * newSurface->pitch + i * newSurface->format->BytesPerPixel + 1] = br;
                    newSurfacePixels[j * newSurface->pitch + i * newSurface->format->BytesPerPixel + 2] = br;
                }
            }
            SDL_UnlockSurface(newSurface);
            SDL_UnlockSurface(surface);
            SDL_FreeSurface(surface);
            surface = newSurface;
        }
        filePath = path;
        texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetRenderer(), surface);
        // handle it in some way
        if (texture == nullptr)
            printf("%s\n", SDL_GetError());
        RefreshPixelValuesArrays();
        return 0;
    }
}

void Image::TurnToGrayScale()
{
    if (texture != nullptr)
        SDL_DestroyTexture(texture);
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
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel] = brightness;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1] = brightness;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2] = brightness;
        }
    }
    SDL_UnlockSurface(surface);
    texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetRenderer(), surface);
    RefreshPixelValuesArrays();
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
        valuesR[i] = 0;
        valuesG[i] = 0;
        valuesB[i] = 0;
        distributorLight[i] = 0;
        distributorR[i] = 0;
        distributorG[i] = 0;
        distributorB[i] = 0;
    }
    width = 0;
    height = 0;
    filePath = "";
}

void Image::SetBlankSurface(int width, int height)
{
    SDL_FreeSurface(surface);
    if (texture != nullptr)
        SDL_DestroyTexture(texture);
    surface = nullptr;
    texture = nullptr;
    this->width = width;
    this->height = height;
    surface = SDL_CreateRGBSurface(0, width, height, 24, 0, 0, 0, 0);
    texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetRenderer(), surface);
    RefreshPixelValuesArrays();
}

void Image::RefreshPixelValuesArrays()
{
    for (int i = 0; i < MAX_VAL; i++)
    {
        lightValues[i] = 0;
        valuesR[i] = 0;
        valuesG[i] = 0;
        valuesB[i] = 0;
        distributorLight[i] = 0;
        distributorR[i] = 0;
        distributorG[i] = 0;
        distributorB[i] = 0;
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
    distributorLight[0] = lightValues[0] / (width * height);
    distributorR[0] = valuesR[0] / (width * height);
    distributorG[0] = valuesG[0] / (width * height);
    distributorB[0] = valuesB[0] / (width * height);
    for (int i = 1; i < MAX_VAL; i++)
    {
        distributorLight[i] = lightValues[i] / (width * height);
        distributorLight[i] += distributorLight[i - 1];

        distributorR[i] = valuesR[i] / (width * height);
        distributorR[i] += distributorR[i - 1];

        distributorG[i] = valuesG[i] / (width * height);
        distributorG[i] += distributorG[i - 1];

        distributorB[i] = valuesB[i] / (width * height);
        distributorB[i] += distributorB[i - 1];
    }
    for (int i = 0; i < MAX_VAL; i++)
    {
        distributorLight[i] *= 255;
        distributorR[i] *= 255;
        distributorG[i] *= 255;
        distributorB[i] *= 255;
    }
}

void Image::RefreshTexture()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetRenderer(), surface);
}

Image::Pixel Image::GetPixel(int x, int y)
{
    SDL_LockSurface(surface);
    Pixel px;
    uint8_t *surfacePixels = (uint8_t *)surface->pixels;
    px.b = surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel];
    px.g = surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel + 1];
    px.r = surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel + 2];
    SDL_UnlockSurface(surface);
    px.brightnes = (px.b + px.g + px.r) / 3;
    return px;
}

// fix here
void Image::SetPixel(int x, int y, Pixel pix)
{
    SDL_LockSurface(surface);
    uint8_t *surfacePixels = (uint8_t *)surface->pixels;
    surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel] = pix.b;
    surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel + 1] = pix.g;
    surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel + 2] = pix.r;
    SDL_UnlockSurface(surface);
}

void Image::SetPixelWhite(int x, int y)
{
    SDL_LockSurface(surface);
    uint8_t *surfacePixels = (uint8_t *)surface->pixels;
    surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel] = 255;
    surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel + 1] = 255;
    surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel + 2] = 255;
    SDL_UnlockSurface(surface);
}

void Image::SetPixelBlack(int x, int y)
{
    SDL_LockSurface(surface);
    uint8_t *surfacePixels = (uint8_t *)surface->pixels;
    surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel] = 0;
    surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel + 1] = 0;
    surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel + 2] = 0;
    SDL_UnlockSurface(surface);
}

void Image::Copy(Image &other)
{
    ClearImage();
    surface = SDL_DuplicateSurface(other.surface);
    if (surface != nullptr)
        texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetRenderer(), surface);
    width = other.width;
    height = other.height;
    for (int i = 0; i < MAX_VAL; i++)
    {
        lightValues[i] = other.lightValues[i];
        valuesR[i] = other.valuesR[i];
        valuesG[i] = other.valuesG[i];
        valuesB[i] = other.valuesB[i];
        distributorLight[i] = other.distributorLight[i];
        distributorR[i] = other.distributorR[i];
        distributorG[i] = other.distributorG[i];
        distributorB[i] = other.distributorB[i];
    }
    filePath = other.filePath;
}

void Image::CopyOnlySurfaceAndSize(Image &other)
{
    ClearImage();
    surface = SDL_DuplicateSurface(other.surface);
    width = other.width;
    height = other.height;
}
