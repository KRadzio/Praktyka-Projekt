#include "App.hpp"

void Algorithms::CreateNegative(Image *outputImage)
{
    Mutex::GetInstance().Lock();

    if (outputImage->NoSurface())
    {
        Mutex::GetInstance().SetOutputCode(Mutex::Error);
        Mutex::GetInstance().ThreadStopped();
        Mutex::GetInstance().Unlock();
        return;
    }
    Mutex::GetInstance().Unlock();

    for (int i = 0; i < outputImage->GetWidth(); i++)
    {
        Mutex::GetInstance().Lock();
        for (int j = 0; j < outputImage->GetHeight(); j++)
        {
            auto pix = outputImage->GetPixel(i, j);
            pix.b = 255 - pix.b;
            pix.g = 255 - pix.g;
            pix.r = 255 - pix.r;
            outputImage->SetPixel(i, j, pix);
        }
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            Mutex::GetInstance().SetOutputCode(Mutex::Stoped);
            Mutex::GetInstance().Unlock();
            return;
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().SetOutputCode(Mutex::Normal);
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Algorithms::BrightenImage(Image *outputImage, ParametersStruct *params)
{
    Mutex::GetInstance().Lock();

    if (outputImage->NoSurface())
    {
        Mutex::GetInstance().SetOutputCode(Mutex::Error);
        Mutex::GetInstance().ThreadStopped();
        Mutex::GetInstance().Unlock();
        return;
    }
    Mutex::GetInstance().Unlock();

    for (int i = 0; i < outputImage->GetWidth(); i++)
    {
        Mutex::GetInstance().Lock();
        for (int j = 0; j < outputImage->GetHeight(); j++)
        {
            auto pix = outputImage->GetPixel(i, j);

            if (pix.b + params->value > 255)
                pix.b = 255;
            else if (pix.b + params->value < 0)
                pix.b = 0;
            else
                pix.b += params->value;

            if (pix.g + params->value > 255)
                pix.g = 255;
            else if (pix.g + params->value < 0)
                pix.g = 0;
            else
                pix.g += params->value;

            if (pix.r + params->value > 255)
                pix.r = 255;
            else if (pix.r + params->value < 0)
                pix.r = 0;
            else
                pix.r += params->value;

            outputImage->SetPixel(i, j, pix);
        }
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            Mutex::GetInstance().SetOutputCode(Mutex::Stoped);
            Mutex::GetInstance().Unlock();
            return;
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().SetOutputCode(Mutex::Normal);
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Contrast(Image *outputImage, ParametersStruct *params)
{
    Mutex::GetInstance().Lock();

    if (outputImage->NoSurface())
    {
        Mutex::GetInstance().SetOutputCode(Mutex::Error);
        Mutex::GetInstance().ThreadStopped();
        Mutex::GetInstance().Unlock();
        return;
    }
    Mutex::GetInstance().Unlock();

    for (int i = 0; i < outputImage->GetWidth(); i++)
    {
        Mutex::GetInstance().Lock();
        for (int j = 0; j < outputImage->GetHeight(); j++)
        {
            auto pix = outputImage->GetPixel(i, j);

            if (pix.b * params->contrast > 255)
                pix.b = 255;
            else
                pix.b *= params->contrast;

            if (pix.g * params->contrast > 255)
                pix.g = 255;
            else
                pix.g *= params->contrast;

            if (pix.r * params->contrast > 255)
                pix.r = 255;
            else
                pix.r *= params->contrast;
            outputImage->SetPixel(i, j, pix);
        }
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            Mutex::GetInstance().SetOutputCode(Mutex::Stoped);
            Mutex::GetInstance().Unlock();
            return;
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().SetOutputCode(Mutex::Normal);
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Exponentiation(Image *outputImage, ParametersStruct *params)
{
    Mutex::GetInstance().Lock();

    if (outputImage->NoSurface())
    {
        Mutex::GetInstance().SetOutputCode(Mutex::Error);
        Mutex::GetInstance().ThreadStopped();
        Mutex::GetInstance().Unlock();
        return;
    }

    int tab[256];
    for (int i = 0; i < 256; i++)
        tab[i] = 255.0 * pow((float)i / 255.0, params->alfa);

    Mutex::GetInstance().Unlock();

    for (int i = 0; i < outputImage->GetWidth(); i++)
    {
        Mutex::GetInstance().Lock();
        for (int j = 0; j < outputImage->GetHeight(); j++)
        {
            auto pix = outputImage->GetPixel(i, j);

            pix.b = tab[pix.b];
            pix.g = tab[pix.g];
            pix.r = tab[pix.r];
            outputImage->SetPixel(i, j, pix);
        }
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            Mutex::GetInstance().SetOutputCode(Mutex::Stoped);
            Mutex::GetInstance().Unlock();
            return;
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().SetOutputCode(Mutex::Normal);
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Algorithms::LevelHistogram(Image *outputImage)
{
    Mutex::GetInstance().Lock();

    if (outputImage->NoSurface())
    {
        Mutex::GetInstance().SetOutputCode(Mutex::Error);
        Mutex::GetInstance().ThreadStopped();
        Mutex::GetInstance().Unlock();
        return;
    }

    float *distR = outputImage->GetDistributorR();
    float *distG = outputImage->GetDistributorG();
    float *distB = outputImage->GetDistributorB();

    Mutex::GetInstance().Unlock();

    for (int i = 0; i < outputImage->GetWidth(); i++)
    {
        for (int j = 0; j < outputImage->GetHeight(); j++)
        {
            Mutex::GetInstance().Lock();
            Image::Pixel pix = outputImage->GetPixel(i, j);
            pix.b = distB[pix.b];
            pix.g = distG[pix.g];
            pix.r = distR[pix.r];
            outputImage->SetPixel(i, j, pix);
            Mutex::GetInstance().Unlock();
        }
        Mutex::GetInstance().Lock();
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            Mutex::GetInstance().SetOutputCode(Mutex::Stoped);
            Mutex::GetInstance().Unlock();
            return;
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().SetOutputCode(Mutex::Normal);
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Binarization(Image *outputImage, ParametersStruct *params)
{
    int method;
    int lowerBound;
    int upperBound;
    int boundCount;
    Mutex::GetInstance().Lock();

    if (outputImage->NoSurface())
    {
        Mutex::GetInstance().SetOutputCode(Mutex::Error);
        Mutex::GetInstance().ThreadStopped();
        Mutex::GetInstance().Unlock();
        return;
    }

    // copy to local vars
    method = params->method;
    boundCount = params->boundCount;
    if (method == None)
    {
        if (params->boundCount == 1)
            lowerBound = params->lowerBound;
        else
        {
            lowerBound = params->lowerBound;
            upperBound = params->upperBound;
        }
    }

    Mutex::GetInstance().Unlock();

    if (method == None)
    {
        for (int i = 0; i < outputImage->GetWidth(); i++)
        {
            Mutex::GetInstance().Lock();
            for (int j = 0; j < outputImage->GetHeight(); j++)
            {

                Image::Pixel pix = outputImage->GetPixel(i, j);

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

                outputImage->SetPixel(i, j, pix);
            }

            if (!Mutex::GetInstance().IsThreadRunning())
            {
                Mutex::GetInstance().SetOutputCode(Mutex::Stoped);
                Mutex::GetInstance().Unlock();
                return;
            }
            Mutex::GetInstance().Unlock();
        }
    }
    else if (method == Gradient)
    {
        float G = 0;
        float Gx = 0;
        float Gy = 0;
        float sum_JG = 0;
        float sum_G = 0;

        for (int i = 1; i < outputImage->GetWidth() - 1; i++)
        {
            Mutex::GetInstance().Lock();

            for (int j = 1; j < outputImage->GetHeight() - 1; j++)
            {
                Image::Pixel pix = outputImage->GetPixel(i, j);
                Gx = outputImage->GetPixel(i + 1, j).brightnes - outputImage->GetPixel(i - 1, j).brightnes;
                Gy = outputImage->GetPixel(i, j + 1).brightnes - outputImage->GetPixel(i, j - 1).brightnes;
                G = std::max(abs(Gx), abs(Gy));
                sum_G += G;
                sum_JG += pix.brightnes * G;
            }

            if (!Mutex::GetInstance().IsThreadRunning())
            {
                Mutex::GetInstance().SetOutputCode(Mutex::Stoped);
                Mutex::GetInstance().Unlock();
                return;
            }
            Mutex::GetInstance().Unlock();
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
        Mutex::GetInstance().Lock();
        outputImage->CopyNormalisedBrightnessHistogram(p);
        Mutex::GetInstance().Unlock();

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

            Mutex::GetInstance().Lock();
            if (!Mutex::GetInstance().IsThreadRunning())
            {
                Mutex::GetInstance().SetOutputCode(Mutex::Stoped);
                Mutex::GetInstance().Unlock();
                return;
            }
            Mutex::GetInstance().Unlock();
        }
    }

    if (params->method != None)
    {
        for (int i = 0; i < outputImage->GetWidth(); i++)
        {
            Mutex::GetInstance().Lock();

            for (int j = 0; j < outputImage->GetHeight(); j++)
            {
                Image::Pixel pix = outputImage->GetPixel(i, j);
                if (pix.brightnes < lowerBound)
                    outputImage->SetPixelBlack(i, j);
                else
                    outputImage->SetPixelWhite(i, j);
            }

            if (!Mutex::GetInstance().IsThreadRunning())
            {
                Mutex::GetInstance().SetOutputCode(Mutex::Stoped);
                Mutex::GetInstance().Unlock();
                return;
            }
            Mutex::GetInstance().Unlock();
        }
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().SetOutputCode(Mutex::Normal);
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Algorithms::LinearFilter(Image *outputImage, ParametersStruct *params)
{
    Image copy;
    int maskCopy[3][3];
    bool normalise = true;
    int maskSum = 0;
    int filter;
    Mutex::GetInstance().Lock();

    if (outputImage->NoSurface())
    {
        Mutex::GetInstance().SetOutputCode(Mutex::Error);
        Mutex::GetInstance().ThreadStopped();
        Mutex::GetInstance().Unlock();
        return;
    }
    copy.CopyOnlySurfaceAndSize(*outputImage);
    if (copy.NoSurface())
    {
        Mutex::GetInstance().SetOutputCode(Mutex::Error);
        Mutex::GetInstance().ThreadStopped();
        Mutex::GetInstance().Unlock();
        return;
    }

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            maskCopy[i][j] = params->linearMask[i][j];
            // all elements must be > 0
            if (maskCopy[i][j] <= 0)
                normalise = false;
            maskSum += maskCopy[i][j];
        }
    filter = params->linerFilterS;

    Mutex::GetInstance().Unlock();

    for (int row = 1; row < outputImage->GetWidth() - 1; row++)
    {
        Mutex::GetInstance().Lock();
        for (int col = 1; col < outputImage->GetHeight() - 1; col++)
        {
            auto pix = copy.GetPixel(row, col);
            int JR = 0;
            int JG = 0;
            int JB = 0;

            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    auto neighbourPix = copy.GetPixel(row + x - 1, col + y - 1);
                    JR += neighbourPix.r * maskCopy[x][y];
                    JG += neighbourPix.g * maskCopy[x][y];
                    JB += neighbourPix.b * maskCopy[x][y];
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
                if (filter == SobelHorizontal || filter == SobelVertical || filter == Laplasjan)
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
            outputImage->SetPixel(row, col, pix);
        }

        if (!Mutex::GetInstance().IsThreadRunning())
        {
            Mutex::GetInstance().SetOutputCode(Mutex::Stoped);
            Mutex::GetInstance().Unlock();
            return;
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().SetOutputCode(Mutex::Normal);
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}
