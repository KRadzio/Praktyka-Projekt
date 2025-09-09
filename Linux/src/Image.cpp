#include "Image.hpp"

Image::Image()
{
    for (int i = 0; i < MAX_VAL; i++)
    {
        lightValues[i] = 0;
        valuesR[i] = 0;
        valuesG[i] = 0;
        valuesB[i] = 0;
        distributor[i] = 0;
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
        distributor[i] = other.distributor[i];
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

void Image::SaveImage()
{
    if (surface == nullptr)
        printf("Can not save \n");
    else
    {
        int pos = 0;
        for (int i = sourceImageName.length(); i >= 0; i--)
            if (sourceImageName[i] == '.')
            {
                pos = i;
                break;
            }
        std::string newFileName = sourceImageName.substr(0, pos) + "Copy" + sourceImageName.substr(pos);
        IMG_SavePNG(surface, newFileName.c_str());
    }
}

void Image::SaveImageAs(std::string filename)
{
    if (surface == nullptr)
        printf("Can not save \n");
    else
    {
        sourceImageName = filename;
        IMG_SavePNG(surface, sourceImageName.c_str());
    }
}

void Image::SaveImageAs(std::string path, char *filename)
{
    if (surface == nullptr)
        printf("Can not save \n");
    else
    {
        sourceImageName = path + '/' + filename + ".png";
        IMG_SavePNG(surface, sourceImageName.c_str());
    }
}

void Image::SetSourceImage(std::string filename, SDL_Renderer *renderer)
{
    ClearImage();
    surface = IMG_Load(filename.c_str());
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
            distributor[i] = 0;
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
        RefreshPixelValuesArrays();
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
        distributor[i] = 0;
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
        distributor[i] = 0;
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
    distributor[0] = lightValues[0] / (width * height);
    distributor[0] *= 255;
    for (int i = 1; i < MAX_VAL; i++)
    {
        distributor[i] = lightValues[i] / (width * height);
        distributor[i] *= 255;
        distributor[i] += distributor[i - 1];
    }
    SDL_UnlockSurface(surface);
}

void Image::RefreshTexture(SDL_Renderer *renderer)
{
    texture = SDL_CreateTextureFromSurface(renderer, surface);
}

Image::Pixel Image::GetPixel(int x, int y)
{
    Pixel px;
    uint8_t *surfacePixels = (uint8_t *)surface->pixels;
    px.b = surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel];
    px.g = surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel + 1];
    px.r = surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel + 2];
    return px;
}

void Image::SetPixel(int x, int y, Pixel pix)
{
    uint8_t *surfacePixels = (uint8_t *)surface->pixels;
    surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel] = pix.b;
    surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel + 1] = pix.g;
    surfacePixels[y * surface->pitch + x * surface->format->BytesPerPixel + 2] = pix.r;
}

void Image::Copy(Image &other)
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
        distributor[i] = other.distributor[i];
    }
    sourceImageName = other.sourceImageName;
}
