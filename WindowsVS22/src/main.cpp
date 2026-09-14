#ifdef _WIN32

#include <Windows.h>

#pragma execution_character_set("utf-8")

#endif

#include "App.hpp"

// entry point
int main(int argc, char* argv[])
{  
    int returnCode = App::GetInstance().Init();
    if(returnCode == -1)
        return returnCode;
    returnCode = App::GetInstance().MainLoop();
    return returnCode;
}