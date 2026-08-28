#ifndef CONTOURINNER_HPP
#define CONTOURINNER_HPP

#include "MorphologicAlgorithm.hpp"

class ContourInner : public MorphologicAlgorithm
{
public:
    ContourInner();

    void AlgorithmFunction(Image *outputImage) override;
};

#endif
