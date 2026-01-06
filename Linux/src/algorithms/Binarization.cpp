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

    if (method == None)
    {
        for (int row = 0; row < copy.GetHeight(); row++)
        {
            for (int col = 0; col < copy.GetWidth(); col++)
            {

                Image::Pixel pix = copy.GetPixel(col, row);

                if (boundCount == 1)
                {
                    if (pix.brightnes < lowerBound)
                    {
                        pix.b = 0;
                        pix.g = 0;
                        pix.r = 0;
                    }
                    else
                    {
                        pix.b = 255;
                        pix.g = 255;
                        pix.r = 255;
                    }
                }
                else
                {
                    if (pix.brightnes > lowerBound && pix.brightnes < upperBound)
                    {
                        pix.b = 0;
                        pix.g = 0;
                        pix.r = 0;
                    }
                    else
                    {
                        pix.b = 255;
                        pix.g = 255;
                        pix.r = 255;
                    }
                }

                copy.SetPixel(col, row, pix);
            }
            if (Canceled(outputImage))
                return;
            AutomaticRefresh(outputImage);
        }
    }
    else if (method == Gradient)
    {
        float G = 0;
        float Gx = 0;
        float Gy = 0;
        float sum_JG = 0;
        float sum_G = 0;

        for (int row = 1; row < copy.GetHeight() - 1; row++)
        {
            for (int col = 1; col < copy.GetWidth() - 1; col++)
            {
                Image::Pixel pix = copy.GetPixel(col, row);
                Gx = copy.GetPixel(col + 1, row).brightnes - copy.GetPixel(col - 1, row).brightnes;
                Gy = copy.GetPixel(col, row + 1).brightnes - copy.GetPixel(col, row - 1).brightnes;
                G = std::max(abs(Gx), abs(Gy));
                sum_G += G;
                sum_JG += pix.brightnes * G;
            }
            if (Canceled(outputImage))
                return;
        }
        lowerBound = sum_JG / sum_G;
    }
    // iter
    else
    {
        uint8_t JMax = 255;
        uint8_t JMin = 0;
        int t = 0;
        int t_old = 0;
        bool end = false;
        float p[MAX_VAL];
        copy.CopyNormalisedBrightnessHistogram(p);

        for (int i = 0; i < MAX_VAL; i++)
            if (p[i] > 0)
            {
                JMin = i;
                break;
            }

        for (int i = 255; i >= 0; i--)
            if (p[i] > 0)
            {
                JMax = i;
                break;
            }

        t = (JMax - JMin) / 2;
        t_old = t;
        while (!end)
        {
            float m0 = 0;
            float P0 = 0;

            float m1 = 0;
            float P1 = 0;

            for (int i = 0; i < t; i++)
            {
                m0 += i * p[i];
                P0 += p[i];
            }

            m0 /= P0;

            for (int i = t; i < MAX_VAL; i++)
            {
                m1 += i * p[i];
                P1 += p[i];
            }

            m1 /= P1;

            t_old = t;
            t = (m0 + m1) / 2;

            if (abs(t_old - t) < 2)
            {
                end = true;
                lowerBound = t;
            }

            if (Canceled(outputImage))
                return;
        }
    }

    if (method != None)
    {
        for (int row = 0; row < copy.GetHeight(); row++)
        {
            for (int col = 0; col < copy.GetWidth(); col++)
            {
                Image::Pixel pix = copy.GetPixel(col, row);
                if (pix.brightnes < lowerBound)
                    copy.SetPixelBlack(col, row);
                else
                    copy.SetPixelWhite(col, row);
            }

            if (Canceled(outputImage))
                return;
            AutomaticRefresh(outputImage);
        }
    }
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