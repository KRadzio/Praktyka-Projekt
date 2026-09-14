#include "Masking.hpp"

Masking::Masking() { algorithmName = "Maskowanie"; }

void Masking::ParamsMenu()
{
    if (!mask.NoSurface())
        ImGui::Text("%s", mask.GetImagePath().filename().c_str());
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

    for (int row = 0; row < copy.GetHeight() && row < mask.GetHeight(); row++)
    {
        for (int col = 0; col < copy.GetWidth() && col < mask.GetWidth(); col++)
        {
            Image::Pixel px = copy.GetPixel(col, row), maskPx = mask.GetPixel(col, row);
            px.r *= (double)maskPx.r / WHITE;
            px.g *= (double)maskPx.g / WHITE;
            px.b *= (double)maskPx.b / WHITE;

            copy.SetPixel(col, row, px);
        }
        if (Canceled(outputImage))
            return;
        AutomaticRefresh(outputImage);
    }

    SaveToOutput(outputImage);
}

// nothing to do
void Masking::ResetToDefaults() {}