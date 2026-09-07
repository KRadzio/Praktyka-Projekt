#include "Mixing.hpp"

Mixing::Mixing() { algorithmName = "Mieszanie Obrazów"; }

void Mixing::ParamsMenu() { ImGui::SliderFloat("Wartość a", &mixRatio, 0.0, 1.0); }

void Mixing::AlgorithmFunction(Image *outputImage)
{
    CopyToLocalVariable(outputImage);
    
    image2.SetSourceImageNoTEXTURE("./resources/maska.jpg");

    for (int row = 0; row < copy.GetHeight() && row < image2.GetHeight(); row++)
    {
        for (int col = 0; col < copy.GetWidth() && col < image2.GetWidth(); col++)
        {
            Image::Pixel px = copy.GetPixel(col, row), px2 = image2.GetPixel(col, row);
            px.r = mixRatio * px.r + (1 - mixRatio) * px2.r;
            px.g = mixRatio * px.g + (1 - mixRatio) * px2.g;
            px.b = mixRatio * px.b + (1 - mixRatio) * px2.b;

            copy.SetPixel(col, row, px);
        }
        if (Canceled(outputImage))
            return;
        AutomaticRefresh(outputImage);
    }

    SaveToOutput(outputImage);
}

void Mixing::ResetToDefaults() { mixRatio = 0.5f; }