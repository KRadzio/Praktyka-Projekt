#ifndef OPENING_HPP
#define OPENING_HPP

#include "MorphologicAlgorithm.hpp"

class Opening : public MorphologicAlgorithm
{
public:
    Opening();

    void AlgorithmFunction(Image *outputImage) override;
};

#endif
