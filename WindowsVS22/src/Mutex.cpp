#include "Mutex.hpp"

Mutex::Mutex() {}
Mutex::~Mutex() {}

Mutex &Mutex::GetInstance()
{
    static Mutex *instance = new Mutex();
    return *instance;
}
