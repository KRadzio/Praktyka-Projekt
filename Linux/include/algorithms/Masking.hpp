#ifndef MASKING_HPP
#define MASKING_HPP

#include "Algorithm.hpp"

class Masking : public Algorithm
{
public:
    Masking();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;

private:
    Image mask;
};

#endif
