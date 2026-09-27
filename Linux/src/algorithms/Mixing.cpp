#include "Mixing.hpp"

Mixing::Mixing() { algorithmName = "Mieszanie Obrazów"; }

void Mixing::ParamsMenu()
{
    ImGui::SliderFloat("Wartość a", &mixRatio, 0.0, 1.0);
    if (!image2.NoSurface())
#ifdef __linux__
        ImGui::Text("%s", image2.GetImagePath().filename().c_str());
#elif _WIN32
        ImGui::Text("%s", image2.GetImagePath().filename().u8string().c_str());
#endif
    else
        ImGui::Text("Brak obrazu");
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
    if (ImGui::Button("Wczytaj obraz", ImVec2(CANCEL_BUTTON_W_FS, 0)))
    {
        FileSelector::GetInstance().RefreshCurrDir();
        mixingLoadMenuActive = true;
    }
    if (mixingLoadMenuActive)
    {
        if (FileSelector::GetInstance().LoadMenu(&image2, true) != 2)
            mixingLoadMenuActive = false;
    }
    ImGui::Separator();
}

void Mixing::AlgorithmFunction(Image *outputImage)
{
    CopyToLocalVariable(outputImage);

    // TUTAJ UZUPEŁNIĆ

    SaveToOutput(outputImage);
}

void Mixing::ResetToDefaults()
{
    mixRatio = 0.5f;
    image2.ClearImage();
}

void Mixing::Save(std::ofstream &file)
{
    file << "mixingRatio" << CONFIG_SPLIT_CHAR_A << mixRatio << std::endl;
    file << "mixingImagePath" << CONFIG_SPLIT_CHAR_A << image2.GetImagePath().string() << std::endl;
}

void Mixing::Load(std::ifstream &file)
{
    try
    {
        mixRatio = std::stof(SplitLine(file));
    }
    catch (const std::exception &e)
    {
        mixRatio = 0.5f;
    }
    auto path = SplitLine(file);
    if (FileSelector::GetInstance().FileExists(path))
    {
        if (image2.SetSourceImageNoTEXTURE(path) != 0)
            image2.ClearImage();
    }
}