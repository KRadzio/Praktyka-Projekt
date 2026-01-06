#include "Brighten.hpp"

Brighten::Brighten() { algorithmName = "Rosjaśnij/Przyciemnij"; }

void Brighten::ParamsMenu()
{
    ImGui::SliderInt("O ile rozjaśnić/przyciemnić?", &value, -255, 255);
}
void Brighten::AlgorithmFunction(Image *outputImage)
{
    CopyToLocalVariable(outputImage);

    for (int row = 0; row < copy.GetHeight(); row++)
    {
        for (int col = 0; col < copy.GetWidth(); col++)
        {
            auto pix = copy.GetPixel(col, row);

            if (pix.b + value > 255)
                pix.b = 255;
            else if (pix.b + value < 0)
                pix.b = 0;
            else
                pix.b += value;

            if (pix.g + value > 255)
                pix.g = 255;
            else if (pix.g + value < 0)
                pix.g = 0;
            else
                pix.g += value;

            if (pix.r + value > 255)
                pix.r = 255;
            else if (pix.r + value < 0)
                pix.r = 0;
            else
                pix.r += value;

            copy.SetPixel(col, row, pix);
        }
        if (Canceled(outputImage))
            return;
        AutomaticRefresh(outputImage);
    }
    
    SaveToOutput(outputImage);
}

void Brighten::ResetToDefaults() { value = 0; }