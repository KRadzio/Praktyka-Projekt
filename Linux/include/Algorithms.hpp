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

    int CreateNegative(Image &inputImage, Image &outputImage);

    int BrightenImage(Image &inputImage, Image &outputImage, ParametersStruct *params);

    int Contrast(Image &inputImage, Image &outputImage, ParametersStruct *params);

    int Exponentiation(Image &inputImage, Image &outputImage, ParametersStruct *params);

    int LevelHistogram(Image &inputImage, Image &outputImage);

}

#endif
