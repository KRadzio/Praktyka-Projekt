#include "Erosion.hpp"
Erosion::Erosion()
{
    algorithmName = "Erozja";
    ResetToDefaults();
}

void Erosion::AlgorithmFunction(Image *outputImage)
{
    // copy
    CopyToLocalVariable(outputImage);
    // element offsets
    CalculateOffsets();
    // the function itself
    ErosionFunc(outputImage);
    // copy back to output
    SaveToOutput(outputImage);
}
