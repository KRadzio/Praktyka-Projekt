#include "Negative.hpp"

Negative::Negative() { algorithmName = "Negatyw"; }

void Negative::ParamsMenu()
{
    ImGui::Text("Brak parametrów do tego algorytmu");
}

void Negative::AlgorithmFunction(Image *outputImage)
{
    CopyToLocalVariable(outputImage);

    // TUTAJ UZUPEŁNIĆ

    SaveToOutput(outputImage);
}

void Negative::ResetToDefaults()
{
    // NO PARAMS TO RESET
}

// ignore the warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Negative::Save(std::ofstream &file) {} // NOTHING TO DO
#pragma GCC diagnostic pop

// ignore the warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Negative::Load(std::ifstream &file) {} // NOTHING TO DO
#pragma GCC diagnostic pop
