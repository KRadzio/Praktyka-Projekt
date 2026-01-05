#include "Exponentiation.hpp"

Exponentiation::Exponentiation() { algorithmName = "Potęgowanie"; }

void Exponentiation::ParamsMenu()
{
    ImGui::SliderFloat("Wartość alfa", &alfa, 0.1, 3.0);
}

void Exponentiation::AlgorithmFunction(Image *outputImage)
{
    // loacl copy
    CopyToLocalVariable(outputImage);
    int tab[256];
    for (int i = 0; i < 256; i++)
        tab[i] = 255.0 * pow((float)i / 255.0, alfa);

    for (int row = 0; row < copy.GetHeight(); row++)
    {
        for (int col = 0; col < copy.GetWidth(); col++)
        {
            auto pix = copy.GetPixel(col, row);
            pix.b = tab[pix.b];
            pix.g = tab[pix.g];
            pix.r = tab[pix.r];
            copy.SetPixel(col, row, pix);
        }
        if (Canceled(outputImage))
            return;
        Refresh(outputImage);
    }

    SaveToOutput(outputImage);
}

void Exponentiation::ResetToDefaults() { alfa = 1.0; }
