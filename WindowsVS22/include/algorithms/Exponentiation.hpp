#ifndef EXPONENTIATION_HPP
#define EXPONENTIATION_HPP

#include "Algorithm.hpp"

class Exponentiation : public Algorithm
{
public:
    Exponentiation();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;

    void Save(std::ofstream &file) override;
    void Load(std::ifstream &file) override;

private:
    float_t alfa = 1.0;
};

#endif
