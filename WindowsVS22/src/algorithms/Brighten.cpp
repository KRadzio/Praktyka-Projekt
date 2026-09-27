#include "Brighten.hpp"

Brighten::Brighten() { algorithmName = "Rosjaśnij/Przyciemnij"; }

void Brighten::ParamsMenu()
{
    ImGui::SliderInt("O ile rozjaśnić/przyciemnić?", &value, -255, 255);
}
void Brighten::AlgorithmFunction(Image *outputImage)
{
    CopyToLocalVariable(outputImage);

    // TUTAJ UZUPEŁNIĆ

    SaveToOutput(outputImage);
}

void Brighten::ResetToDefaults() { value = 0; }

void Brighten::Save(std::ofstream &file) { file << "brightenValue" << CONFIG_SPLIT_CHAR_A << value << std::endl; }

void Brighten::Load(std::ifstream &file)
{
    try
    {
        value = std::stoi(SplitLine(file));
    }
    catch (const std::exception &e)
    {
        value = 0;
    }
}