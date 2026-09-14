#ifndef MIXING_HPP
#define MIXING_HPP

#include "FileSelector.hpp"

#include "Algorithm.hpp"

#define CANCEL_BUTTON_W 120

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

    bool mixingLoadMenuActive = false;

};

#endif
