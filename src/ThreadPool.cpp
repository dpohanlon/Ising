#include <iostream>

#include "ThreadPool.h"

ThreadPool::ThreadPool(int n)
{
    this->nThreads = n;
    for (int i = 0; i < nThreads; ++i){
        threads.push_back(new boost::thread(waitState));
    }
}

ThreadPool::~ThreadPool(void)
{
    std::vector<boost::thread *>::iterator itr;
    for(itr = threads.begin(); itr!= threads.end(); ++itr){
        delete *itr;
        *itr = NULL;
    }
}

void ThreadPool::addJob(func f, double arg)
{
    funcQ.push(std::make_pair(f, arg));
}

void ThreadPool::join(void)
{
    std::vector<boost::thread *>::iterator itr;

    // To break infinite loop to join, give dummy 'break job'
    addJob(breakLoop, 0);
    
    for(itr = threads.begin(); itr != threads.end(); ++itr){
        (*itr)->join();
    }
}

void ThreadPool::waitState(void)
{
    while(1){
        if (!funcQ.empty()){
            if (funcQ.front().first == breakLoop) return;

            qMutex.lock();
            func localFunc = funcQ.front().first;
            double localArg = funcQ.front().second;
            funcQ.pop();
            qMutex.unlock();

            localFunc(localArg);
        }
    }
}

ThreadPool::funcQ_t ThreadPool::funcQ;
ThreadPool::threads_t ThreadPool::threads;
ThreadPool::mutex ThreadPool::qMutex;
