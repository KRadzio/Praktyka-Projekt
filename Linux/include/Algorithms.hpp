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
#include "Binarization.hpp"
#include "LinearFilter.hpp"
#include "MedianFilter.hpp"

// pre defined
// (it took some time)

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
    enum MatrixSize
    {
        S3x3 = 3,
        S5x5 = 5,
        S7x7 = 7
    };

    struct ParametersStruct
    {
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

    void MedianFilter(Image *outputImage, ParametersStruct *params);

    void Erosion(Image *outputImage, ParametersStruct *params);

    void Dilatation(Image *outputImage, ParametersStruct *params);

    void Skeletonization(Image *outputImage);

    void Hought(Image *outputImage, ParametersStruct *params);

}

#endif
