#include "Negative.hpp"

Negative::Negative(){algorithmName = "Negatyw";}

void Negative::ParamsMenu()
{
    ImGui::Text("Brak parametrów do tego algorytmu");
}

void Negative::AlgorithmFunction(Image *outputImage)
{
    CopyToLocalVariable(outputImage);

    for (int row = 0; row < copy.GetHeight(); row++)
    {
        for (int col = 0; col < copy.GetWidth(); col++)
        {
            auto pix = copy.GetPixel(col, row);
            pix.b = 255 - pix.b;
            pix.g = 255 - pix.g;
            pix.r = 255 - pix.r;
            copy.SetPixel(col, row, pix);
        }
        if (Canceled(outputImage))
            return;
        AutoRefresh(outputImage);
    }
    SaveToOutput(outputImage);
}

void Negative::ResetToDefaults()
{
    // NO PARAMS TO RESET
}