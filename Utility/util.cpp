#include "util.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <signal.h> // for kill()
#include <sys/syscall.h>
#include <sys/stat.h>
namespace sylar
{
    uint64_t GetElapsedMS()
    {
        struct timespec ts = {0};
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    }
    pid_t GetThreadId()
    {

        return syscall(SYS_gettid);
    }
    std::string GetThreadName()
    {
        char thread_name[16] = {0};
        pthread_getname_np(pthread_self(), thread_name, 16);
        return std::string(thread_name);
    }
    void SetThreadName(const std::string &name)
    {
        pthread_setname_np(pthread_self(), name.substr(0, 15).c_str());
    }
    uint64_t GetFiberId()
    {
        return 0;
    }
}