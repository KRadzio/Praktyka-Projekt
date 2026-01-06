#include "Hought.hpp"

Hought::Hought()
{
    autoRefresh = false;
    algorithmName = "Transformacja Houghta";
}

void Hought::ParamsMenu() { ImGui::Text("Parametry wynikowe"); }

void Hought::AlgorithmFunction(Image *outputImage)
{
    // local copy
    CopyToLocalVariable(outputImage);
    roMax = sqrt(copy.GetHeight() * copy.GetHeight() + copy.GetWidth() * copy.GetWidth());

    acumulator = std::vector<std::vector<int32_t>>(2 * roMax);
    for (int i = 0; i < 2 * roMax; i++)
        acumulator[i] = std::vector<int32_t>(THETA_NUM);

    for (int i = 0; i < 2 * roMax; i++)
        for (int j = 0; j < THETA_NUM; j++)
            acumulator[i][j] = 0;

    for (int row = 0; row < copy.GetHeight(); row++)
    {
        for (int col = 0; col < copy.GetWidth(); col++)
        {
            auto pix = copy.GetPixel(col, row);
            if (pix.brightnes == BLACK)
            {
                int currRo;
                for (int t = 0; t < THETA_NUM; t++)
                {
                    currRo = col * cos(t * (M_PIf / 180)) + row * sin(t * (M_PIf / 180));
                    acumulator[currRo + roMax][t]++;
                    if (acumulator[currRo + roMax][t] > maxHoughtVal)
                    {
                        maxHoughtVal = acumulator[currRo + roMax][t];
                        maxIndexTheta = t;
                        maxIndexRo = currRo;
                    }
                }
            }
        }
        if (Canceled(outputImage))
            return;
        // refresh for every line
        ManualRefresh(outputImage);
    }

    // refresh after the end
    // copy back to output
    SaveToOutput(outputImage);
}

void Hought::ResetToDefaults()
{
    maxIndexRo = 0;
    maxIndexTheta = 0;
    maxHoughtVal = INT32_MIN;
    roMax = 0;
    ClearAcumulator();
}

void Hought::CopyToLocalVariable(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    copy.CopyNoTexture(*outputImage);
    // set output to a black surface
    outputImage->SetBlankSurfaceNoTexture(THETA_NUM, 2 * (int32_t)sqrt(copy.GetHeight() * copy.GetHeight() + copy.GetWidth() * copy.GetWidth()));
    Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
    Mutex::GetInstance().Unlock();
}

bool Hought::Canceled(Image *outputImage)
{
    Mutex::GetInstance().Lock();

    if (!Mutex::GetInstance().IsThreadRunning())
    {
        Mutex::GetInstance().Unlock();
        CopyAcumulator(outputImage);
        copy.ClearImageNoTexture();
        ClearAcumulator();
        return true;
    }
    Mutex::GetInstance().Unlock();
    return false;
}

void Hought::ManualRefresh(Image *outputImage)
{
    CopyAcumulator(outputImage);

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
    Mutex::GetInstance().Unlock();
}

void Hought::SaveToOutput(Image *outputImage)
{

    CopyAcumulator(outputImage);

    ClearAcumulator();

    copy.ClearImageNoTexture();

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Hought::CopyAcumulator(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    for (int i = 0; i < 2 * roMax; i++)
        for (int j = 0; j < THETA_NUM; j++)
        {
            float tmp = acumulator[i][j];
            tmp /= maxHoughtVal;
            tmp *= 255;
            Image::Pixel pix;
            pix.g = 255 - tmp;
            pix.b = 255 - tmp;
            pix.r = 255 - tmp;
            outputImage->SetPixel(j, i, pix);
        }
    Mutex::GetInstance().Unlock();
}

void Hought::ClearAcumulator()
{
    for (int i = 0; i < 2 * roMax; i++)
        acumulator[i].clear();
    acumulator.clear();
}
