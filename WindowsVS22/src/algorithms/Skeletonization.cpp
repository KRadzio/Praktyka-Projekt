#include "Skeletonization.hpp"
Skeletonization::Skeletonization()
{
    autoRefresh = false;
    algorithmName = "Szkieletyzacja";
}

void Skeletonization::ParamsMenu() { ImGui::Text("Brak parametrów do tego algorytmu"); }

void Skeletonization::AlgorithmFunction(Image *outputImage) {
     // neighbours
    int32_t S[8];
    bool remain = true;
    bool skel = false;
    // local copy
    CopyToLocalVariable(outputImage);

    while (remain)
    {
        remain = false;
        for (int i = 0; i < 8; i += 2)
        {
            for (int row = 1; row < copy.GetHeight() - 1; row++)
            {
                for (int col = 1; col < copy.GetWidth() - 1; col++)
                {
                    auto pix = copy.GetPixel(col, row);
                    // set neighbours
                    S[0] = copy.GetPixel(col + 1, row).brightnes;
                    S[1] = copy.GetPixel(col + 1, row - 1).brightnes;
                    S[2] = copy.GetPixel(col, row - 1).brightnes;
                    S[3] = copy.GetPixel(col - 1, row - 1).brightnes;
                    S[4] = copy.GetPixel(col - 1, row).brightnes;
                    S[5] = copy.GetPixel(col - 1, row + 1).brightnes;
                    S[6] = copy.GetPixel(col, row + 1).brightnes;
                    S[7] = copy.GetPixel(col + 1, row + 1).brightnes;

                    if ((pix.brightnes == BLACK) && (S[i] == WHITE))
                    {
                        bool W1 = W1_CHECK;
                        bool W2 = W2_CHECK;
                        bool W3 = W3_CHECK;
                        bool W4 = W4_CHECK;
                        bool W5 = W5_CHECK;
                        bool W6 = W6_CHECK;
                        skel = W1 || W2 || W3 || W4 || W5 || W6;
                        if (skel)
                        {
                            Image::Pixel pix;
                            pix.b = SKELETON;
                            pix.g = SKELETON;
                            pix.r = SKELETON;
                            copy.SetPixel(col,row,pix);
                        }
                        else
                        {
                           Image::Pixel pix;
                            pix.b = REMOVE;
                            pix.g = REMOVE;
                            pix.r = REMOVE;
                            copy.SetPixel(col,row,pix);
                            remain = true;
                        }
                    }
                }
            }
            for (int row = 0; row < copy.GetHeight(); row++)
                for (int col = 0; col < copy.GetWidth(); col++)
                    if (copy.GetPixel(col,row).brightnes == REMOVE)
                        copy.SetPixelWhite(col, row);
        }
        if(Canceled(outputImage))
            return;
        ManualRefresh(outputImage);
    }
    SaveToOutput(outputImage);
}

void Skeletonization::ResetToDefaults() {} // NO PARAMS TO RESET