#ifndef NEGATIVE_HPP
#define NEGATIVE_HPP

#include "Algorithm.hpp"

class Negative : public Algorithm
{
public:

    Negative();
    
    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;
};

#endif
