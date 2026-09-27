#include "Contrast.hpp"

Contrast::Contrast() { algorithmName = "Kontrast"; }

void Contrast::ParamsMenu()
{
    ImGui::SliderFloat("O ile zmienić kontrast?", &contrast, 0.1, 5.0);
}

void Contrast::AlgorithmFunction(Image *outputImage)
{

    // local copy
    CopyToLocalVariable(outputImage);
    
    // TUTAJ UZUPEŁNIĆ

    SaveToOutput(outputImage);
}

void Contrast::ResetToDefaults() { contrast = 1.0; }

void Contrast::Save(std::ofstream &file) { file << "contrastValue" << CONFIG_SPLIT_CHAR_A << contrast << std::endl; }

void Contrast::Load(std::ifstream &file)
{
    try
    {
        contrast = std::stof(SplitLine(file));
    }
    catch (const std::exception &e)
    {
        contrast = 1.0;
    }
}