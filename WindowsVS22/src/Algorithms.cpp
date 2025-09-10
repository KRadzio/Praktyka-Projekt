#include "App.hpp"

int Algorithms::CreateNegative(Image &inputImage, Image &outputImage, SDL_Renderer *renderer)
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
    outputImage.RefreshTexture(renderer);
    return 0;
}

int Algorithms::BrightenImage(Image &inputImage, Image &outputImage, SDL_Renderer *renderer, int value)
{
    if (inputImage.NoSurface())
    {
        // ImGui::OpenPopup("Blad");
        // ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        // ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // if (ImGui::BeginPopupModal("Blad", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        // {
        //     ImGui::Text("Brak wczytanego obrazu");
        //     ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
        //     if (ImGui::Button("Cofnij", ImVec2(CANCEL_BUTTON_W, 0)))
        //     {
        //         ImGui::CloseCurrentPopup();
        //     }
        //     ImGui::EndPopup();
        // }
        return -1;
    }

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

            if (b + value > 255)
                b = 255;
            else if (b + value < 0)
                b = 0;
            else
                b += value;

            if (g + value > 255)
                g = 255;
            else if (g + value < 0)
                g = 0;
            else
                g += value;

            if (r + value > 255)
                r = 255;
            else if (r + value < 0)
                r = 0;
            else
                r += value;

            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel] = b;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1] = g;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2] = r;
        }
    }
    SDL_UnlockSurface(surface);

    outputImage.RefreshPixelValuesArrays();
    outputImage.RefreshTexture(renderer);
    return 0;
}

int Algorithms::Contrast(Image &inputImage, Image &outputImage, SDL_Renderer *renderer, float value)
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

            if (b * value > 255)
                b = 255;
            else
                b *= value;

            if (g * value > 255)
                g = 255;
            else
                g *= value;

            if (r * value > 255)
                r = 255;
            else
                r *= value;

            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel] = b;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1] = g;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2] = r;
        }
    }
    SDL_UnlockSurface(surface);

    outputImage.RefreshPixelValuesArrays();
    outputImage.RefreshTexture(renderer);
    return 0;
}

int Algorithms::Exponentiation(Image &inputImage, Image &outputImage, SDL_Renderer *renderer, float alfa)
{
    if (inputImage.NoSurface())
        return -1;

    int tab[256];
    for (int i = 0; i < 256; i++)
        tab[i] = 255.0 * pow((float)i / 255.0, alfa);

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
    outputImage.RefreshTexture(renderer);
    return 0;
}

int Algorithms::LevelHistogram(Image &inputImage, Image &outputImage, SDL_Renderer *renderer)
{
    if (inputImage.NoSurface())
        return -1;

    outputImage = inputImage;
    SDL_Surface *surface = outputImage.GetSurface();
    float* dist = outputImage.GetDistributor();

    SDL_LockSurface(surface);
    uint8_t *surfacePixels = (uint8_t *)surface->pixels;
    for (int i = 0; i < outputImage.GetWidth(); i++)
    {
        for (int j = 0; j < outputImage.GetHeight(); j++)
        {
            uint8_t b = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel];
            uint8_t g = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1];
            uint8_t r = surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2];

            b = dist[b];
            g = dist[g];
            r = dist[r];

            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel] = b;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 1] = g;
            surfacePixels[j * surface->pitch + i * surface->format->BytesPerPixel + 2] = r;
        }
    }
    SDL_UnlockSurface(surface);

    outputImage.RefreshPixelValuesArrays();
    outputImage.RefreshTexture(renderer);
    return 0;
}