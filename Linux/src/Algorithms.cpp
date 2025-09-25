#include "App.hpp"

void Algorithms::CreateNegative(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    Image copy = *outputImage;
    Mutex::GetInstance().Unlock();

    for (int row = 0; row < copy.GetHeight(); row++)
    {
        for (int col = 0; col < copy.GetWidth(); col++)
        {
            auto pix = copy.GetPixel(col, row);
            pix.b = 255 - pix.b;
            pix.g = 255 - pix.g;
            pix.r = 255 - pix.r;
            copy.SetPixel(col, row, pix);
        }
        Mutex::GetInstance().Lock();
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            *outputImage = copy;
            copy.ClearImage();
            Mutex::GetInstance().Unlock();
            return;
        }
        if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
        {
            *outputImage = copy;
            Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    *outputImage = copy;
    copy.ClearImage();
    Mutex::GetInstance().Unlock();
}

void Algorithms::BrightenImage(Image *outputImage, ParametersStruct *params)
{

    Mutex::GetInstance().Lock();
    Image copy = *outputImage;
    auto value = params->value;
    Mutex::GetInstance().Unlock();

    for (int row = 0; row < copy.GetHeight(); row++)
    {
        for (int col = 0; col < copy.GetWidth(); col++)
        {
            auto pix = copy.GetPixel(col, row);

            if (pix.b + value > 255)
                pix.b = 255;
            else if (pix.b + value < 0)
                pix.b = 0;
            else
                pix.b += value;

            if (pix.g + value > 255)
                pix.g = 255;
            else if (pix.g + value < 0)
                pix.g = 0;
            else
                pix.g += value;

            if (pix.r + value > 255)
                pix.r = 255;
            else if (pix.r + value < 0)
                pix.r = 0;
            else
                pix.r += value;

            copy.SetPixel(col, row, pix);
        }

        Mutex::GetInstance().Lock();
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            *outputImage = copy;
            copy.ClearImage();
            Mutex::GetInstance().Unlock();
            return;
        }
        if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
        {
            *outputImage = copy;
            Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    *outputImage = copy;
    copy.ClearImage();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Contrast(Image *outputImage, ParametersStruct *params)
{

    Mutex::GetInstance().Lock();
    Image copy = *outputImage;
    auto contrast = params->contrast;
    Mutex::GetInstance().Unlock();

    for (int row = 0; row < copy.GetHeight(); row++)
    {
        for (int col = 0; col < copy.GetWidth(); col++)
        {
            auto pix = copy.GetPixel(col, row);

            if (pix.b * contrast > 255)
                pix.b = 255;
            else
                pix.b *= contrast;

            if (pix.g * contrast > 255)
                pix.g = 255;
            else
                pix.g *= contrast;

            if (pix.r * contrast > 255)
                pix.r = 255;
            else
                pix.r *= contrast;
            copy.SetPixel(col, row, pix);
        }

        Mutex::GetInstance().Lock();
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            *outputImage = copy;
            copy.ClearImage();
            Mutex::GetInstance().Unlock();
            return;
        }
        if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
        {
            *outputImage = copy;
            Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    *outputImage = copy;
    copy.ClearImage();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Exponentiation(Image *outputImage, ParametersStruct *params)
{
    Mutex::GetInstance().Lock();
    Image copy = *outputImage;
    int tab[256];
    for (int i = 0; i < 256; i++)
        tab[i] = 255.0 * pow((float)i / 255.0, params->alfa);

    Mutex::GetInstance().Unlock();
    for (int row = 0; row < copy.GetHeight(); row++)
    {
        for (int col = 0; col < copy.GetWidth(); col++)
        {
            auto pix = copy.GetPixel(col, row);

            pix.b = tab[pix.b];
            pix.g = tab[pix.g];
            pix.r = tab[pix.r];
            copy.SetPixel(col, row, pix);
        }
        Mutex::GetInstance().Lock();
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            *outputImage = copy;
            copy.ClearImage();
            Mutex::GetInstance().Unlock();
            return;
        }
        if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
        {
            *outputImage = copy;
            Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    *outputImage = copy;
    copy.ClearImage();
    Mutex::GetInstance().Unlock();
}

void Algorithms::LevelHistogram(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    auto copy = *outputImage;
    Mutex::GetInstance().Unlock();

    float *distR = copy.GetDistributorR();
    float *distG = copy.GetDistributorG();
    float *distB = copy.GetDistributorB();

    for (int row = 0; row < copy.GetHeight(); row++)
    {
        for (int col = 0; col < copy.GetWidth(); col++)
        {
            Image::Pixel pix = copy.GetPixel(col, row);
            pix.b = distB[pix.b];
            pix.g = distG[pix.g];
            pix.r = distR[pix.r];
            copy.SetPixel(col, row, pix);
        }
        Mutex::GetInstance().Lock();
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            *outputImage = copy;
            copy.ClearImage();
            Mutex::GetInstance().Unlock();
            return;
        }
        if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
        {
            *outputImage = copy;
            Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    *outputImage = copy;
    copy.ClearImage();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Binarization(Image *outputImage, ParametersStruct *params)
{
    int method;
    int lowerBound;
    int upperBound;
    int boundCount;
    Mutex::GetInstance().Lock();

    auto copy = *outputImage;
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
            Mutex::GetInstance().Lock();
            if (!Mutex::GetInstance().IsThreadRunning())
            {
                *outputImage = copy;
                copy.ClearImage();
                Mutex::GetInstance().Unlock();
                return;
            }
            if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
            {
                *outputImage = copy;
                Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
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

        for (int row = 0; row < copy.GetHeight(); row++)
        {
            for (int col = 0; col < copy.GetWidth(); col++)
            {
                Image::Pixel pix = copy.GetPixel(col, row);
                Gx = copy.GetPixel(col + 1, row).brightnes - copy.GetPixel(col - 1, row).brightnes;
                Gy = copy.GetPixel(col, row + 1).brightnes - copy.GetPixel(col, row - 1).brightnes;
                G = std::max(abs(Gx), abs(Gy));
                sum_G += G;
                sum_JG += pix.brightnes * G;
            }
            Mutex::GetInstance().Lock();
            if (!Mutex::GetInstance().IsThreadRunning())
            {
                *outputImage = copy;
                copy.ClearImage();
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
        copy.CopyNormalisedBrightnessHistogram(p);
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
                *outputImage = copy;
                copy.ClearImage();
                Mutex::GetInstance().Unlock();
                return;
            }
            Mutex::GetInstance().Unlock();
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

            Mutex::GetInstance().Lock();
            if (!Mutex::GetInstance().IsThreadRunning())
            {
                *outputImage = copy;
                copy.ClearImage();
                Mutex::GetInstance().Unlock();
                return;
            }
            if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
            {
                *outputImage = copy;
                Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
            }
            Mutex::GetInstance().Unlock();
        }
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    *outputImage = copy;
    copy.ClearImage();
    Mutex::GetInstance().Unlock();
}

void Algorithms::LinearFilter(Image *outputImage, ParametersStruct *params)
{
    // only for pixels, the result is saved to fullCopy
    Image copy;
    int offset = 0;
    int maskCopy[7][7];
    int maskSize = 0;
    bool normalise = true;
    int maskSum = 0;
    int filter;
    Mutex::GetInstance().Lock();

    // to set pixels
    auto fullCopy = *outputImage;

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

    for (int row = offset; row < copy.GetHeight() - offset; row++)
    {
        for (int col = offset; col < copy.GetWidth() - offset; col++)
        {
            auto pix = copy.GetPixel(col, row);
            int JR = 0;
            int JG = 0;
            int JB = 0;

            for (int y = 0; y < maskSize; y++)
            {
                for (int x = 0; x < maskSize; x++)
                {
                    auto neighbourPix = copy.GetPixel(col + x - offset, row + y - offset);
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
            fullCopy.SetPixel(col, row, pix);
        }
        Mutex::GetInstance().Lock();
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            *outputImage = fullCopy;
            fullCopy.ClearImage();
            copy.ClearImage();
            Mutex::GetInstance().Unlock();
            return;
        }
        if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
        {
            *outputImage = fullCopy;
            Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    *outputImage = fullCopy;
    fullCopy.ClearImage();
    copy.ClearImage();
    Mutex::GetInstance().Unlock();
}

void Algorithms::MedianFilter(Image *outputImage, ParametersStruct *params)
{
    // only for pixels
    Image copy;
    int offset = 0;
    int maskCopy[7][7];
    int maskSize = 0;
    Mutex::GetInstance().Lock();

    // results go here
    auto fullCopy = *outputImage;

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

    for (int row = offset; row < copy.GetHeight() - offset; row++)
    {
        for (int col = offset; col < copy.GetWidth() - offset; col++)
        {
            auto pix = copy.GetPixel(col, row);
            std::vector<int> JR;
            std::vector<int> JG;
            std::vector<int> JB;

            for (int y = 0; y < maskSize; y++)
            {
                for (int x = 0; x < maskSize; x++)
                {
                    auto neighbourPix = copy.GetPixel(col + x - offset, row + y - offset);
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

            fullCopy.SetPixel(col, row, pix);
        }

        Mutex::GetInstance().Lock();
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            *outputImage = fullCopy;
            fullCopy.ClearImage();
            copy.ClearImage();
            Mutex::GetInstance().Unlock();
            return;
        }
        if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
        {
            *outputImage = fullCopy;
            Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    *outputImage = fullCopy;
    fullCopy.ClearImage();
    copy.ClearImage();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Erosion(Image *outputImage, ParametersStruct *params)
{
    Image copy;
    int offsetLeft = 0;
    int offsetRight = 0;
    int offsetTop = 0;
    int offsetBottom = 0;
    int elemntCopy[7][7];
    int elementSize = 0;
    Mutex::GetInstance().Lock();

    // results go here
    auto fullCopy = *outputImage;

    copy.CopyOnlySurfaceAndSize(*outputImage);
    if (copy.NoSurface())
    {
        Mutex::GetInstance().ThreadStopped();
        Mutex::GetInstance().Unlock();
        return;
    }

    elementSize = params->erosionElementSize;

    for (int i = 0; i < elementSize; i++)
        for (int j = 0; j < elementSize; j++)
        {
            if (elementSize == S3x3)
                elemntCopy[i][j] = params->erosionElement3x3[i][j];
            else if (elementSize == S5x5)
                elemntCopy[i][j] = params->erosionElement5x5[i][j];
            else
                elemntCopy[i][j] = params->erosionElement7x7[i][j];
        }

    Mutex::GetInstance().Unlock();

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

    for (int row = offsetTop; row < copy.GetHeight() - offsetBottom; row++)
    {
        for (int col = offsetLeft; col < copy.GetWidth() - offsetRight; col++)
        {
            auto pix = copy.GetPixel(col, row);
            if (pix.brightnes == BLACK)
            {
                bool elemntOutside = false;
                for (int y = row - offsetTop; y < row + offsetBottom + 1; y++)
                {
                    for (int x = col - offsetLeft; x < col + offsetRight + 1; x++)
                    {
                        auto neighbourPix = copy.GetPixel(x, y);
                        if (elemntCopy[y - row + elementSize / 2][x - col + elementSize / 2])
                        {
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
                    fullCopy.SetPixelWhite(col, row);
            }
        }

        Mutex::GetInstance().Lock();
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            *outputImage = fullCopy;
            fullCopy.ClearImage();
            copy.ClearImage();
            Mutex::GetInstance().Unlock();
            return;
        }
        if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
        {
            *outputImage = fullCopy;
            Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    *outputImage = fullCopy;
    fullCopy.ClearImage();
    copy.ClearImage();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Dilatation(Image *outputImage, ParametersStruct *params)
{
    Image copy;
    int offsetLeft = 0;
    int offsetRight = 0;
    int offsetTop = 0;
    int offsetBottom = 0;
    int elemntCopy[7][7];
    int elementSize = 0;
    Mutex::GetInstance().Lock();

    // results go here
    auto fullCopy = *outputImage;

    copy.CopyOnlySurfaceAndSize(*outputImage);
    if (copy.NoSurface())
    {
        Mutex::GetInstance().ThreadStopped();
        Mutex::GetInstance().Unlock();
        return;
    }

    elementSize = params->dilatationElementSize;

    for (int i = 0; i < elementSize; i++)
        for (int j = 0; j < elementSize; j++)
        {
            if (elementSize == S3x3)
                elemntCopy[i][j] = params->dilatationElement3x3[i][j];
            else if (elementSize == S5x5)
                elemntCopy[i][j] = params->dilatationElement5x5[i][j];
            else
                elemntCopy[i][j] = params->dilatationElement7x7[i][j];
        }

    Mutex::GetInstance().Unlock();

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

    for (int row = offsetTop; row < copy.GetHeight() - offsetBottom; row++)
    {
        for (int col = offsetLeft; col < copy.GetWidth() - offsetRight; col++)
        {
            auto pix = copy.GetPixel(col, row);
            if (pix.brightnes == BLACK)
            {
                bool elemntOutside = false;
                for (int y = row - offsetTop; y < row + offsetBottom + 1; y++)
                {
                    for (int x = col - offsetLeft; x < col + offsetRight + 1; x++)
                    {
                        auto neighbourPix = copy.GetPixel(x, y);
                        if (elemntCopy[y - row + elementSize / 2][x - col + elementSize / 2])
                        {
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
                {
                    for (int y = row - offsetTop; y < row + offsetBottom + 1; y++)
                    {
                        for (int x = col - offsetLeft; x < col + offsetRight + 1; x++)
                        {
                            if (elemntCopy[y - row + elementSize / 2][x - col + elementSize / 2])
                                fullCopy.SetPixelBlack(x, y);
                        }
                    }
                }
            }
        }

        Mutex::GetInstance().Lock();
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            *outputImage = fullCopy;
            fullCopy.ClearImage();
            copy.ClearImage();
            Mutex::GetInstance().Unlock();
            return;
        }
        if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
        {
            *outputImage = fullCopy;
            Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
        }
        Mutex::GetInstance().Unlock();
    }

    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    *outputImage = fullCopy;
    fullCopy.ClearImage();
    copy.ClearImage();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Skeletonization(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    auto o = *outputImage;
    o.ClearImage();
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Hought(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    auto o = *outputImage;
    o.ClearImage();
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}
