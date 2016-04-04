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

}


#endif // MUTEX_H
