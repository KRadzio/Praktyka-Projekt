#ifdef _WIN32

#include <Windows.h>

#pragma execution_character_set("utf-8")

#endif

#include "App.hpp"

// entry point
// windows need argc and argv
// on linux it shows warnings since not used
#ifdef __linux__
int main()
#elif _WIN32
int main(int argc, char *argv[])
#endif
{
    int returnCode = App::GetInstance().Init();
    if (returnCode == -1)
        return returnCode;
    returnCode = App::GetInstance().MainLoop();
    return returnCode;
}