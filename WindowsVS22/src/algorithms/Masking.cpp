#include "Masking.hpp"

Masking::Masking() { algorithmName = "Maskowanie"; }

void Masking::ParamsMenu()
{
    if (!mask.NoSurface())
#ifdef __linux__
        ImGui::Text("%s", mask.GetImagePath().filename().c_str());
#elif _WIN32
        ImGui::Text("%s", mask.GetImagePath().filename().u8string().c_str());
#endif
    else
        ImGui::Text("Brak obrazu");
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - CANCEL_BUTTON_W / 2);
    if (ImGui::Button("Wczytaj obraz", ImVec2(CANCEL_BUTTON_W_FS, 0)))
    {
        FileSelector::GetInstance().RefreshCurrDir();
        maskingLoadMenuActive = true;
    }
    if (maskingLoadMenuActive)
    {
        if (FileSelector::GetInstance().LoadMenu(&mask, true) != 2)
            maskingLoadMenuActive = false;
    }
    ImGui::Separator();
}

void Masking::AlgorithmFunction(Image *outputImage)
{

    CopyToLocalVariable(outputImage);

    // TUTAJ UZUPEŁNIĆ

    SaveToOutput(outputImage);
}

void Masking::ResetToDefaults()
{
    mask.ClearImage();
}

void Masking::Save(std::ofstream &file)
{
    file << "maskPath" << CONFIG_SPLIT_CHAR_A << mask.GetImagePath().string() << std::endl;
}

void Masking::Load(std::ifstream &file)
{
    auto path = SplitLine(file);
    if (FileSelector::GetInstance().FileExists(path))
    {
        if (mask.SetSourceImageNoTEXTURE(path) != 0)
            mask.ClearImage();
    }
}