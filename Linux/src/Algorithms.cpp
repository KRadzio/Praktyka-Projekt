#include "App.hpp"

void Algorithms::CreateNegative(Image *outputImage)
{
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
            Mutex::GetInstance().Unlock();
            return;
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Algorithms::BrightenImage(Image *outputImage, ParametersStruct *params)
{

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
            Mutex::GetInstance().Unlock();
            return;
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Contrast(Image *outputImage, ParametersStruct *params)
{

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
            Mutex::GetInstance().Unlock();
            return;
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Exponentiation(Image *outputImage, ParametersStruct *params)
{
    Mutex::GetInstance().Lock();
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
            Mutex::GetInstance().Unlock();
            return;
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Algorithms::LevelHistogram(Image *outputImage)
{
    Mutex::GetInstance().Lock();

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
            Mutex::GetInstance().Unlock();
            return;
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
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
                Mutex::GetInstance().Unlock();
                return;
            }
            Mutex::GetInstance().Unlock();
        }
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Algorithms::LinearFilter(Image *outputImage, ParametersStruct *params)
{
    Image copy;
    int offset = 0;
    int maskCopy[7][7];
    int maskSize = 0;
    bool normalise = true;
    int maskSum = 0;
    int filter;
    Mutex::GetInstance().Lock();

    copy.CopyOnlySurfaceAndSize(*outputImage);
    if (copy.NoSurface())
    {
        Mutex::GetInstance().ThreadStopped();
        Mutex::GetInstance().Unlock();
        return;
    }

    maskSize = params->linearFilterSize;
    offset = params->linearFilterSize / 2;

    for (int i = 0; i < maskSize; i++)
        for (int j = 0; j < maskSize; j++)
        {
            if (maskSize == S3x3)
                maskCopy[i][j] = params->linearMask3x3[i][j];
            else if (maskSize == S5x5)
                maskCopy[i][j] = params->linearMask5x5[i][j];
            else
                maskCopy[i][j] = params->linearMask7x7[i][j];

            // all elements must be > 0
            if (maskCopy[i][j] <= 0)
                normalise = false;
            maskSum += maskCopy[i][j];
        }
    filter = params->linerFilterS;

    Mutex::GetInstance().Unlock();

    for (int row = offset; row < outputImage->GetWidth() - offset; row++)
    {
        Mutex::GetInstance().Lock();
        for (int col = offset; col < outputImage->GetHeight() - offset; col++)
        {
            auto pix = copy.GetPixel(row, col);
            int JR = 0;
            int JG = 0;
            int JB = 0;

            for (int x = 0; x < maskSize; x++)
            {
                for (int y = 0; y < maskSize; y++)
                {
                    auto neighbourPix = copy.GetPixel(row + x - offset, col + y - offset);
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
            Mutex::GetInstance().Unlock();
            return;
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Algorithms::MedianFilter(Image *outputImage, ParametersStruct *params)
{
    Image copy;
    int offset = 0;
    int maskCopy[7][7];
    int maskSize = 0;
    Mutex::GetInstance().Lock();

    copy.CopyOnlySurfaceAndSize(*outputImage);
    if (copy.NoSurface())
    {
        Mutex::GetInstance().ThreadStopped();
        Mutex::GetInstance().Unlock();
        return;
    }

    maskSize = params->linearFilterSize;
    offset = params->linearFilterSize / 2;

    for (int i = 0; i < maskSize; i++)
        for (int j = 0; j < maskSize; j++)
        {
            if (maskSize == S3x3)
                maskCopy[i][j] = params->medianMask3x3[i][j];
            else if (maskSize == S5x5)
                maskCopy[i][j] = params->medianMask5x5[i][j];
            else
                maskCopy[i][j] = params->medianMask7x7[i][j];
        }

    Mutex::GetInstance().Unlock();

    for (int row = offset; row < outputImage->GetWidth() - offset; row++)
    {
        Mutex::GetInstance().Lock();
        for (int col = offset; col < outputImage->GetHeight() - offset; col++)
        {
            auto pix = copy.GetPixel(row, col);
            std::vector<int> JR;
            std::vector<int> JG;
            std::vector<int> JB;

            for (int x = 0; x < maskSize; x++)
            {
                for (int y = 0; y < maskSize; y++)
                {
                    auto neighbourPix = copy.GetPixel(row + x - offset, col + y - offset);
                    if (maskCopy[x][y])
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

            outputImage->SetPixel(row, col, pix);
        }

        if (!Mutex::GetInstance().IsThreadRunning())
        {
            Mutex::GetInstance().Unlock();
            return;
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Erosion(Image *outputImage, ParametersStruct *params)
{
}

void Algorithms::Dilatation(Image *outputImage, ParametersStruct *params)
{
}

void Algorithms::Skeletonization(Image *outputImage, ParametersStruct *params)
{
}

void Algorithms::Hought(Image *outputImage, ParametersStruct *params)
{
}
