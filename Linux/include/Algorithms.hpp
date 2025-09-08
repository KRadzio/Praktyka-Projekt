#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <SDL2/SDL.h>

#include "Image.hpp"

namespace Algorithms
{
    int CreateNegative(Image &inputImage, Image &outputImage, SDL_Renderer *renderer);

    int BrightenImage(Image &inputImage, Image &outputImage, SDL_Renderer *renderer, int value);

    int Contrast(Image &inputImage, Image &outputImage, SDL_Renderer *renderer, float value);

}

#endif
