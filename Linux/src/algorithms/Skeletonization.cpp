#include "Skeletonization.hpp"
Skeletonization::Skeletonization()
{
    autoRefresh = false;
    algorithmName = "Szkieletyzacja";
}

void Skeletonization::ParamsMenu() { ImGui::Text("Brak parametrów do tego algorytmu"); }

void Skeletonization::AlgorithmFunction(Image *outputImage)
{
    // local copy
    CopyToLocalVariable(outputImage);

    // TUTAJ UZUPEŁNIĆ

    SaveToOutput(outputImage);
}

void Skeletonization::ResetToDefaults() {} // NO PARAMS TO RESET

// ignore the warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Skeletonization::Save(std::ofstream &file) {} // NOTHING TO DO
#pragma GCC diagnostic pop

// ignore the warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Skeletonization::Load(std::ifstream &file) {} // NOTHING TO DO
#pragma GCC diagnostic pop