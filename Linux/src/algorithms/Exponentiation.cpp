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
    
    // TUTAJ UZUPEŁNIĆ

    SaveToOutput(outputImage);
}

void Exponentiation::ResetToDefaults() { alfa = 1.0; }

void Exponentiation::Save(std::ofstream &file) { file << "exponentiationAlfaValue" << CONFIG_SPLIT_CHAR_A << alfa << std::endl; }
void Exponentiation::Load(std::ifstream &file)
{
    try
    {
        alfa = std::stof(SplitLine(file));
    }
    catch (const std::exception &e)
    {
        alfa = 1.0;
    }
}
