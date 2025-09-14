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

Image &Image::operator=(const Image &other)
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
    sourceImageName = other.sourceImageName;
    ext = other.ext;
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

std::string Image::GetExtension()
{
    if (ext == PNG)
        return ".png";
    else if (ext == JPG)
        return ".jpg";
    else if (ext == BMP)
        return ".bmp";
    // default to png
    else
        return ".png";
}

void Image::SaveImage()
{
    if (surface == nullptr)
        printf("Can not save \n");
    else
    {
        if (ext == PNG)
        {
            std::string newFileName = sourceImageName + ".png";
            IMG_SavePNG(surface, newFileName.c_str());
        }
        else if (ext == JPG)
        {
            std::string newFileName = sourceImageName + ".jpg";
            IMG_SaveJPG(surface, newFileName.c_str(), 100);
        }
        else if (ext == BMP)
        {
            std::string newFileName = sourceImageName + ".bmp";
            SDL_SaveBMP(surface, newFileName.c_str());
        }
        // default to png
        else
        {
            std::string newFileName = sourceImageName + ".png";
            IMG_SavePNG(surface, newFileName.c_str());
        }
    }
}

void Image::SaveImageAs(std::string filename)
{
    if (surface == nullptr)
        printf("Can not save \n");
    else
    {
        int pos = 0;
        for (int i = filename.length() - 1; i >= 0; i--)
            if (filename[i] == '.')
            {
                pos = i;
                break;
            }
        std::string extS = filename.substr(pos);
        sourceImageName = filename.substr(0, pos);
        if (extS == ".png")
        {
            ext = PNG;
            std::string newFileName = sourceImageName + ".png";
            IMG_SavePNG(surface, newFileName.c_str());
        }
        else if (extS == ".jpg" || extS == "jpeg")
        {
            ext = JPG;
            std::string newFileName = sourceImageName + ".jpg";
            IMG_SaveJPG(surface, newFileName.c_str(), 100);
        }
        else if (extS == ".bmp")
        {
            ext = BMP;
            std::string newFileName = sourceImageName + ".bmp";
            SDL_SaveBMP(surface, newFileName.c_str());
        }
        // default to png
        else
        {
            ext = PNG;
            std::string newFileName = sourceImageName + ".png";
            IMG_SavePNG(surface, newFileName.c_str());
        }
    }
}

void Image::SaveImageAs(std::string path, char *filename, int extension)
{
    if (surface == nullptr)
        printf("Can not save \n");
    else
    {
        sourceImageName = path + '/' + filename;
        if (extension == PNG)
        {
            ext = PNG;
            std::string newFileName = sourceImageName + ".png";
            IMG_SavePNG(surface, newFileName.c_str());
        }
        else if (extension == JPG)
        {
            ext = JPG;
            std::string newFileName = sourceImageName + ".jpg";
            IMG_SaveJPG(surface, newFileName.c_str(), 100);
        }
        else if (extension == BMP)
        {
            ext = BMP;
            std::string newFileName = sourceImageName + ".bmp";
            SDL_SaveBMP(surface, newFileName.c_str());
        }
        // default to png
        else
        {
            ext = PNG;
            std::string newFileName = sourceImageName + ".png";
            IMG_SavePNG(surface, newFileName.c_str());
        }
    }
}

int Image::SetSourceImage(std::string filename)
{
    ClearImage();
    surface = IMG_Load(filename.c_str());
    if (surface == nullptr)
        return -1;
    else
    {
        int pos = 0;
        for (int i = filename.length() - 1; i >= 0; i--)
            if (filename[i] == '.')
            {
                pos = i;
                break;
            }
        std::string extS = filename.substr(pos);
        sourceImageName = filename.substr(0, pos);
        if (extS == ".png")
            ext = PNG;
        else if (extS == ".jpg" || extS == "jpeg")
            ext = JPG;
        else if (extS == ".bmp")
            ext = BMP;
        // default to png
        else
            ext = PNG;
        texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetRenderer(), surface);
        // handle it in some way
        if (texture == nullptr)
            printf("%s\n", SDL_GetError());
        width = surface->w;
        height = surface->h;
        RefreshPixelValuesArrays();
        return 0;
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
    sourceImageName = "";
    ext = UNKNOWN;
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
    texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetRenderer(), surface);
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
        distributorLight[i] = other.distributorLight[i];
        distributorR[i] = other.distributorR[i];
        distributorG[i] = other.distributorG[i];
        distributorB[i] = other.distributorB[i];
    }
    sourceImageName = other.sourceImageName;
    ext = other.ext;
}
