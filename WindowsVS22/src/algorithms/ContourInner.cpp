#include "ContourInner.hpp"

ContourInner::ContourInner()
{
    algorithmName = "Kontur Wewnętrzny";
    ResetToDefaults();
}

void ContourInner::AlgorithmFunction(Image *outputImage)
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
            bool elementFits = true; 

            // check if every element part fits inside if not stop checking
            for (int localRow = row - offsetTop, elementRow = 0; localRow <= row + offsetBottom; localRow++, elementRow++)
            {
                for (int localCol = col - offsetLeft, elementCol = 0; localCol <= col + offsetRight; localCol++, elementCol++)
                    if (elemntCopy[elementRow][elementCol] && copyRead.GetPixel(localCol, localRow).brightnes == WHITE)
                        {
                            elementFits = false;
                            break;
                        }
                if(!elementFits)
                        break;
            }

            // contour (does not fit and pixel is black meaning an object)
            if (!elementFits && copyRead.GetPixel(col, row).brightnes == BLACK)
                copy.SetPixelBlack(col, row);
        }
    }

    // copy back to output
    SaveToOutput(outputImage);
}