#ifndef MASKING_HPP
#define MASKING_HPP

#include "FileSelector.hpp"

#include "Algorithm.hpp"

#define CANCEL_BUTTON_W 120

class Masking : public Algorithm
{
public:
    Masking();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;

    void Save(std::ofstream &file) override;
    void Load(std::ifstream &file) override;

private:
    Image mask;

    bool maskingLoadMenuActive = false;
};

#endif
