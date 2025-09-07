#include "App.hpp"


int main()
{  
    int returnCode = App::GetInstance().Init();
    if(returnCode == -1)
        return returnCode;
    returnCode = App::GetInstance().MainLoop();
    return returnCode;
}