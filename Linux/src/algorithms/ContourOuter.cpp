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

    // TUTAJ UZUPEŁNIĆ

    // copy back to output
    SaveToOutput(outputImage);
}