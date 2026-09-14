#ifndef HOUGHT_HPP
#define HOUGHT_HPP


#include "Algorithm.hpp"

class Hought : public Algorithm
{
private:
    #define THETA_NUM 180

public:
    Hought();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;

private:
    void CopyToLocalVariable(Image *outputImage) override;
    bool Canceled(Image *outputImage) override;
    void ManualRefresh(Image *outputImage) override;
    void SaveToOutput(Image *outputImage) override;

    void CopyAcumulator(Image *outputImage);
    void ClearAcumulator();

private:
    int32_t maxIndexRo = 0;
    int32_t maxIndexTheta = 0;
    int32_t maxHoughtVal = INT32_MIN;
    std::vector<std::vector<int32_t>> acumulator;
    int32_t roMax = 0;

};

#endif
