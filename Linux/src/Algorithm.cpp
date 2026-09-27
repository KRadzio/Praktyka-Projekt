#include "Algorithm.hpp"

Algorithm::Algorithm() {}

Algorithm::~Algorithm() { copy.ClearImage(); }

void Algorithm::Save(std::ofstream &file) { file << algorithmName << " SAVE NOT IMPLEMENTED!" << std::endl; }

void Algorithm::Load(std::ifstream &file)
{
    std::string line;
    getline(file, line, '\n');
}

std::string Algorithm::SplitLine(std::ifstream &file)
{
    std::string line;
    std::string sub;
    getline(file, line, CONFIG_FILE_DELIM_A);
    sub = line.substr(line.find(CONFIG_SPLIT_CHAR_A) + 1);
    return sub;
}

void Algorithm::CopyToLocalVariable(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    copy.CopyNoTexture(*outputImage);
    Mutex::GetInstance().Unlock();
}

bool Algorithm::Canceled(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    // if canceled
    if (!Mutex::GetInstance().IsThreadRunning())
    {
        outputImage->CopyNoTexture(copy);
        copy.ClearImage();
        Mutex::GetInstance().Unlock();
        return true;
    }
    Mutex::GetInstance().Unlock();
    return false;
}

void Algorithm::AutomaticRefresh(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
    {
        outputImage->CopyNoTexture(copy);
        Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
    }
    Mutex::GetInstance().Unlock();
}

void Algorithm::ManualRefresh(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    outputImage->CopyNoTexture(copy);
    Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
    Mutex::GetInstance().Unlock();
}

void Algorithm::SaveToOutput(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    outputImage->CopyNoTexture(copy);
    copy.ClearImage();
    Mutex::GetInstance().Unlock();
}