#include "Logarithm.hpp"

Logarithm::Logarithm() { algorithmName = "Logarytmowanie"; }

void Logarithm::ParamsMenu() { ImGui::Text("Brak parametrów do tego algorytmu"); }

void Logarithm::AlgorithmFunction(Image *outputImage)
{

    CopyToLocalVariable(outputImage);

    uint8_t lut[MAX_VAL];

    for (int i = 0; i < MAX_VAL; i++)
        lut[i] = WHITE * std::log(1 + i) / std::log(MAX_VAL);

    for (int i = 0; i < copy.GetHeight(); i++)
    {
        for (int j = 0; j < copy.GetWidth(); j++)
        {
            Image::Pixel px = copy.GetPixel(j, i);
            px.r = lut[px.r];
            px.g = lut[px.g];
            px.b = lut[px.b];

            copy.SetPixel(j, i, px);
        }
    }
    if (Canceled(outputImage))
        return;
    AutomaticRefresh(outputImage);

    SaveToOutput(outputImage);
}

void Logarithm::ResetToDefaults() {} // NO PARAMS TO RESET

// ignore the warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Logarithm::Save(std::ofstream &file) {} // NOTHING TO DO
#pragma GCC diagnostic pop

// ignore the warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Logarithm::Load(std::ifstream &file) {} // NOTHING TO DO
#pragma GCC diagnostic pop