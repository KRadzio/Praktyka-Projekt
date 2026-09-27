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

    // NOTHING TO DO
    void Save(std::ofstream &file) override;
    // NOTHING TO DO
    void Load(std::ifstream &file) override;
};

#endif
