#ifndef MORPHOLOGICALGORITHM_HPP
#define MORPHOLOGICALGORITHM_HPP

#include "Algorithm.hpp"

class MorphologicAlgorithm : public Algorithm
{
protected:
#define EMPTY_ELEMENT_3x3 {{false, false, false}, {false, false, false}, {false, false, false}}
#define EMPTY_ELEMENT_5x5 {{false, false, false, false, false},{false, false, false, false, false}, {false, false, false, false, false}, {false, false, false, false, false},{false, false, false, false, false}}
#define EMPTY_ELEMENT_7x7 {{false, false, false, false, false, false, false},{false, false, false, false, false, false, false}, {false, false, false, false, false, false, false}, {false, false, false, false, false, false, false}, {false, false, false, false, false, false, false}, {false, false, false, false, false, false, false},{false, false, false, false, false, false, false}}


public:
    MorphologicAlgorithm();

    // called when params menu opend
    void ParamsMenu() override;
    // called when params needs to be reset
    void ResetToDefaults() override;

protected:
    void DrawInputArray();
    void CalculateOffsets();
    void ErosionFunc(Image *outputImage);
    void DilatationFunc(Image *outputImage);

protected:
    enum MatrixSize
    {
        S3x3 = 3,
        S5x5 = 5,
        S7x7 = 7
    };
#define ARRAY_INPUT_WIDTH 100
#define ARRAY_ITEM_WIDTH 30
#define ARRAY_FIELD_WIDTH 40

protected:
    int elementSize = S3x3;

    bool element3x3[3][3] = EMPTY_ELEMENT_3x3;
    bool element5x5[5][5] = EMPTY_ELEMENT_5x5;
    bool element7x7[7][7] = EMPTY_ELEMENT_7x7;

    Image copyRead; // contex operation
    int32_t offsetLeft = 0;
    int32_t offsetRight = 0;
    int32_t offsetTop = 0;
    int32_t offsetBottom = 0;
    int32_t elemntCopy[7][7]; // copy the current mask (there are 3 sizes this way its easier)

};

#endif
