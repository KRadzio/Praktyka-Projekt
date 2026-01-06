#include "Dilatation.hpp"
Dilatation::Dilatation()
{
    algorithmName = "Dylatacja";
    ResetToDefaults();
}

void Dilatation::ParamsMenu()
{
    ImGui::Text("Wybierz rozmiar");
    ImGui::RadioButton("3x3", &dilatationElementSize, S3x3);
    ImGui::SameLine();
    ImGui::RadioButton("5x5", &dilatationElementSize, S5x5);
    ImGui::SameLine();
    ImGui::RadioButton("7x7", &dilatationElementSize, S7x7);
    ImGui::Separator();
    ImGui::Text("Zaznacz pola, które mają \nbyć elementem strukturalnym");
    DrawInputArray();
}

void Dilatation::AlgorithmFunction(Image *outputImage)
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
    for (int i = 0; i < dilatationElementSize; i++)
        for (int j = 0; j < dilatationElementSize; j++)
        {
            if (dilatationElementSize == S3x3)
                elemntCopy[i][j] = dilatationElement3x3[i][j];
            else if (dilatationElementSize == S5x5)
                elemntCopy[i][j] = dilatationElement5x5[i][j];
            else
                elemntCopy[i][j] = dilatationElement7x7[i][j];
        }

    Mutex::GetInstance().Unlock();

    // calculate offsets
    for (int row = 0; row < dilatationElementSize; row++)
    {
        for (int col = 0; col < dilatationElementSize; col++)
        {
            if (elemntCopy[row][col])
            {
                int left = dilatationElementSize / 2 - col;
                int right = col - dilatationElementSize / 2;
                int top = dilatationElementSize / 2 - row;
                int bottom = row - dilatationElementSize / 2;
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
                        if (elemntCopy[y - row + dilatationElementSize / 2][x - col + dilatationElementSize / 2])
                        {
                            // element outside
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
                // set all outside pixels to black
                if (elemntOutside)
                {
                    for (int y = row - offsetTop; y < row + offsetBottom + 1; y++)
                    {
                        for (int x = col - offsetLeft; x < col + offsetRight + 1; x++)
                        {
                            if (elemntCopy[y - row + dilatationElementSize / 2][x - col + dilatationElementSize / 2])
                                copy.SetPixelBlack(x, y);
                        }
                    }
                }
            }
        }
        if (Canceled(outputImage))
            return;
        AutomaticRefresh(outputImage);
    }
    // copy back to output
    SaveToOutput(outputImage);
}

void Dilatation::ResetToDefaults()
{
    for (int32_t r = 0; r < S3x3; r++)
        for (int32_t c = 0; c < S3x3; c++)
            dilatationElement3x3[r][c] = false;
    for (int32_t r = 0; r < S5x5; r++)
        for (int32_t c = 0; c < S5x5; c++)
            dilatationElement5x5[r][c] = false;
    for (int32_t r = 0; r < S7x7; r++)
        for (int32_t c = 0; c < S7x7; c++)
            dilatationElement7x7[r][c] = false;
}

void Dilatation::DrawInputArray()
{
    if (ImGui::BeginTable("Element strukturalny", dilatationElementSize, ImGuiTableFlags_Borders))
    {
        for (int row = 0; row < dilatationElementSize; row++)
        {
            ImGui::PushID(row);
            ImGui::TableNextRow();
            for (int col = 0; col < dilatationElementSize; col++)
            {
                ImGui::TableSetColumnIndex(col);
                ImGui::PushItemWidth(ARRAY_INPUT_WIDTH);
                std::string s = "##" + std::to_string(col);
                if (dilatationElementSize == S3x3)
                    ImGui::Checkbox(s.c_str(), &dilatationElement3x3[row][col]);
                else if (dilatationElementSize == S5x5)
                    ImGui::Checkbox(s.c_str(), &dilatationElement5x5[row][col]);
                else
                    ImGui::Checkbox(s.c_str(), &dilatationElement7x7[row][col]);
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}