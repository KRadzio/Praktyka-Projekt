#ifndef CONTOUROUTER_HPP
#define CONTOUROUTER_HPP

#include "MorphologicAlgorithm.hpp"

class ContourOuter : public MorphologicAlgorithm
{
public:
    ContourOuter();

    void AlgorithmFunction(Image *outputImage) override;
};

#endif
