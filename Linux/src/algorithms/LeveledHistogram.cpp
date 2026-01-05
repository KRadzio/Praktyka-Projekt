#include "LeveledHistogram.hpp"
LeveledHistogram::LeveledHistogram() { algorithmName = "Wyrównanie histogramu"; }

void LeveledHistogram::ParamsMenu()
{
    ImGui::Text("Brak parametrów do tego algorytmu");
}

void LeveledHistogram::AlgorithmFunction(Image *outputImage)
{
    // local copy
    CopyToLocalVariable(outputImage);

    // copy disttributors
    float *distR = copy.GetDistributorR();
    float *distG = copy.GetDistributorG();
    float *distB = copy.GetDistributorB();

    for (int row = 0; row < copy.GetHeight(); row++)
    {
        for (int col = 0; col < copy.GetWidth(); col++)
        {
            Image::Pixel pix = copy.GetPixel(col, row);
            pix.r = distR[pix.r];
            pix.g = distG[pix.g];
            pix.b = distB[pix.b];
            copy.SetPixel(col, row, pix);
        }
        if (Canceled(outputImage))
            return;
        Refresh(outputImage);
    }

    SaveToOutput(outputImage);
}

void LeveledHistogram::ResetToDefaults() {} // NO PARAMS TO RESET