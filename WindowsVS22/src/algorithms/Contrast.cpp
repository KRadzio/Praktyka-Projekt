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

    for (int row = 0; row < copy.GetHeight(); row++)
    {
        for (int col = 0; col < copy.GetWidth(); col++)
        {
            auto pix = copy.GetPixel(col, row);

            if (pix.b * contrast > 255)
                pix.b = 255;
            else
                pix.b *= contrast;

            if (pix.g * contrast > 255)
                pix.g = 255;
            else
                pix.g *= contrast;

            if (pix.r * contrast > 255)
                pix.r = 255;
            else
                pix.r *= contrast;
            copy.SetPixel(col, row, pix);
        }
        if (Canceled(outputImage))
            return;
        AutomaticRefresh(outputImage);
    }

    SaveToOutput(outputImage);
}

void Contrast::ResetToDefaults() { contrast = 1.0; }

void Contrast::Save(std::ofstream &file) {file << "contrastValue" << CONFIG_SPLIT_CHAR_A << contrast << std::endl;}

void Contrast::Load(std::ifstream &file) {
    try
    {
        contrast = std::stof(SplitLine(file));
    }
    catch(const std::exception& e)
    {
        contrast = 1.0;
    }
    
}