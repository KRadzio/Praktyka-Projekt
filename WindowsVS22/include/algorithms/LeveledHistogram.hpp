#ifndef LEVELEDHISTOGRAM_HPP
#define LEVELEDHISTOGRAM_HPP

#include "Algorithm.hpp"

class LeveledHistogram : public Algorithm
{
public:
    LeveledHistogram();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;

    void Save(std::ofstream &file) override;
    void Load(std::ifstream &file) override;
};

#endif
