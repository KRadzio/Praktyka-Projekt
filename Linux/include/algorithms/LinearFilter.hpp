#ifndef LINEARFILTER_HPP
#define LINEARFILTER_HPP

#include "Algorithm.hpp"

#define AVERAGE_3x3 {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}

#define AVERAGE_5x5 {{1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}}

#define AVERAGE_7x7 {{1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1}}

#define GAUSS_3x3 {{1, 4, 1}, {4, 12, 4}, {1, 4, 1}}

#define GAUSS_5x5 {{1, 4, 7, 4, 1}, {4, 16, 26, 16, 4}, {7, 26, 41, 26, 7}, {4, 16, 26, 16, 4}, {1, 4, 7, 4, 1}}

#define GAUSS_7x7 {{0, 0, 1, 2, 1, 0, 0}, {0, 3, 13, 22, 13, 3, 0}, {1, 13, 59, 97, 59, 13, 1}, {2, 22, 97, 159, 97, 22, 2}, {1, 13, 59, 97, 59, 13, 1}, {0, 3, 13, 22, 13, 3, 0}, {0, 0, 1, 2, 1, 0, 0}}

#define SOBEL_HORIZONTAL_3x3 {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}}

#define SOBEL_HORIZONTAL_5x5 {{2, 2, 4, 2, 2}, {1, 1, 2, 1, 1}, {0, 0, 0, 0, 0}, {-1, -1, -2, -1, -1}, {-2, -2, -4, -2, -2}}

#define SOBEL_HORIZONTAL_7x7 {{4, 4, 4, 8, 4, 4, 4}, {2, 2, 2, 4, 2, 2, 2}, {1, 1, 1, 2, 2, 1, 1}, {0, 0, 0, 0, 0, 0, 0}, {-1, -1, -1, -2, -1, -1, -1}, {-2, -2, -2, -4, -2, -2, -2}, {-4, -4, -4, -8, -4, -4, -4}}

#define SOBEL_VERTICAL_3x3 {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}

#define SOBEL_VERTICAL_5x5 {{2, 1, 0, -1, 2}, {2, 1, 0, -1, -2}, {4, 2, 0, -2, -4}, {2, 1, 0, -1, -2}, {2, 1, 0, -1, -2}}

#define SOBEL_VERTICAL_7x7 {{4, 2, 1, 0, -1, -2, -4}, {4, 2, 1, 0, -1, -2, -4}, {4, 2, 1, 0, -1, -2, -4}, {8, 4, 2, 0, -2, -4, -8}, {4, 2, 1, 0, -1, -2, -4}, {4, 2, 1, 0, -1, -2, -4}, {4, 2, 1, 0, -1, -2, -4}}

#define LAPLACIAN_3x3 {{-2, 1, -2}, {1, 4, 1}, {-2, 1, -2}}

#define LAPLACIAN_5x5 {{0, 0, 1, 0, 0}, {0, 1, 2, 1, 0}, {1, 2, -16, 2, 1}, {0, 1, 2, 1, 0}, {0, 0, 1, 0, 0}}

#define LAPLACIAN_7x7 {{0, 0, 1, 1, 1, 0, 0}, {0, 1, 3, 3, 3, 1, 0}, {1, 3, 0, -7, 0, 3, 1}, {1, 3, -7, -24, -7, 3, 1}, {1, 3, 0, -7, 0, 3, 1}, {0, 1, 3, 3, 3, 1, 0}, {0, 0, 1, 1, 1, 0, 0}}

#define SHARPENING_3x3 {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}}

#define SHARPENING_5x5 {{-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}, {-1, -1, 25, -1, -1}, {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}}

#define SHARPENING_7x7 {{-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, 49, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1}}

class LinearFilter : public Algorithm
{
public:
    LinearFilter();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;

private:
    void ChangeFilter(int32_t tmp3x3[3][3], int32_t tmp5x5[5][5], int32_t tmp7x7[7][7]);
    void DrawLinearInputArray();
    void DrawLinearDisplayArray();

private:
    enum LinearFilters
    {
        Average,
        Gauss,
        SobelHorizontal,
        SobelVertical,
        Laplasjan,
        Sharpening,
        CustomL
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
    int linerFilterType = Average;
    int linearFilterSize = S3x3;

    int32_t linearMask3x3[3][3] = AVERAGE_3x3;
    int32_t linearMask5x5[5][5] = AVERAGE_5x5;
    int32_t linearMask7x7[7][7] = AVERAGE_7x7;
};

#endif
