#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <mutex>

class Mutex
{
public:
    static Mutex &GetInstance();

    inline void Lock() { mutex.lock(); }
    inline void Unlock() { mutex.unlock(); }

private:
    Mutex();
    ~Mutex();

private:
    std::mutex mutex;
};

#endif
