#ifndef MUTEX_H
#define MUTEX_H


#ifdef _WIN32 || _WIN64
#else
#include <pthread.h>
#endif


namespace video_annotator
{

#ifdef _WIN32 || _WIN64
    class Mutex {
    public:
        Mutex() {
        }
        void lock() {
        }
        void unlock() {
        }

        class ScopedLock
        {
            Mutex & _mutex;
        public:
            ScopedLock(Mutex & mutex)
                : _mutex(mutex)
            {
                _mutex.lock();
            }
            ~ScopedLock()
            {
                _mutex.unlock();
            }
        };

    private:
    };
#else
    class Mutex {
    public:
        Mutex() {
            pthread_mutex_init( &m_mutex, NULL );
        }
        void lock() {
            pthread_mutex_lock( &m_mutex );
        }
        void unlock() {
            pthread_mutex_unlock( &m_mutex );
        }

        class ScopedLock
        {
            Mutex & _mutex;
        public:
            ScopedLock(Mutex & mutex)
                : _mutex(mutex)
            {
                _mutex.lock();
            }
            ~ScopedLock()
            {
                _mutex.unlock();
            }
        };

    private:
        pthread_mutex_t m_mutex;
    };
#endif

}


#endif // MUTEX_H
