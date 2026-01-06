#include "MedianFilter.hpp"

MedianFilter::MedianFilter() { algorithmName = "Filtry medianowe"; }

void MedianFilter::ParamsMenu()
{
    ImGui::Text("Wybierz rozmiar filtru");
    ImGui::RadioButton("3x3", &medianFilterSize, S3x3);
    ImGui::SameLine();
    ImGui::RadioButton("5x5", &medianFilterSize, S5x5);
    ImGui::SameLine();
    ImGui::RadioButton("7x7", &medianFilterSize, S7x7);
    ImGui::Separator();
    ImGui::Text("Wybierz rodzaj filtru");
    // set to predefined
    if (ImGui::RadioButton("Pełny", &medianFilterType, Full))
    {
        bool tmp3x3[3][3] = MEDIAN_3x3;
        bool tmp5x5[5][5] = MEDIAN_5x5;
        bool tmp7x7[7][7] = MEDIAN_7x7;
        ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Krzyżowy", &medianFilterType, Cross))
    {
        bool tmp3x3[3][3] = MEDIAN_CROSS_3x3;
        bool tmp5x5[5][5] = MEDIAN_CROSS_5x5;
        bool tmp7x7[7][7] = MEDIAN_CROSS_7x7;
        ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
    }
    ImGui::SameLine();
    ImGui::RadioButton("Własna", &medianFilterType, CustomM);

    ImGui::Separator();
    if (medianFilterType == CustomM)
    {

        ImGui::Text("Ustaw własną maskę");
        DrawInputArray();
    }
    else
        DrawMedianDisplayArray();
}

void MedianFilter::AlgorithmFunction(Image *outputImage)
{
    // local copy
    CopyToLocalVariable(outputImage);
    // copy - to save
    // copyRead - read
    // this operation require 2 copies, one for readin, the other to save result
    Image copyRead; 
    copyRead.CopyNoTexture(copy);
    int32_t offset = medianFilterSize / 2;
    bool maskCopy[7][7]; // copy the current mask (there are 3 sizes this way its easier)

    // copy mask
    for (int i = 0; i < medianFilterSize; i++)
        for (int j = 0; j < medianFilterSize; j++)
        {
            if (medianFilterSize == S3x3)
                maskCopy[i][j] = medianMask3x3[i][j];
            else if (medianFilterSize == S5x5)
                maskCopy[i][j] = medianMask5x5[i][j];
            else
                maskCopy[i][j] = medianMask7x7[i][j];
        }


    for (int row = offset; row < copyRead.GetHeight() - offset; row++)
    {
        for (int col = offset; col < copyRead.GetWidth() - offset; col++)
        {
            auto pix = copyRead.GetPixel(col, row);
            std::vector<int> JR;
            std::vector<int> JG;
            std::vector<int> JB;

            for (int y = 0; y < medianFilterSize; y++)
            {
                for (int x = 0; x < medianFilterSize; x++)
                {
                    auto neighbourPix = copyRead.GetPixel(col + x - offset, row + y - offset);
                    if (maskCopy[y][x])
                    {
                        JR.emplace(JR.end(), neighbourPix.r);
                        JG.emplace(JG.end(), neighbourPix.g);
                        JB.emplace(JB.end(), neighbourPix.b);
                    }
                }
            }

            std::sort(JR.begin(), JR.end());
            std::sort(JG.begin(), JG.end());
            std::sort(JB.begin(), JB.end());

            pix.r = JR[JR.size() / 2];
            pix.g = JG[JG.size() / 2];
            pix.b = JB[JB.size() / 2];

            copy.SetPixel(col, row, pix);
        }
        if (Canceled(outputImage))
            return;
        AutomaticRefresh(outputImage);
    }
    SaveToOutput(outputImage);
}

void MedianFilter::ResetToDefaults()
{
    medianFilterType = Full;
    medianFilterSize = S3x3;
    bool tmp3x3[3][3] = MEDIAN_3x3;
    bool tmp5x5[5][5] = MEDIAN_5x5;
    bool tmp7x7[7][7] = MEDIAN_7x7;
    ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
}

void MedianFilter::ChangeFilter(bool tmp3x3[3][3], bool tmp5x5[5][5], bool tmp7x7[7][7])
{
    std::copy(&tmp3x3[0][0], &tmp3x3[0][0] + 3 * 3, &medianMask3x3[0][0]);
    std::copy(&tmp5x5[0][0], &tmp5x5[0][0] + 5 * 5, &medianMask5x5[0][0]);
    std::copy(&tmp7x7[0][0], &tmp7x7[0][0] + 7 * 7, &medianMask7x7[0][0]);
}

void MedianFilter::DrawMedianDisplayArray()
{
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - (ARRAY_FIELD_WIDTH * medianFilterSize) / 2);
    if (ImGui::BeginTable("Maska", medianFilterSize, ImGuiTableFlags_Borders, ImVec2(ARRAY_FIELD_WIDTH * medianFilterSize, 0)))
    {
        for (int i = 0; i < medianFilterSize; i++)
            ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_WidthFixed, ARRAY_ITEM_WIDTH);
        for (int row = 0; row < medianFilterSize; row++)
        {
            ImGui::TableNextRow();
            for (int col = 0; col < medianFilterSize; col++)
            {
                ImGui::TableSetColumnIndex(col);
                if (medianFilterSize == S3x3)
                    ImGui::Text("%d", medianMask3x3[row][col]);
                else if (medianFilterSize == S5x5)
                    ImGui::Text("%d", medianMask5x5[row][col]);
                else
                    ImGui::Text("%d", medianMask7x7[row][col]);
            }
        }
        ImGui::EndTable();
    }
}

void MedianFilter::DrawInputArray()
{
    if (ImGui::BeginTable("Maska", medianFilterSize, ImGuiTableFlags_Borders))
    {
        for (int row = 0; row < medianFilterSize; row++)
        {
            ImGui::PushID(row);
            ImGui::TableNextRow();
            for (int col = 0; col < medianFilterSize; col++)
            {
                ImGui::TableSetColumnIndex(col);
                ImGui::PushItemWidth(ARRAY_INPUT_WIDTH);
                std::string s = "##" + std::to_string(col);
                if (medianFilterSize == S3x3)
                    ImGui::Checkbox(s.c_str(), &medianMask3x3[row][col]);
                else if (medianFilterSize == S5x5)
                    ImGui::Checkbox(s.c_str(), &medianMask5x5[row][col]);
                else
                    ImGui::Checkbox(s.c_str(), &medianMask7x7[row][col]);
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}