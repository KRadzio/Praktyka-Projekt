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
    inline bool CanBeAutoRefreshed() { return autoRefresh; }
    // TODO
    // virtual void Save();
    // virtual void Load();

// this methods may be overriden if needed, but the are defined by default
protected:
    virtual void CopyToLocalVariable(Image *outputImage);
    virtual bool Canceled(Image *outputImage);
    // the main difference is that one waits for permission from main thread (automatic in some set interval) and the other refreshes and tells the main thread that refresh was done
    // automatic in some interval (waits for permission from main thread, only then can it refresh)
    virtual void AutomaticRefresh(Image *outputImage);
    // the main difference is that one waits for permission from main thread (automatic in some set interval) and the other refreshes and tells the main thread that refresh was done
    // manual when the programer wants (refreshes and notifies the main thread)
    virtual void ManualRefresh(Image *outputImage);
    virtual void SaveToOutput(Image *outputImage);

protected:
    bool autoRefresh = true;
    std::string algorithmName = "None";
    Image copy; // the copys texture should not be created or copied (it causes memory leaks 233 bytes per texture modification)
};

#endif
