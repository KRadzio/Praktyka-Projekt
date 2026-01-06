#include "App.hpp"
// IMPORTNANT
// if adding and new fuctions
// (or implementing them)
// remember to:
// 1) make a local copy
//     Mutex::GetInstance().Lock();
//     Image copy = *outputImage;
//     Mutex::GetInstance().Unlock();
// 2.1) [standard] make sure that it can be canleled and auto refreshed
//         Mutex::GetInstance().Lock();
//         // if canceled
//         if (!Mutex::GetInstance().IsThreadRunning())
//         {
//             *outputImage = copy;
//             copy.ClearImage();
//             Mutex::GetInstance().Unlock();
//             return;
//         }
//         // if auto refresh enabled
//         if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
//         {
//             *outputImage = copy;
//             Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
//         }
//         Mutex::GetInstance().Unlock();
// 2.2) [special] refreshed maualy (remember to either set auto refresh off, or prevent it from being used)
//         Mutex::GetInstance().Lock();
//         if (!Mutex::GetInstance().IsThreadRunning())
//         {
//             *outputImage = fullCopy;
//             fullCopy.ClearImage();
//             for (int i = 0; i < h; i++)
//                 pixelsStatus[i].clear();
//             pixelsStatus.clear();
//             Mutex::GetInstance().Unlock();
//             return;
//         }
//         *outputImage = fullCopy;
//         Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
//         Mutex::GetInstance().Unlock();
// 3) copy back to shared resource
//     // copy back to output
//     Mutex::GetInstance().Lock();
//     Mutex::GetInstance().ThreadStopped();
//     *outputImage = copy;
//     copy.ClearImage();
//     Mutex::GetInstance().Unlock();


void Algorithms::Erosion(Image *outputImage, ParametersStruct *params)
{
    // local copy
    // only for pixels
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

    // copy element
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
                    fullCopy.SetPixelWhite(col, row);
            }
        }

        Mutex::GetInstance().Lock();
        // if canceled
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            *outputImage = fullCopy;
            fullCopy.ClearImage();
            copy.ClearImage();
            Mutex::GetInstance().Unlock();
            return;
        }
        // if auto refresh enabled
        if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
        {
            *outputImage = fullCopy;
            Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
        }
        Mutex::GetInstance().Unlock();
    }
    // copy back to output
    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    *outputImage = fullCopy;
    fullCopy.ClearImage();
    copy.ClearImage();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Dilatation(Image *outputImage, ParametersStruct *params)
{
    // local copy
    // only for pixels
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

    // copy element
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
                                fullCopy.SetPixelBlack(x, y);
                        }
                    }
                }
            }
        }

        Mutex::GetInstance().Lock();
        // if canceled
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            *outputImage = fullCopy;
            fullCopy.ClearImage();
            copy.ClearImage();
            Mutex::GetInstance().Unlock();
            return;
        }
        // if auto refresh enabled
        if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
        {
            *outputImage = fullCopy;
            Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
        }
        Mutex::GetInstance().Unlock();
    }
    // copy back to output
    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    *outputImage = fullCopy;
    fullCopy.ClearImage();
    copy.ClearImage();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Skeletonization(Image *outputImage)
{
    // neighbours
    int S[8];
    bool remain = true;
    bool skel = false;
    // local copy
    Mutex::GetInstance().Lock();
    auto fullCopy = *outputImage;

    if (fullCopy.NoSurface())
    {
        Mutex::GetInstance().ThreadStopped();
        Mutex::GetInstance().Unlock();
        return;
    }

    Mutex::GetInstance().Unlock();

    while (remain)
    {
        remain = false;
        for (int i = 0; i < 8; i += 2)
        {
            for (int row = 1; row < fullCopy.GetHeight() - 1; row++)
            {
                for (int col = 1; col < fullCopy.GetWidth() - 1; col++)
                {
                    auto pix = fullCopy.GetPixel(col, row);
                    // set neighbours
                    S[0] = fullCopy.GetPixel(col + 1, row).brightnes;
                    S[1] = fullCopy.GetPixel(col + 1, row - 1).brightnes;
                    S[2] = fullCopy.GetPixel(col, row - 1).brightnes;
                    S[3] = fullCopy.GetPixel(col - 1, row - 1).brightnes;
                    S[4] = fullCopy.GetPixel(col - 1, row).brightnes;
                    S[5] = fullCopy.GetPixel(col - 1, row + 1).brightnes;
                    S[6] = fullCopy.GetPixel(col, row + 1).brightnes;
                    S[7] = fullCopy.GetPixel(col + 1, row + 1).brightnes;

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
                            fullCopy.SetPixel(col,row,pix);
                        }
                        else
                        {
                           Image::Pixel pix;
                            pix.b = REMOVE;
                            pix.g = REMOVE;
                            pix.r = REMOVE;
                            fullCopy.SetPixel(col,row,pix);
                            remain = true;
                        }
                    }
                }
            }
            for (int row = 0; row < fullCopy.GetHeight(); row++)
                for (int col = 0; col < fullCopy.GetWidth(); col++)
                    if (fullCopy.GetPixel(col,row).brightnes == REMOVE)
                        fullCopy.SetPixelWhite(col, row);
        }
        Mutex::GetInstance().Lock();
        // if canceled
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            *outputImage = fullCopy;
            fullCopy.ClearImage();
            Mutex::GetInstance().Unlock();
            return;
        }
        // manualy refreshed
        *outputImage = fullCopy;
        Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
        Mutex::GetInstance().Unlock();
    }

    // copy back to output
    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    *outputImage = fullCopy;
    fullCopy.ClearImage();
    Mutex::GetInstance().Unlock();
}

void Algorithms::Hought(Image *outputImage, ParametersStruct *params)
{
    // to store results
    std::vector<std::vector<int>> acumulator;
    // max val
    int max = INT_MIN;
    // max coords
    int maxThetaIndex = 0;
    int maxRoIndex = 0;

    // local copy
    Mutex::GetInstance().Lock();
    auto copy = *outputImage;
    int roMax = sqrt(copy.GetHeight() * copy.GetHeight() + copy.GetWidth() * copy.GetWidth());
    // set output to a black surface
    outputImage->SetBlankSurface(THETA_NUM, 2 * roMax);
    Mutex::GetInstance().Unlock();

    acumulator = std::vector<std::vector<int>>(2 * roMax);
    for (int i = 0; i < 2 * roMax; i++)
        acumulator[i] = std::vector<int>(THETA_NUM);

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
                    if (acumulator[currRo + roMax][t] > max)
                    {
                        max = acumulator[currRo + roMax][t];
                        maxThetaIndex = t;
                        maxRoIndex = currRo;
                    }
                }
            }
        }

        Mutex::GetInstance().Lock();
        // if canceled
        if (!Mutex::GetInstance().IsThreadRunning())
        {
            for (int i = 0; i < 2 * roMax; i++)
                for (int j = 0; j < THETA_NUM; j++)
                {
                    float tmp = acumulator[i][j];
                    tmp /= max;
                    tmp *= 255;
                    Image::Pixel pix;
                    pix.g = 255 - tmp;
                    pix.b = 255 - tmp;
                    pix.r = 255 - tmp;
                    outputImage->SetPixel(j, i, pix);
                }
            params->maxIndexRo = maxRoIndex;
            params->maxIndexTheta = maxThetaIndex;
            params->maxHoughtVal = max;
            copy.ClearImage();
            for (int i = 0; i < 2 * roMax; i++)
                acumulator[i].clear();
            acumulator.clear();
            Mutex::GetInstance().Unlock();
            return;
        }
        // refresh for every line
        for (int i = 0; i < 2 * roMax; i++)
            for (int j = 0; j < THETA_NUM; j++)
            {
                float tmp = acumulator[i][j];
                tmp /= max;
                tmp *= 255;
                Image::Pixel pix;
                pix.g = 255 - tmp;
                pix.b = 255 - tmp;
                pix.r = 255 - tmp;
                outputImage->SetPixel(j, i, pix);
            }
        params->maxIndexRo = maxRoIndex;
        params->maxIndexTheta = maxThetaIndex;
        params->maxHoughtVal = max;
        Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
        Mutex::GetInstance().Unlock();
    }

    // refresh after the end
    // copy back to output
    Mutex::GetInstance().Lock();

    for (int i = 0; i < 2 * roMax; i++)
        for (int j = 0; j < THETA_NUM; j++)
        {
            float tmp = acumulator[i][j];
            tmp /= max;
            tmp *= 255;
            Image::Pixel pix;
            pix.g = 255 - tmp;
            pix.b = 255 - tmp;
            pix.r = 255 - tmp;
            outputImage->SetPixel(j, i, pix);
        }

    for (int i = 0; i < 2 * roMax; i++)
        acumulator[i].clear();
    acumulator.clear();

    params->maxIndexRo = maxRoIndex;
    params->maxIndexTheta = maxThetaIndex;
    params->maxHoughtVal = max;
    copy.ClearImage();
    Mutex::GetInstance().ThreadStopped();
    Mutex::GetInstance().Unlock();
}
