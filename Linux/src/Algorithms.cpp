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