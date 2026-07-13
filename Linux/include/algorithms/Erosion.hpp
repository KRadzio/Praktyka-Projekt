#ifndef EROSION_HPP
#define EROSION_HPP

#include "MorphologicAlgorithm.hpp"

class Erosion : public MorphologicAlgorithm
{
public:
    Erosion();

    void AlgorithmFunction(Image *outputImage) override;
};

#endif
