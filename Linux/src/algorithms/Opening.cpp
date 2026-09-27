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
    
    // TUTAJ UZUPEŁNIĆ

    SaveToOutput(outputImage);
}
