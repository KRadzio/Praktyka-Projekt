#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <mutex>

class Mutex
{
public:
    enum RefreshState
    {
        WaitingForCounter,
        AlgorithmThreadRefresh,
        MainThreadRefresh
    };

public:
    static Mutex &GetInstance();

    inline void Lock() { mutex.lock(); }
    inline void Unlock() { mutex.unlock(); }

    inline int GetState() { return refreshState; }
    inline void SetState(int newState) { refreshState = newState; }

    inline bool IsThreadRunning() { return threadIsRunning; }

    inline void ThreadRunning() { threadIsRunning = true; }
    inline void ThreadStopped() { threadIsRunning = false; }

private:
    Mutex();
    ~Mutex();

private:
    std::mutex mutex;
    int refreshState = WaitingForCounter;
    bool threadIsRunning = false;
};

#endif
