#ifndef ALGORITHMS
#define ALGORITHMS

#include <array>
#include <vector>
#include <algorithm>

#include <SDL2/SDL.h>

#include "Image.hpp"
#include "Mutex.hpp"

#include "Negative.hpp"
#include "Brighten.hpp"
#include "Contrast.hpp"
#include "Exponentiation.hpp"
#include "LeveledHistogram.hpp"

// pre defined
// (it took some time)

#define AVERAGE_3x3                                  \
    {                                                \
        {                                            \
            {{1, 1, 1}}, {{1, 1, 1}}, {{1, 1, 1} } } \
    }
#define AVERAGE_5x5                                                                                          \
    {                                                                                                        \
        {                                                                                                    \
            {{1, 1, 1, 1, 1}}, {{1, 1, 1, 1, 1}}, {{1, 1, 1, 1, 1}}, {{1, 1, 1, 1, 1}}, {{1, 1, 1, 1, 1} } } \
    }
#define AVERAGE_7x7                                                                                                                                                                          \
    {                                                                                                                                                                                        \
        {                                                                                                                                                                                    \
            {{1, 1, 1, 1, 1, 1, 1}}, {{1, 1, 1, 1, 1, 1, 1}}, {{1, 1, 1, 1, 1, 1, 1}}, {{1, 1, 1, 1, 1, 1, 1}}, {{1, 1, 1, 1, 1, 1, 1}}, {{1, 1, 1, 1, 1, 1, 1}}, {{1, 1, 1, 1, 1, 1, 1} } } \
    }

#define GAUSS_3x3                                     \
    {                                                 \
        {                                             \
            {{1, 4, 1}}, {{4, 12, 4}}, {{1, 4, 1} } } \
    }
#define GAUSS_5x5                                                                                                     \
    {                                                                                                                 \
        {                                                                                                             \
            {{1, 4, 7, 4, 1}}, {{4, 16, 26, 16, 4}}, {{7, 26, 41, 26, 7}}, {{4, 16, 26, 16, 4}}, {{1, 4, 7, 4, 1} } } \
    }
#define GAUSS_7x7                                                                                                                                                                                                  \
    {                                                                                                                                                                                                              \
        {                                                                                                                                                                                                          \
            {{0, 0, 1, 2, 1, 0, 0}}, {{0, 3, 13, 22, 13, 3, 0}}, {{1, 13, 59, 97, 59, 13, 1}}, {{2, 22, 97, 159, 97, 22, 2}}, {{1, 13, 59, 97, 59, 13, 1}}, {{0, 3, 13, 22, 13, 3, 0}}, {{0, 0, 1, 2, 1, 0, 0} } } \
    }

#define SOBEL_HORIZONTAL_3x3                            \
    {                                                   \
        {                                               \
            {{1, 2, 1}}, {{0, 0, 0}}, {{-1, -2, -1} } } \
    }
#define SOBEL_HORIZONTAL_5x5                                                                                           \
    {                                                                                                                  \
        {                                                                                                              \
            {{2, 2, 4, 2, 2}}, {{1, 1, 2, 1, 1}}, {{0, 0, 0, 0, 0}}, {{-1, -1, -2, -1, -1}}, {{-2, -2, -4, -2, -2} } } \
    }
#define SOBEL_HORIZONTAL_7x7                                                                                                                                                                                      \
    {                                                                                                                                                                                                             \
        {                                                                                                                                                                                                         \
            {{4, 4, 4, 8, 4, 4, 4}}, {{2, 2, 2, 4, 2, 2, 2}}, {{1, 1, 1, 2, 2, 1, 1}}, {{0, 0, 0, 0, 0, 0, 0}}, {{-1, -1, -1, -2, -1, -1, -1}}, {{-2, -2, -2, -4, -2, -2, -2}}, {{-4, -4, -4, -8, -4, -4, -4} } } \
    }

#define SOBEL_VERTICAL_3x3                              \
    {                                                   \
        {                                               \
            {{-1, 0, 1}}, {{-2, 0, 2}}, {{-1, 0, 1} } } \
    }
#define SOBEL_VERTICAL_5x5                                                                                            \
    {                                                                                                                 \
        {                                                                                                             \
            {{2, 1, 0, -1, 2}}, {{2, 1, 0, -1, -2}}, {{4, 2, 0, -2, -4}}, {{2, 1, 0, -1, -2}}, {{2, 1, 0, -1, -2} } } \
    }
#define SOBEL_VERTICAL_7x7                                                                                                                                                                                        \
    {                                                                                                                                                                                                             \
        {                                                                                                                                                                                                         \
            {{4, 2, 1, 0, -1, -2, -4}}, {{4, 2, 1, 0, -1, -2, -4}}, {{4, 2, 1, 0, -1, -2, -4}}, {{8, 4, 2, 0, -2, -4, -8}}, {{4, 2, 1, 0, -1, -2, -4}}, {{4, 2, 1, 0, -1, -2, -4}}, {{4, 2, 1, 0, -1, -2, -4} } } \
    }

#define LAPLACIAN_3x3                                    \
    {                                                    \
        {                                                \
            {{-2, 1, -2}}, {{1, 4, 1}}, {{-2, 1, -2} } } \
    }
#define LAPLACIAN_5x5                                                                                          \
    {                                                                                                          \
        {                                                                                                      \
            {{0, 0, 1, 0, 0}}, {{0, 1, 2, 1, 0}}, {{1, 2, -16, 2, 1}}, {{0, 1, 2, 1, 0}}, {{0, 0, 1, 0, 0} } } \
    }
#define LAPLACIAN_7x7                                                                                                                                                                              \
    {                                                                                                                                                                                              \
        {                                                                                                                                                                                          \
            {{0, 0, 1, 1, 1, 0, 0}}, {{0, 1, 3, 3, 3, 1, 0}}, {{1, 3, 0, -7, 0, 3, 1}}, {{1, 3, -7, -24, -7, 3, 1}}, {{1, 3, 0, -7, 0, 3, 1}}, {{0, 1, 3, 3, 3, 1, 0}}, {{0, 0, 1, 1, 1, 0, 0} } } \
    }

#define SHARPENING_3x3                                   \
    {                                                    \
        {                                                \
            {{0, -1, 0}}, {{-1, 5, -1}}, {{0, -1, 0} } } \
    }
#define SHARPENING_5x5                                                                                                                \
    {                                                                                                                                 \
        {                                                                                                                             \
            {{-1, -1, -1, -1, -1}}, {{-1, -1, -1, -1, -1}}, {{-1, -1, 25, -1, -1}}, {{-1, -1, -1, -1, -1}}, {{-1, -1, -1, -1, -1} } } \
    }
#define SHARPENING_7x7                                                                                                                                                                                                                        \
    {                                                                                                                                                                                                                                         \
        {                                                                                                                                                                                                                                     \
            {{-1, -1, -1, -1, -1, -1, -1}}, {{-1, -1, -1, -1, -1, -1, -1}}, {{-1, -1, -1, -1, -1, -1, -1}}, {{-1, -1, -1, 49, -1, -1, -1}}, {{-1, -1, -1, -1, -1, -1, -1}}, {{-1, -1, -1, -1, -1, -1, -1}}, {{-1, -1, -1, -1, -1, -1, -1} } } \
    }

#define MEDIAN_3x3                                      \
    {                                                   \
        {                                               \
            {{true, true, true}}, {{true, true, true}}, \
            {                                           \
                {                                       \
                    true, true, true                    \
                }                                       \
            }                                           \
        }                                               \
    }
#define MEDIAN_5x5                                                                                                                                  \
    {                                                                                                                                               \
        {                                                                                                                                           \
            {{true, true, true, true, true}}, {{true, true, true, true, true}}, {{true, true, true, true, true}}, {{true, true, true, true, true}}, \
            {                                                                                                                                       \
                {                                                                                                                                   \
                    true, true, true, true, true                                                                                                    \
                }                                                                                                                                   \
            }                                                                                                                                       \
        }                                                                                                                                           \
    }
#define MEDIAN_7x7                                                                                                                                                                                                                                                                              \
    {                                                                                                                                                                                                                                                                                           \
        {                                                                                                                                                                                                                                                                                       \
            {{true, true, true, true, true, true, true}}, {{true, true, true, true, true, true, true}}, {{true, true, true, true, true, true, true}}, {{true, true, true, true, true, true, true}}, {{true, true, true, true, true, true, true}}, {{true, true, true, true, true, true, true}}, \
            {                                                                                                                                                                                                                                                                                   \
                {                                                                                                                                                                                                                                                                               \
                    true, true, true, true, true, true, true                                                                                                                                                                                                                                    \
                }                                                                                                                                                                                                                                                                               \
            }                                                                                                                                                                                                                                                                                   \
        }                                                                                                                                                                                                                                                                                       \
    }

#define MEDIAN_CROSS_3x3                                  \
    {                                                     \
        {                                                 \
            {{false, true, false}}, {{true, true, true}}, \
            {                                             \
                {                                         \
                    false, true, false                    \
                }                                         \
            }                                             \
        }                                                 \
    }
#define MEDIAN_CROSS_5x5                                                                                                                                        \
    {                                                                                                                                                           \
        {                                                                                                                                                       \
            {{false, false, true, false, false}}, {{false, false, true, false, false}}, {{true, true, true, true, true}}, {{false, false, true, false, false}}, \
            {                                                                                                                                                   \
                {                                                                                                                                               \
                    false, false, true, false, false                                                                                                            \
                }                                                                                                                                               \
            }                                                                                                                                                   \
        }                                                                                                                                                       \
    }
#define MEDIAN_CROSS_7x7                                                                                                                                                                                                                                                                                                      \
    {                                                                                                                                                                                                                                                                                                                         \
        {                                                                                                                                                                                                                                                                                                                     \
            {{false, false, false, true, false, false, false}}, {{false, false, false, true, false, false, false}}, {{false, false, false, true, false, false, false}}, {{true, true, true, true, true, true, true}}, {{false, false, false, true, false, false, false}}, {{false, false, false, true, false, false, false}}, \
            {                                                                                                                                                                                                                                                                                                                 \
                {                                                                                                                                                                                                                                                                                                             \
                    false, false, false, true, false, false, false                                                                                                                                                                                                                                                            \
                }                                                                                                                                                                                                                                                                                                             \
            }                                                                                                                                                                                                                                                                                                                 \
        }                                                                                                                                                                                                                                                                                                                     \
    }

#define EMPTY_3x3                                             \
    {                                                         \
        {                                                     \
            {{false, false, false}}, {{false, false, false}}, \
            {                                                 \
                {                                             \
                    false, false, false                       \
                }                                             \
            }                                                 \
        }                                                     \
    }

#define EMPTY_5x5                                                                                                                                                       \
    {                                                                                                                                                                   \
        {                                                                                                                                                               \
            {{false, false, false, false, false}}, {{false, false, false, false, false}}, {{false, false, false, false, false}}, {{false, false, false, false, false}}, \
            {                                                                                                                                                           \
                {                                                                                                                                                       \
                    false, false, false, false, false                                                                                                                   \
                }                                                                                                                                                       \
            }                                                                                                                                                           \
        }                                                                                                                                                               \
    }

#define EMPTY_7x7                                                                                                                                                                                                                                                                                                                         \
    {                                                                                                                                                                                                                                                                                                                                     \
        {                                                                                                                                                                                                                                                                                                                                 \
            {{false, false, false, false, false, false, false}}, {{false, false, false, false, false, false, false}}, {{false, false, false, false, false, false, false}}, {{false, false, false, false, false, false, false}}, {{false, false, false, false, false, false, false}}, {{false, false, false, false, false, false, false}}, \
            {                                                                                                                                                                                                                                                                                                                             \
                {                                                                                                                                                                                                                                                                                                                         \
                    false, false, false, false, false, false, false                                                                                                                                                                                                                                                                       \
                }                                                                                                                                                                                                                                                                                                                         \
            }                                                                                                                                                                                                                                                                                                                             \
        }                                                                                                                                                                                                                                                                                                                                 \
    }

// Skeletonization defines
#define NONE 0
#define SKELETON 2
#define REMOVE 3
#define W1_CHECK (S[0] == WHITE) && (S[4] == WHITE) && ((S[1] != WHITE) || (S[2] != WHITE) || (S[3] != WHITE)) && ((S[5] != WHITE) || (S[6] != WHITE) || (S[7] != WHITE))
#define W2_CHECK (S[2] == WHITE) && (S[6] == WHITE) && ((S[3] != WHITE) || (S[4] != WHITE) || (S[5] != WHITE)) && ((S[0] != WHITE) || (S[1] != WHITE) || (S[7] != WHITE))
#define W3_CHECK (S[4] == WHITE) && (S[6] == WHITE) && ((S[0] != WHITE) || (S[1] != WHITE) || (S[2] != WHITE) || (S[3] != WHITE) || (S[7] != WHITE)) && (S[5] != WHITE)
#define W4_CHECK (S[0] == WHITE) && (S[6] == WHITE) && ((S[1] != WHITE) || (S[2] != WHITE) || (S[3] != WHITE) || (S[4] != WHITE) || (S[5] != WHITE)) && (S[7] != WHITE)
#define W5_CHECK (S[0] == WHITE) && (S[2] == WHITE) && ((S[3] != WHITE) || (S[4] != WHITE) || (S[5] != WHITE) || (S[6] != WHITE) || (S[7] != WHITE)) && (S[1] != WHITE)
#define W6_CHECK (S[2] == WHITE) && (S[4] == WHITE) && ((S[0] != WHITE) || (S[1] != WHITE) || (S[5] != WHITE) || (S[6] != WHITE) || (S[7] != WHITE)) && (S[3] != WHITE)

// Hought defines
#define THETA_NUM 180

namespace Algorithms
{
    enum BinarizationMethod
    {
        None,
        Gradient,
        Histogram
    };

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

    struct ParametersStruct
    {
        // Brighten / Darken
        int value = 0;
        // Contrast
        float contrast = 1.0;
        // Exp
        float alfa = 1.0;
        // Binarization
        int boundCount = 1;
        int lowerBound = 0;
        int upperBound = 0;
        int method = None;
        // Linear Filters
        int linerFilterS = Average;
        int linearFilterSize = S3x3;
        std::array<std::array<int, 3>, 3> linearMask3x3 = AVERAGE_3x3;
        std::array<std::array<int, 5>, 5> linearMask5x5 = AVERAGE_5x5;
        std::array<std::array<int, 7>, 7> linearMask7x7 = AVERAGE_7x7;
        // Median Filters
        int medianFilterS = Full;
        int medianFilterSize = S3x3;
        std::array<std::array<bool, 3>, 3> medianMask3x3 = MEDIAN_3x3;
        std::array<std::array<bool, 5>, 5> medianMask5x5 = MEDIAN_5x5;
        std::array<std::array<bool, 7>, 7> medianMask7x7 = MEDIAN_7x7;
        // Erosion
        int erosionElementSize = S3x3;
        std::array<std::array<bool, 3>, 3> erosionElement3x3 = EMPTY_3x3;
        std::array<std::array<bool, 5>, 5> erosionElement5x5 = EMPTY_5x5;
        std::array<std::array<bool, 7>, 7> erosionElement7x7 = EMPTY_7x7;
        // Dilatation
        int dilatationElementSize = S3x3;
        std::array<std::array<bool, 3>, 3> dilatationElement3x3 = EMPTY_3x3;
        std::array<std::array<bool, 5>, 5> dilatationElement5x5 = EMPTY_5x5;
        std::array<std::array<bool, 7>, 7> dilatationElement7x7 = EMPTY_7x7;
        // Hought
        int maxIndexRo = 0;
        int maxIndexTheta = 0;
        int maxHoughtVal = 0;

    };

    void CreateNegative(Image *outputImage);

    void BrightenImage(Image *outputImage, ParametersStruct *params);

    void Contrast(Image *outputImage, ParametersStruct *params);

    void Exponentiation(Image *outputImage, ParametersStruct *params);

    void LevelHistogram(Image *outputImage);

    void Binarization(Image *outputImage, ParametersStruct *params);

    void LinearFilter(Image *outputImage, ParametersStruct *params);

    void MedianFilter(Image *outputImage, ParametersStruct *params);

    void Erosion(Image *outputImage, ParametersStruct *params);

    void Dilatation(Image *outputImage, ParametersStruct *params);

    void Skeletonization(Image *outputImage);

    void Hought(Image *outputImage, ParametersStruct *params);

}

#endif
