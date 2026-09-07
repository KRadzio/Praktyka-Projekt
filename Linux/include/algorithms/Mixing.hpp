#ifndef MIXING_HPP
#define MIXING_HPP

#include "Algorithm.hpp"

class Mixing : public Algorithm
{
public:
    Mixing();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;

private:
    float mixRatio = 0.5f;
    Image image2;

};

#endif
