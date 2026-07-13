#include "Closing.hpp"

Closing::Closing()
{
    algorithmName = "Zamknięcie";
    ResetToDefaults();
}

void Closing::AlgorithmFunction(Image *outputImage)
{
    // copy
    CopyToLocalVariable(outputImage);
    // element offsets
    CalculateOffsets();
    // dilatation 
    DilatationFunc(outputImage);
    // erosion
    ErosionFunc(outputImage);
    // copy back to output
    SaveToOutput(outputImage);
}