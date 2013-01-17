#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <list>
#include <queue>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

class ThreadPool
{

public:

    typedef void (*func)(double);

    ThreadPool(int n);
    ~ThreadPool(void);

    void addJob(func f, double arg);

    int getNThreads(void){ return nThreads; }
    void join(void);

private:

    int nThreads;

    typedef std::pair<func, double> funcArg;
    typedef std::queue<funcArg, std::list<funcArg> > funcQ_t;
    typedef std::vector<boost::thread *> threads_t;

    static funcQ_t funcQ;
    static threads_t threads;

    static void waitState(void);
    static void breakLoop(double){ }

    typedef boost::mutex mutex;
    static mutex qMutex;

};

#endif
