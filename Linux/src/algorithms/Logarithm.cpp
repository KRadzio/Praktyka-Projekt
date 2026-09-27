#include "Logarithm.hpp"

Logarithm::Logarithm() { algorithmName = "Logarytmowanie"; }

void Logarithm::ParamsMenu() { ImGui::Text("Brak parametrów do tego algorytmu"); }

void Logarithm::AlgorithmFunction(Image *outputImage)
{

    CopyToLocalVariable(outputImage);

    // TUTAJ UZUPEŁNIĆ

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