#ifndef BRIGHTEN_HPP
#define BRIGHTEN_HPP

#include "Algorithm.hpp"

class Brighten : public Algorithm
{
public:
    Brighten();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;

private:
    int32_t value = 0;
};

#endif
