#include "Erosion.hpp"
Erosion::Erosion()
{
    algorithmName = "Erozja";
    ResetToDefaults();
}

void Erosion::ParamsMenu()
{
    ImGui::Text("Wybierz rozmiar");
    ImGui::RadioButton("3x3", &erosionElementSize, S3x3);
    ImGui::SameLine();
    ImGui::RadioButton("5x5", &erosionElementSize, S5x5);
    ImGui::SameLine();
    ImGui::RadioButton("7x7", &erosionElementSize, S7x7);
    ImGui::Separator();
    ImGui::Text("Zaznacz pola, które mają \nbyć elementem strukturalnym");
    DrawInputArray();
}

void Erosion::AlgorithmFunction(Image *outputImage)
{
    // local copy
    CopyToLocalVariable(outputImage);
    // copy - to save
    // copyRead - read
    // this operation require 2 copies, one for readin, the other to save result
    Image copyRead;
    copyRead.CopyNoTexture(copy);
    int32_t offsetLeft = 0;
    int32_t offsetRight = 0;
    int32_t offsetTop = 0;
    int32_t offsetBottom = 0;
    int32_t elemntCopy[7][7]; // copy the current mask (there are 3 sizes this way its easier)

    // copy element
    for (int i = 0; i < erosionElementSize; i++)
        for (int j = 0; j < erosionElementSize; j++)
        {
            if (erosionElementSize == S3x3)
                elemntCopy[i][j] = erosionElement3x3[i][j];
            else if (erosionElementSize == S5x5)
                elemntCopy[i][j] = erosionElement5x5[i][j];
            else
                elemntCopy[i][j] = erosionElement7x7[i][j];
        }

    Mutex::GetInstance().Unlock();

    // calculate offsets
    for (int row = 0; row < erosionElementSize; row++)
    {
        for (int col = 0; col < erosionElementSize; col++)
        {
            if (elemntCopy[row][col])
            {
                int left = erosionElementSize / 2 - col;
                int right = col - erosionElementSize / 2;
                int top = erosionElementSize / 2 - row;
                int bottom = row - erosionElementSize / 2;
                if (left >= 0)
                    if (left > offsetLeft)
                        offsetLeft = left;
                if (right >= 0)
                    if (right > offsetRight)
                        offsetRight = right;
                if (top >= 0)
                    if (top > offsetTop)
                        offsetTop = top;
                if (bottom >= 0)
                    if (bottom > offsetBottom)
                        offsetBottom = bottom;
            }
        }
    }

    for (int row = offsetTop; row < copyRead.GetHeight() - offsetBottom; row++)
    {
        for (int col = offsetLeft; col < copyRead.GetWidth() - offsetRight; col++)
        {
            auto pix = copyRead.GetPixel(col, row);
            if (pix.brightnes == BLACK)
            {
                bool elemntOutside = false;
                for (int y = row - offsetTop; y < row + offsetBottom + 1; y++)
                {
                    for (int x = col - offsetLeft; x < col + offsetRight + 1; x++)
                    {
                        auto neighbourPix = copyRead.GetPixel(x, y);
                        if (elemntCopy[y - row + erosionElementSize / 2][x - col + erosionElementSize / 2])
                        {
                            // erese pixel, not all element fields are black
                            if (neighbourPix.brightnes == WHITE)
                            {
                                elemntOutside = true;
                                break;
                            }
                        }
                    }
                    if (elemntOutside)
                        break;
                }
                if (elemntOutside)
                    copy.SetPixelWhite(col, row);
            }
        }
        if (Canceled(outputImage))
            return;
        AutomaticRefresh(outputImage);
    }
    SaveToOutput(outputImage);
}

void Erosion::ResetToDefaults()
{
    for (int32_t r = 0; r < S3x3; r++)
        for (int32_t c = 0; c < S3x3; c++)
            erosionElement3x3[r][c] = false;
    for (int32_t r = 0; r < S5x5; r++)
        for (int32_t c = 0; c < S5x5; c++)
            erosionElement5x5[r][c] = false;
    for (int32_t r = 0; r < S7x7; r++)
        for (int32_t c = 0; c < S7x7; c++)
            erosionElement7x7[r][c] = false;
}

void Erosion::DrawInputArray()
{
    if (ImGui::BeginTable("Element strukturalny", erosionElementSize, ImGuiTableFlags_Borders))
    {
        for (int row = 0; row < erosionElementSize; row++)
        {
            ImGui::PushID(row);
            ImGui::TableNextRow();
            for (int col = 0; col < erosionElementSize; col++)
            {
                ImGui::TableSetColumnIndex(col);
                ImGui::PushItemWidth(ARRAY_INPUT_WIDTH);
                std::string s = "##" + std::to_string(col);
                if (erosionElementSize == S3x3)
                    ImGui::Checkbox(s.c_str(), &erosionElement3x3[row][col]);
                else if (erosionElementSize == S5x5)
                    ImGui::Checkbox(s.c_str(), &erosionElement5x5[row][col]);
                else
                    ImGui::Checkbox(s.c_str(), &erosionElement7x7[row][col]);
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}
