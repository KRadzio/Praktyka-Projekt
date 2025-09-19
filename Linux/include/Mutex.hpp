#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <mutex>

class Mutex
{

public:
    static Mutex &GetInstance();

    inline void Lock() { mutex.lock(); }
    inline void Unlock() { mutex.unlock(); }

    inline bool IsThreadRunning() { return threadIsRunning; }

    inline void ThreadRunning() { threadIsRunning = true; }
    inline void ThreadStopped() { threadIsRunning = false; }

private:
    Mutex();
    ~Mutex();

private:
    std::mutex mutex;
    bool threadIsRunning = false;
};

#endif
