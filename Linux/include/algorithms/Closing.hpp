#ifndef CLOSING_HPP
#define CLOSING_HPP

#include "Algorithm.hpp"

class Closing : public Algorithm
{
public:
    Closing();

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
    int elementSize = S3x3;

    bool element3x3[3][3];
    bool element5x5[5][5];
    bool element7x7[7][7];
};

#endif
