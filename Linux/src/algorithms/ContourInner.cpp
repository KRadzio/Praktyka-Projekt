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

    // TUTAJ UZUPEŁNIĆ

    SaveToOutput(outputImage);
}