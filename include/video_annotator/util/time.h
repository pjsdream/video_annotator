#ifndef TIME_H
#define TIME_H


#include <time.h>
#include <sys/time.h>


namespace video_annotator
{

inline double time()
{
    struct timeval t;
    if (gettimeofday(&t, NULL))
    {
        //  Handle error
        return 0;
    }
    return (double)t.tv_sec + (double)t.tv_usec * .000001;
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
