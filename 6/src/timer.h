#ifndef INC_6__TIMER_H_
#define INC_6__TIMER_H_

#include <sys/time.h>

unsigned long get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

#endif  // INC_6__TIMER_H_
