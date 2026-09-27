#ifndef SKELETONIZATION_HPP
#define SKELETONIZATION_HPP

#include "Algorithm.hpp"

class Skeletonization : public Algorithm
{
private:
// Skeletonization defines
#define NONE 0
#define SKELETON 2
#define REMOVE 3

public:
    Skeletonization();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;

    void Save(std::ofstream &file) override;
    void Load(std::ifstream &file) override;
};

#endif
