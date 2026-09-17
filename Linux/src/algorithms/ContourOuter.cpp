#include "ContourOuter.hpp"

ContourOuter::ContourOuter()
{
    algorithmName = "Kontur Zewnętrzny";
    ResetToDefaults();
}

void ContourOuter::AlgorithmFunction(Image *outputImage)
{
    // copy
    CopyToLocalVariable(outputImage);
    // element offsets
    CalculateOffsets();

    copyRead.CopyNoTexture(copy);

    copy.SetBlankSurfaceNoTexture(copyRead.GetWidth(), copyRead.GetHeight());

    for (int row = offsetTop; row < copyRead.GetHeight() - offsetBottom; row++)
    {
        for (int col = offsetLeft; col < copyRead.GetWidth() - offsetRight; col++)
        {
            // if part of object on picture
            if (copyRead.GetPixel(col, row).brightnes == BLACK)
            {
                for (int localRow = row - offsetTop; localRow <= row + offsetBottom; localRow++)
                {
                    for (int localCol = col - offsetLeft; localCol <= col + offsetRight; localCol++)
                    {
                        // if part of element 
                        if (elemntCopy[localRow - row + elementSize / 2][localCol - col + elementSize / 2])
                            copy.SetPixelBlack(localCol, localRow);
                    }
                }
            }
        }
    }

    for (int row = 0; row < copyRead.GetHeight(); row++)
    {
        for (int col = 0; col < copyRead.GetWidth(); col++)
        {
            // is outside object (white on copyRead, black on copy)
            if (copyRead.GetPixel(col, row).brightnes == WHITE && copy.GetPixel(col, row).brightnes == BLACK)
                copy.SetPixelBlack(col, row);
            else
                copy.SetPixelWhite(col, row);
        }
    }

    // copy back to output
    SaveToOutput(outputImage);
}