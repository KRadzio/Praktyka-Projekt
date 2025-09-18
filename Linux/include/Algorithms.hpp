#ifndef ALGORITHMS
#define ALGORITHMS

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

    enum LinearFilters
    {
        Average,
        Gauss,
        SobelHorizontal,
        SobelVertical,
        Laplasjan,
        Sharpening,
        Custom
    };

    struct ParametersStruct
    {
        // Brighten / Darken
        int value = 0;
        // Contrast
        float contrast = 1.0;
        // Exp
        float alfa = 1.0;
        // Binarization
        int boundCount = 1;
        int lowerBound = 0;
        int upperBound = 0;
        int method;
        // Linear Filters
        int linerFilterS = Average;
        int linearMask[3][3] = {{1,1,1}, {1,1,1}, {1,1,1}};
        // Median Filters
        int** medianMask = nullptr; // this one can change size
    };

    void CreateNegative(Image* outputImage);

    void BrightenImage(Image *outputImage, ParametersStruct *params);

    void Contrast(Image *outputImage, ParametersStruct *params);

    void Exponentiation(Image *outputImage, ParametersStruct *params);

    void LevelHistogram(Image *outputImage);

    void Binarization(Image *outputImage, ParametersStruct *params);

    void LinearFilter(Image *outputImage, ParametersStruct *params);

}

#endif
