#include "Opening.hpp"

Opening::Opening()
{
    algorithmName = "Otwarcie";
    ResetToDefaults();
}

void Opening::AlgorithmFunction(Image *outputImage)
{

    // copy
    CopyToLocalVariable(outputImage);
    // element offsets
    CalculateOffsets();
     // erosion
    ErosionFunc(outputImage);
    // dilatation 
    DilatationFunc(outputImage);
    // copy back to output
    SaveToOutput(outputImage);
}
