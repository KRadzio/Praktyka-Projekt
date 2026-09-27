#include "Binarization.hpp"

Binarization::Binarization() { algorithmName = "Binaryzacja"; }

void Binarization::ParamsMenu()
{ // set if auto or manual
    ImGui::Text("Wybierz czy chcesz ustawić samemu\nczy automatycznie");
    ImGui::RadioButton("Ręcznie ustaw próg", &method, None);
    ImGui::SameLine();
    ImGui::RadioButton("Metoda Gradientowa", &method, Gradient);
    ImGui::SameLine();
    ImGui::RadioButton("Metoda iteracyjna", &method, Iteration);
    if (method == None)
    {
        ImGui::Separator();
        ImGui::Text("Wybierz ilość progów");
        ImGui::RadioButton("Jeden próg", &boundCount, 1);
        ImGui::SameLine();
        ImGui::RadioButton("Dwa progi", &boundCount, 2);
        ImGui::Separator();
        ImGui::Text("Ustaw progi");
        ImGui::SliderInt("t", &lowerBound, 0, 255);
        if (boundCount == 2)
            ImGui::SliderInt("t1", &upperBound, 0, 255);
    }
}

void Binarization::AlgorithmFunction(Image *outputImage)
{
    // local copy
    CopyToLocalVariable(outputImage);

    // TUTAJ UZUPEŁNIĆ

    // copt back to output
    SaveToOutput(outputImage);
}

void Binarization::ResetToDefaults()
{
    boundCount = 1;
    lowerBound = 0;
    upperBound = 0;
    method = None;
}

void Binarization::Save(std::ofstream &file)
{
    file << "binarizationBoundCount" << CONFIG_SPLIT_CHAR_A << boundCount << std::endl;
    file << "binarizationLowerBoundValue" << CONFIG_SPLIT_CHAR_A << lowerBound << std::endl;
    file << "binarizationUpperBoundValue" << CONFIG_SPLIT_CHAR_A  << upperBound << std::endl;
    file << "binarizationMethod" << CONFIG_SPLIT_CHAR_A << method << std::endl;
}

void Binarization::Load(std::ifstream &file) {
    try
    {
        boundCount = std::stoi(SplitLine(file));
    }
    catch(const std::exception& e)
    {
        boundCount = 1;
    }
    try
    {
        lowerBound = std::stoi(SplitLine(file));
    }
    catch(const std::exception& e)
    {
        lowerBound = 0;
    }
    try
    {
        upperBound = std::stoi(SplitLine(file));
    }
    catch(const std::exception& e)
    {
        upperBound = 0;
    }
    try
    {
        method = std::stoi(SplitLine(file));
    }
    catch(const std::exception& e)
    {
        method = None;
    }
}