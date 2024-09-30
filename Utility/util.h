#ifndef __SYLAR_UTIL_H__
#define __SYLAR_UTIL_H__

#include <time.h>
#include <unistd.h>
#include <cstdint>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <string>
namespace sylar
{

    /// @brief 返回系统启动至现在的时间，单位毫秒
    /// @return
    uint64_t GetElapsedMS();

    pid_t GetThreadId();


    std::string GetThreadName();


    void SetThreadName(const std::string &name);

    uint64_t GetFiberId();
}
#endif