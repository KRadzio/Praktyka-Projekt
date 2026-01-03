#include "Algorithm.hpp"

Algorithm::Algorithm() {}

Algorithm::~Algorithm() { copy.ClearImage(); }

void Algorithm::CopyToLocalVariable(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    copy = *outputImage;
    Mutex::GetInstance().Unlock();
}

bool Algorithm::Canceled(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    // if canceled
    if (!Mutex::GetInstance().IsThreadRunning())
    {
        *outputImage = copy;
        copy.ClearImage();
        Mutex::GetInstance().Unlock();
        return true;
    }
    Mutex::GetInstance().Unlock();
    return false;
}

void Algorithm::AutoRefresh(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    if (Mutex::GetInstance().GetState() == Mutex::AlgorithmThreadRefresh)
    {
        *outputImage = copy;
        Mutex::GetInstance().SetState(Mutex::MainThreadRefresh);
    }
    Mutex::GetInstance().Unlock();
}

void Algorithm::SaveToOutput(Image *outputImage)
{
    Mutex::GetInstance().Lock();
    Mutex::GetInstance().ThreadStopped();
    *outputImage = copy;
    copy.ClearImage();
    Mutex::GetInstance().Unlock();
}