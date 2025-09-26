#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <mutex>

// singleton
class Mutex
{
public:
    enum RefreshState
    {
        WaitingForCounter, // the counter is not 0
        AlgorithmThreadRefresh, // the counter is 0,  the algorithm thread needs to update shared resources
        MainThreadRefresh // the counter is 0, the main thread needs to refresh the texture and arrays, the counter need to be reset
    };

public:
    static Mutex &GetInstance();

    // locks the mutex
    inline void Lock() { mutex.lock(); }
    // unlocks the mutex
    inline void Unlock() { mutex.unlock(); }

    // returns the refresh state
    // see what each state means in Mutex::RefreshState
    inline int GetState() { return refreshState; }
    // sets the refresh state
    // see what each state means in Mutex::RefreshState
    inline void SetState(int newState) { refreshState = newState; }

    inline bool IsThreadRunning() { return threadIsRunning; }

    // raises the running flag
    inline void ThreadRunning() { threadIsRunning = true; }
    // lowers the running flag 
    inline void ThreadStopped() { threadIsRunning = false; }

private:
    Mutex();
    ~Mutex();

private:
    std::mutex mutex;
    // the state
    int refreshState = WaitingForCounter;
    // running flag
    bool threadIsRunning = false;
};

#endif
