#ifndef EROSION_HPP
#define EROSION_HPP

#include "Algorithm.hpp"

class Erosion : public Algorithm
{
public:
    Erosion();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;

private:
    void DrawInputArray();

private:
    enum MatrixSize
    {
        S3x3 = 3,
        S5x5 = 5,
        S7x7 = 7
    };
#define ARRAY_INPUT_WIDTH 100
#define ARRAY_ITEM_WIDTH 30
#define ARRAY_FIELD_WIDTH 40

private:
    int erosionElementSize = S3x3;

    bool erosionElement3x3[3][3];
    bool erosionElement5x5[5][5];
    bool erosionElement7x7[7][7];
};

#endif
