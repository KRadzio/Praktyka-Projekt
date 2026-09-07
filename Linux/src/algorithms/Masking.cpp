#include "Masking.hpp"

Masking::Masking() { algorithmName = "Maskowanie"; }

void Masking::ParamsMenu() { ImGui::Text("Brak parametrów do tego algorytmu"); }

void Masking::AlgorithmFunction(Image *outputImage)
{

    CopyToLocalVariable(outputImage);

    mask.SetSourceImageNoTEXTURE("./resources/maska.jpg");

    for (int row = 0; row < copy.GetHeight() && row < mask.GetHeight(); row++)
    {
        for (int col = 0; col < copy.GetWidth() && col < mask.GetWidth(); col++)
        {
            Image::Pixel px = copy.GetPixel(col, row), maskPx = mask.GetPixel(col, row);
            px.r *= (double)maskPx.r / WHITE;
            px.g *= (double)maskPx.g / WHITE;
            px.b *= (double)maskPx.b / WHITE;

            copy.SetPixel(col, row, px);
        }
        if (Canceled(outputImage))
            return;
        AutomaticRefresh(outputImage);
    }

    SaveToOutput(outputImage);
}

// nothing to do
void Masking::ResetToDefaults() {}