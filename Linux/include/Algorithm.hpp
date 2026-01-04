#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "imgui.h"

#include "Image.hpp"
#include "Mutex.hpp"

class Algorithm
{
public:
    Algorithm();
    virtual ~Algorithm(); // some may need an extra image

    // called when params menu opend
    virtual void ParamsMenu() = 0;
    // called when processing the image
    virtual void AlgorithmFunction(Image *outputImage) = 0;
    // called when params needs to be reset
    virtual void ResetToDefaults() = 0;

    inline std::string GetName() { return algorithmName; }
    // TODO
    // virtual void Save();
    // virtual void Load();

protected:
    void CopyToLocalVariable(Image *outputImage);
    bool Canceled(Image *outputImage);
    void AutoRefresh(Image *outputImage);
    void SaveToOutput(Image *outputImage);

protected:
    std::string algorithmName = "None";
    Image copy;
};

#endif
