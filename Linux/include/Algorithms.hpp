#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <SDL2/SDL.h>

#include "Image.hpp"
#include "Mutex.hpp"

namespace Algorithms
{
    struct ParametersStruct
    {
        int value = 0;
        float contrast = 1.0;
        float alfa = 1.0;
    };

    void CreateNegative(Image* outputImage);

    void BrightenImage(Image *outputImage, ParametersStruct *params);

    void Contrast(Image *outputImage, ParametersStruct *params);

    void Exponentiation(Image *outputImage, ParametersStruct *params);

    void LevelHistogram(Image *outputImage);

}

#endif
