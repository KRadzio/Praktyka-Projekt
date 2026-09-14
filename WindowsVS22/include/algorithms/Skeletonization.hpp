#ifndef SKELETONIZATION_HPP
#define SKELETONIZATION_HPP

#include "Algorithm.hpp"

class Skeletonization : public Algorithm
{
private:
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

public:
    Skeletonization();

    void ParamsMenu() override;
    void AlgorithmFunction(Image *outputImage) override;
    void ResetToDefaults() override;
};

#endif
