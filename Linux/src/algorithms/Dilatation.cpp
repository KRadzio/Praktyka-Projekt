#include "Dilatation.hpp"
Dilatation::Dilatation()
{
    algorithmName = "Dylatacja";
    ResetToDefaults();
}


void Dilatation::AlgorithmFunction(Image *outputImage)
{
    // copy
    CopyToLocalVariable(outputImage);
    // element offsets
    CalculateOffsets();
    // the function itself
    DilatationFunc(outputImage);
    // copy back to output
    SaveToOutput(outputImage);
}