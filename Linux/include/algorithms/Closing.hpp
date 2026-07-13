#ifndef CLOSING_HPP
#define CLOSING_HPP

#include "MorphologicAlgorithm.hpp"

class Closing : public MorphologicAlgorithm
{
public:
    Closing();

    void AlgorithmFunction(Image *outputImage) override;
};

#endif
