#include "LinearFilter.hpp"
LinearFilter::LinearFilter() { algorithmName = "Filtry Liniowe"; }

void LinearFilter::ParamsMenu()
{
    ImGui::Text("Wybierz rozmiar filtru");
    ImGui::RadioButton("3x3", &linearFilterSize, S3x3);
    ImGui::SameLine();
    ImGui::RadioButton("5x5", &linearFilterSize, S5x5);
    ImGui::SameLine();
    ImGui::RadioButton("7x7", &linearFilterSize, S7x7);

    ImGui::Separator();
    ImGui::Text("Wybierz rodzaj filtru");

    // set to predefined ones

    if (ImGui::RadioButton("Uśredniający", &linearFilterType, Average))
    {
        int32_t tmp3x3[3][3] = AVERAGE_3x3;
        int32_t tmp5x5[5][5] = AVERAGE_5x5;
        int32_t tmp7x7[7][7] = AVERAGE_7x7;
        ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
    }
    ImGui::SameLine();

    if (ImGui::RadioButton("Gauss", &linearFilterType, Gauss))
    {
        int32_t tmp3x3[3][3] = GAUSS_3x3;
        int32_t tmp5x5[5][5] = GAUSS_5x5;
        int32_t tmp7x7[7][7] = GAUSS_7x7;
        ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
    }
    ImGui::SameLine();

    if (ImGui::RadioButton("Sobel Poziomy", &linearFilterType, SobelHorizontal))
    {
        int32_t tmp3x3[3][3] = SOBEL_HORIZONTAL_3x3;
        int32_t tmp5x5[5][5] = SOBEL_HORIZONTAL_5x5;
        int32_t tmp7x7[7][7] = SOBEL_HORIZONTAL_7x7;
        ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
    }
    // in new line
    if (ImGui::RadioButton("Sobel Pionowy", &linearFilterType, SobelVertical))
    {
        int32_t tmp3x3[3][3] = SOBEL_VERTICAL_3x3;
        int32_t tmp5x5[5][5] = SOBEL_VERTICAL_5x5;
        int32_t tmp7x7[7][7] = SOBEL_VERTICAL_7x7;
        ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
    }
    ImGui::SameLine();

    if (ImGui::RadioButton("Laplasjan", &linearFilterType, Laplasjan))
    {
        int32_t tmp3x3[3][3] = LAPLACIAN_3x3;
        int32_t tmp5x5[5][5] = LAPLACIAN_5x5;
        int32_t tmp7x7[7][7] = LAPLACIAN_7x7;
        ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
    }
    ImGui::SameLine();

    if (ImGui::RadioButton("Wyostrzający", &linearFilterType, Sharpening))
    {
        int32_t tmp3x3[3][3] = SHARPENING_3x3;
        int32_t tmp5x5[5][5] = SHARPENING_5x5;
        int32_t tmp7x7[7][7] = SHARPENING_7x7;
        ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
    }

    ImGui::RadioButton("Własna", &linearFilterType, CustomL);
    ImGui::Separator();

    if (linearFilterType == CustomL)
    {
        ImGui::Text("Ustaw własną maskę");
        DrawLinearInputArray();
    }
    else
        DrawLinearDisplayArray();
}

void LinearFilter::AlgorithmFunction(Image *outputImage)
{
    // local copy
    CopyToLocalVariable(outputImage);
    // copy - to save
    // copyRead - read
    // this operation require 2 copies, one for readin, the other to save result
    Image copyRead;
    copyRead.CopyNoTexture(copy);
    int32_t offset = linearFilterSize / 2;
    int32_t maskCopy[7][7]; // copy the current mask (there are 3 sizes this way its easier)
    bool normalise = true;
    int32_t maskSum = 0;

    // copy mask (easier this way)
    for (int i = 0; i < linearFilterSize; i++)
        for (int j = 0; j < linearFilterSize; j++)
        {
            if (linearFilterSize == S3x3)
                maskCopy[i][j] = linearMask3x3[i][j];
            else if (linearFilterSize == S5x5)
                maskCopy[i][j] = linearMask5x5[i][j];
            else
                maskCopy[i][j] = linearMask7x7[i][j];

            // all elements must be > 0
            if (maskCopy[i][j] <= 0)
                normalise = false;
            maskSum += maskCopy[i][j];
        }

    for (int row = offset; row < copyRead.GetHeight() - offset; row++)
    {
        for (int col = offset; col < copyRead.GetWidth() - offset; col++)
        {
            auto pix = copyRead.GetPixel(col, row);
            int JR = 0;
            int JG = 0;
            int JB = 0;

            for (int y = 0; y < linearFilterSize; y++)
            {
                for (int x = 0; x < linearFilterSize; x++)
                {
                    auto neighbourPix = copyRead.GetPixel(col + x - offset, row + y - offset);
                    JR += neighbourPix.r * maskCopy[y][x];
                    JG += neighbourPix.g * maskCopy[y][x];
                    JB += neighbourPix.b * maskCopy[y][x];
                }
            }

            if (normalise)
            {
                JR /= maskSum;
                JG /= maskSum;
                JB /= maskSum;
                pix.r = JR;
                pix.g = JG;
                pix.b = JB;
            }
            else
            {
                JR = abs(JR);
                JG = abs(JG);
                JB = abs(JB);
                if (linearFilterType == SobelHorizontal || linearFilterType == SobelVertical || linearFilterType == Laplasjan)
                {
                    JR += 127;
                    JG += 127;
                    JB += 127;
                }
                if (JR > 255)
                    pix.r = 255;
                else
                    pix.r = JR;

                if (JG > 255)
                    pix.g = 255;
                else
                    pix.g = JG;

                if (JB > 255)
                    pix.b = 255;
                else
                    pix.b = JB;
            }
            copy.SetPixel(col, row, pix);
        }
        if (Canceled(outputImage))
            return;
        AutomaticRefresh(outputImage);
    }
    // copy back to output
    SaveToOutput(outputImage);
}

void LinearFilter::ResetToDefaults()
{
    linearFilterType = Average;
    linearFilterSize = S3x3;
    int32_t tmp3x3[3][3] = AVERAGE_3x3;
    int32_t tmp5x5[5][5] = AVERAGE_5x5;
    int32_t tmp7x7[7][7] = AVERAGE_7x7;
    ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
}

void LinearFilter::Save(std::ofstream &file)
{
    file << "linearFilterType" << CONFIG_SPLIT_CHAR_A << linearFilterType << std::endl;
    file << "linearFilterSize" << CONFIG_SPLIT_CHAR_A << linearFilterSize << std::endl;

    for (int size = 3; size <= 7; size += 2)
    {
        file << "linerMaskSize" << size << CONFIG_SPLIT_CHAR_A;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (size == 3)
                    file << linearMask3x3[i][j];
                else if (size == 5)
                    file << linearMask5x5[i][j];
                else
                    file << linearMask7x7[i][j];
                if (j < size - 1)
                    file << CONFIG_COL_DELIM_A;
            }
            if (i < size - 1)
                file << CONFIG_ROW_DELIM_A;
        }
        file << std::endl;
    }
}

void LinearFilter::Load(std::ifstream &file)
{
    try
    {
        linearFilterType = std::stoi(SplitLine(file));
    }
    catch (const std::exception &e)
    {
        linearFilterType = Average;
    }
    try
    {
        linearFilterSize = std::stoi(SplitLine(file));
    }
    catch (const std::exception &e)
    {
        linearFilterSize = S3x3;
    }

    if (linearFilterType == CustomL)
    {
        // read the filter from file
        std::string sub3x3 = SplitLine(file);
        std::string sub5x5 = SplitLine(file);
        std::string sub7x7 = SplitLine(file);
        ParseCustomFilter(sub3x3, 3);
        ParseCustomFilter(sub5x5, 5);
        ParseCustomFilter(sub7x7, 7);
    }
    else
    {
        // skip becouse predefined
        std::string tmp;
        getline(file, tmp, CONFIG_FILE_DELIM_A);
        getline(file, tmp, CONFIG_FILE_DELIM_A);
        getline(file, tmp, CONFIG_FILE_DELIM_A);
        SetPreDefinedFilters();
    }
}

void LinearFilter::ChangeFilter(int32_t tmp3x3[3][3], int32_t tmp5x5[5][5], int32_t tmp7x7[7][7])
{
    std::copy(&tmp3x3[0][0], &tmp3x3[0][0] + 3 * 3, &linearMask3x3[0][0]);
    std::copy(&tmp5x5[0][0], &tmp5x5[0][0] + 5 * 5, &linearMask5x5[0][0]);
    std::copy(&tmp7x7[0][0], &tmp7x7[0][0] + 7 * 7, &linearMask7x7[0][0]);
}

void LinearFilter::DrawLinearInputArray()
{
    if (ImGui::BeginTable("Maska", linearFilterSize, ImGuiTableFlags_Borders))
    {
        for (int row = 0; row < linearFilterSize; row++)
        {
            ImGui::PushID(row);
            ImGui::TableNextRow();
            for (int col = 0; col < linearFilterSize; col++)
            {
                ImGui::TableSetColumnIndex(col);
                ImGui::PushItemWidth(ARRAY_INPUT_WIDTH);
                std::string s = "##" + std::to_string(col);
                if (linearFilterSize == S3x3)
                    ImGui::InputInt(s.c_str(), &linearMask3x3[row][col]);
                else if (linearFilterSize == S5x5)
                    ImGui::InputInt(s.c_str(), &linearMask5x5[row][col]);
                else
                    ImGui::InputInt(s.c_str(), &linearMask7x7[row][col]);
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}

void LinearFilter::DrawLinearDisplayArray()
{
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - (ARRAY_FIELD_WIDTH * linearFilterSize) / 2);
    if (ImGui::BeginTable("Maska", linearFilterSize, ImGuiTableFlags_Borders, ImVec2(ARRAY_FIELD_WIDTH * linearFilterSize, 0)))
    {
        for (int i = 0; i < linearFilterSize; i++)
            ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_WidthFixed, ARRAY_ITEM_WIDTH);
        for (int row = 0; row < linearFilterSize; row++)
        {
            ImGui::TableNextRow();
            for (int col = 0; col < linearFilterSize; col++)
            {
                ImGui::TableSetColumnIndex(col);
                if (linearFilterSize == S3x3)
                    ImGui::Text("%d", linearMask3x3[row][col]);
                else if (linearFilterSize == S5x5)
                    ImGui::Text("%d", linearMask5x5[row][col]);
                else
                    ImGui::Text("%d", linearMask7x7[row][col]);
            }
        }
        ImGui::EndTable();
    }
}

void LinearFilter::SetPreDefinedFilters()
{
    if (linearFilterType == Average)
    {
        int32_t tmp3x3[3][3] = AVERAGE_3x3;
        int32_t tmp5x5[5][5] = AVERAGE_5x5;
        int32_t tmp7x7[7][7] = AVERAGE_7x7;
        ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
    }

    if (linearFilterType == Gauss)
    {
        int32_t tmp3x3[3][3] = GAUSS_3x3;
        int32_t tmp5x5[5][5] = GAUSS_5x5;
        int32_t tmp7x7[7][7] = GAUSS_7x7;
        ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
    }

    if (linearFilterType == SobelHorizontal)
    {
        int32_t tmp3x3[3][3] = SOBEL_HORIZONTAL_3x3;
        int32_t tmp5x5[5][5] = SOBEL_HORIZONTAL_5x5;
        int32_t tmp7x7[7][7] = SOBEL_HORIZONTAL_7x7;
        ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
    }

    if (linearFilterType == SobelVertical)
    {
        int32_t tmp3x3[3][3] = SOBEL_VERTICAL_3x3;
        int32_t tmp5x5[5][5] = SOBEL_VERTICAL_5x5;
        int32_t tmp7x7[7][7] = SOBEL_VERTICAL_7x7;
        ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
    }

    if (linearFilterType == Laplasjan)
    {
        int32_t tmp3x3[3][3] = LAPLACIAN_3x3;
        int32_t tmp5x5[5][5] = LAPLACIAN_5x5;
        int32_t tmp7x7[7][7] = LAPLACIAN_7x7;
        ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
    }

    if (linearFilterType == Sharpening)
    {
        int32_t tmp3x3[3][3] = SHARPENING_3x3;
        int32_t tmp5x5[5][5] = SHARPENING_5x5;
        int32_t tmp7x7[7][7] = SHARPENING_7x7;
        ChangeFilter(tmp3x3, tmp5x5, tmp7x7);
    }
}

void LinearFilter::ParseCustomFilter(std::string line, int size)
{
    int rowSplitCount = 0;
    int colSplitCount = 0;
    for (size_t it = 0; it < line.size(); it++)
    {
        if (line[it] == CONFIG_ROW_DELIM_A)
            rowSplitCount++;
        if (line[it] == CONFIG_COL_DELIM_A)
            colSplitCount++;
    }
    // invalid line skip
    if(rowSplitCount != size - 1 || colSplitCount != size * size - size)
        return;

    for (int i = 0; i < size; i++)
    {
        std::string row;
        // the last does not need this
        if (i < size - 1)
        {
            row = line.substr(0, line.find(CONFIG_ROW_DELIM_A));
            line = line.substr(line.find(CONFIG_ROW_DELIM_A) + 1);
        }
        else
            row = line;

        for (int j = 0; j < size; j++)
        {
            std::string col;
            if (j < size - 1)
            {
                col = row.substr(0, row.find(CONFIG_COL_DELIM_A));
                row = row.substr(row.find(CONFIG_COL_DELIM_A) + 1);
            }
            else
                col = row;
            if (size == 3)
            {
                try
                {
                    linearMask3x3[i][j] = std::stoi(col);
                }
                catch (const std::exception &e)
                {
                    // default 1
                    linearMask3x3[i][j] = 1;
                }
            }
            else if (size == 5)
            {
                try
                {
                    linearMask5x5[i][j] = std::stoi(col);
                }
                catch (const std::exception &e)
                {
                    // default 1
                    linearMask5x5[i][j] = 1;
                }
            }
            else
            {
                try
                {
                    linearMask7x7[i][j] = std::stoi(col);
                }
                catch (const std::exception &e)
                {
                    // default 1
                    linearMask7x7[i][j] = 1;
                }
            }
        }
    }
}