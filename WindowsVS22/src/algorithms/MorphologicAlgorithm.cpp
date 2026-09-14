#include "MorphologicAlgorithm.hpp"

MorphologicAlgorithm::MorphologicAlgorithm() {}

void MorphologicAlgorithm::ParamsMenu()
{
    ImGui::Text("Wybierz rozmiar");
    ImGui::RadioButton("3x3", &elementSize, S3x3);
    ImGui::SameLine();
    ImGui::RadioButton("5x5", &elementSize, S5x5);
    ImGui::SameLine();
    ImGui::RadioButton("7x7", &elementSize, S7x7);
    ImGui::Separator();
    ImGui::Text("Zaznacz pola, które mają \nbyć elementem strukturalnym");
    DrawInputArray();
}

void MorphologicAlgorithm::ResetToDefaults()
{
    elementSize = S3x3;
    for (int32_t r = 0; r < S3x3; r++)
        for (int32_t c = 0; c < S3x3; c++)
            element3x3[r][c] = false;
    for (int32_t r = 0; r < S5x5; r++)
        for (int32_t c = 0; c < S5x5; c++)
            element5x5[r][c] = false;
    for (int32_t r = 0; r < S7x7; r++)
        for (int32_t c = 0; c < S7x7; c++)
            element7x7[r][c] = false;
}

void MorphologicAlgorithm::DrawInputArray()
{
    if (ImGui::BeginTable("Element strukturalny", elementSize, ImGuiTableFlags_Borders))
    {
        for (int row = 0; row < elementSize; row++)
        {
            ImGui::PushID(row);
            ImGui::TableNextRow();
            for (int col = 0; col < elementSize; col++)
            {
                ImGui::TableSetColumnIndex(col);
                ImGui::PushItemWidth(ARRAY_INPUT_WIDTH);
                std::string s = "##" + std::to_string(col);
                if (elementSize == S3x3)
                    ImGui::Checkbox(s.c_str(), &element3x3[row][col]);
                else if (elementSize == S5x5)
                    ImGui::Checkbox(s.c_str(), &element5x5[row][col]);
                else
                    ImGui::Checkbox(s.c_str(), &element7x7[row][col]);
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}

void MorphologicAlgorithm::CalculateOffsets()
{

    // copy element
    for (int i = 0; i < elementSize; i++)
        for (int j = 0; j < elementSize; j++)
        {
            if (elementSize == S3x3)
                elemntCopy[i][j] = element3x3[i][j];
            else if (elementSize == S5x5)
                elemntCopy[i][j] = element5x5[i][j];
            else
                elemntCopy[i][j] = element7x7[i][j];
        }

    // calculate offsets
    for (int row = 0; row < elementSize; row++)
    {
        for (int col = 0; col < elementSize; col++)
        {
            if (elemntCopy[row][col])
            {
                int left = elementSize / 2 - col;
                int right = col - elementSize / 2;
                int top = elementSize / 2 - row;
                int bottom = row - elementSize / 2;
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
}

void MorphologicAlgorithm::ErosionFunc(Image *outputImage)
{

    // CopyToLocalVariable(outputImage);

    copyRead.CopyNoTexture(copy);

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
                        if (elemntCopy[y - row + elementSize / 2][x - col + elementSize / 2])
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
    // SaveToOutput(outputImage);
}

void MorphologicAlgorithm::DilatationFunc(Image *outputImage)
{

    // CopyToLocalVariable(outputImage);

    copyRead.CopyNoTexture(copy);

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
                        if (elemntCopy[y - row + elementSize / 2][x - col + elementSize / 2])
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
                            if (elemntCopy[y - row + elementSize / 2][x - col + elementSize / 2])
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
    // SaveToOutput(outputImage);
}
