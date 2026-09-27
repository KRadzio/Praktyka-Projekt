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

    // TUTAJ UZUPEŁNIĆ

    SaveToOutput(outputImage);
}