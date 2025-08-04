#include <timeUtil.h>
#include <time.h>
#include <iostream>
uint64_t getTickCount64()
{
    struct timespec ts;
    if (0 == clock_gettime(CLOCK_MONOTONIC, &ts)) 
    {
        return static_cast<uint64_t>(ts.tv_sec) * 1000 + static_cast<uint64_t>(ts.tv_nsec) / 1000000;
    } 
    else 
    {
        std::cerr << "Error: clock_gettime failed" << std::endl;
        return 0;
    }
}
