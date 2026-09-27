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

    // TUTAJ UZUPEŁNIĆ

    SaveToOutput(outputImage);
}

void LeveledHistogram::ResetToDefaults() {} // NO PARAMS TO RESET

// ignore the warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void LeveledHistogram::Save(std::ofstream &file) {} // NOTHING TO DO
#pragma GCC diagnostic pop

// ignore the warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void LeveledHistogram::Load(std::ifstream &file) {} // NOTHING TO DO
#pragma GCC diagnostic pop