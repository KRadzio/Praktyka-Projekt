#ifndef CONTRAST_HPP
#define CONTRAST_HPP

#include "Algorithm.hpp"

class Contrast : public Algorithm
{
    public:
    Contrast();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;

    void Save(std::ofstream& file) override;
    void Load(std::ifstream& file) override;

private:
    float_t contrast = 1.0;
};


#endif
