#ifndef TIME_H
#define TIME_H


#include <time.h>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <sys/time.h>
#endif


namespace video_annotator
{

inline double time()
{
#ifdef _WIN32
    LARGE_INTEGER time,freq;
    if (!QueryPerformanceFrequency(&freq)){
        //  Handle error
        return 0;
    }
    if (!QueryPerformanceCounter(&time)){
        //  Handle error
        return 0;
    }
    return (double)time.QuadPart / freq.QuadPart;
#else
    struct timeval t;
    if (gettimeofday(&t, NULL))
    {
        //  Handle error
        return 0;
    }
    return (double)t.tv_sec + (double)t.tv_usec * .000001;
#endif
}

inline void sleep(int sec)
{
    const double start_time = time();
    while (time() - start_time < sec);
}

class Rate
{
public:
    Rate(int rate)
        : rate_(rate)
        , timestep_(1. / rate)
        , last_time_(-1.0)
    {
    }

    inline void sleep()
    {
        if (last_time_ < 0)
            last_time_ = time();
        while (time() - last_time_ < timestep_);
        last_time_ += timestep_;
    }

private:

    const int rate_;
    const double timestep_;
    double last_time_;
};

}


#endif // MUTEX_H
