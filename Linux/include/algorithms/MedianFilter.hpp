#ifndef MEDIANFILTER_HPP
#define MEDIANFILTER_HPP

#include <algorithm>

#include "Algorithm.hpp"

class MedianFilter : public Algorithm
{
private:
#define MEDIAN_3x3 {{true, true, true}, {true, true, true}, {true, true, true}}
#define MEDIAN_5x5 {{true, true, true, true, true},{true, true, true, true, true}, {true, true, true, true, true}, {true, true, true, true, true},{true, true, true, true, true}}
#define MEDIAN_7x7 {{true, true, true, true, true, true, true},{true, true, true, true, true, true, true}, {true, true, true, true, true, true, true}, {true, true, true, true, true, true, true}, {true, true, true, true, true, true, true}, {true, true, true, true, true, true, true},{true, true, true, true, true, true, true}}

#define MEDIAN_CROSS_3x3 {{false, true, false},{true, true, true},{false, true, false}}
#define MEDIAN_CROSS_5x5 {{false, false, true, false, false},{false, false, true, false, false}, {true, true, true, true, true}, {false, false, true, false, false},{false, false, true, false, false}}
#define MEDIAN_CROSS_7x7 {{false, false, false, true, false, false, false},{false, false, false, true, false, false, false}, {false, false, false, true, false, false, false}, {true, true, true, true, true, true, true}, {false, false, false, true, false, false, false}, {false, false, false, true, false, false, false},{false, false, false, true, false, false, false}}

public:
    MedianFilter();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;

private:
    void ChangeFilter(bool tmp3x3[3][3], bool tmp5x5[5][5], bool tmp7x7[7][7]);
    void DrawMedianDisplayArray();
    void DrawInputArray();

private:
    enum MedianFilters
    {
        Full,
        Cross,
        CustomM
    };

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
    int medianFilterType = Full;
    int medianFilterSize = S3x3;

    bool medianMask3x3[3][3] = MEDIAN_3x3;
    bool medianMask5x5[5][5] = MEDIAN_5x5;
    bool medianMask7x7[7][7] = MEDIAN_7x7;
};

#endif
