#ifndef LOGARITHM_HPP
#define LOGARITHM_HPP

#include "Algorithm.hpp"

class Logarithm : public Algorithm
{
public:
    Logarithm();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;

    void Save(std::ofstream &file) override;
    void Load(std::ifstream &file) override;
};

#endif
