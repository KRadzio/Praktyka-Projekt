#ifndef DILATATION_HPP
#define DILATATION_HPP

#include "Algorithm.hpp"

class Dilatation : public Algorithm
{
public:
    Dilatation();

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
    int dilatationElementSize = S3x3;

    bool dilatationElement3x3[3][3];
    bool dilatationElement5x5[5][5];
    bool dilatationElement7x7[7][7];
};

#endif
