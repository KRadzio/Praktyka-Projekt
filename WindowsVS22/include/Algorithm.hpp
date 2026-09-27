#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <string>
#include <vector>
#include <fstream>

#include <math.h>

#ifdef __linux__

#include <SDL2/SDL.h>

#elif _WIN32

#include <SDL.h>

#endif

#include "imgui.h"

#include "Image.hpp"
#include "Mutex.hpp"

#define CONFIG_SPLIT_CHAR_A '='
#define CONFIG_FILE_DELIM_A '\n'

// used in linear filter, median filter and morphological algorithms
#define CONFIG_ROW_DELIM_A '|'
// used in linear filter, median filter and morphological algorithms
#define CONFIG_COL_DELIM_A ';'

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

    // by default saves name and info that it is not impelented (to avoid the need to implement saving, but it is recomended to implement it)
    virtual void Save(std::ofstream& file);
    // by default load name (to avoid the need to implement loading, but it is recomended to implement it)
    virtual void Load(std::ifstream& file);

protected:
    // return a substring after the delimiter
    std::string SplitLine(std::ifstream& file);

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
    Image copy; // the copys textures should not be created or copied (it causes memory leaks 233 bytes per texture modification)
};

#endif
