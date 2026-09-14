#ifndef DILATATION_HPP
#define DILATATION_HPP

#include "MorphologicAlgorithm.hpp"

class Dilatation : public MorphologicAlgorithm
{
public:
    Dilatation();

    void AlgorithmFunction(Image *outputImage) override;

};

#endif
