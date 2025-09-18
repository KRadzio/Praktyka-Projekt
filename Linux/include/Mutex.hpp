#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <mutex>

class Mutex
{
public:
    enum ThreadExitCode
    {
        Error = -1,
        Normal = 0,
        Stoped = 1,
        Awating = 2
    };

public:
    static Mutex &GetInstance();

    inline void Lock() { mutex.lock(); }
    inline void Unlock() { mutex.unlock(); }

    inline int GetOutputCode() { return outputCode; }
    inline bool IsThreadRunning() { return threadIsRunning; }

    inline void SetOutputCode(int code) { outputCode = code; }
    inline void ThreadRunning() { threadIsRunning = true; }
    inline void ThreadStopped() { threadIsRunning = false; }

private:
    Mutex();
    ~Mutex();

private:
    std::mutex mutex;
    int outputCode = Awating;
    bool threadIsRunning = false;
};

#endif
