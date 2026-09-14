#ifndef BINARIZATION_HPP
#define BINARIZATION_HPP

#include "Algorithm.hpp"

class Binarization : public Algorithm
{
public:
    Binarization();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;

private:
    enum BinarizationMethod
    {
        None,
        Gradient,
        Iteration
    };

private:
    int boundCount = 1;
    int lowerBound = 0;
    int upperBound = 0;
    int method = None;
};

#endif
