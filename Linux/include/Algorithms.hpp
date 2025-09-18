#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <SDL2/SDL.h>

#include "Image.hpp"
#include "Mutex.hpp"

namespace Algorithms
{
    enum BinarizationMethod
    {
        None,
        Gradient,
        Histogram
    };

    struct ParametersStruct
    {
        int value = 0;
        float contrast = 1.0;
        float alfa = 1.0;
        // Binarization
        int boundCount = 1;
        int lowerBound = 0;
        int upperBound = 0;
        int method;
    };

    void CreateNegative(Image* outputImage);

    void BrightenImage(Image *outputImage, ParametersStruct *params);

    void Contrast(Image *outputImage, ParametersStruct *params);

    void Exponentiation(Image *outputImage, ParametersStruct *params);

    void LevelHistogram(Image *outputImage);

    void Binarization(Image *outputImage, ParametersStruct *params);

}

#endif
