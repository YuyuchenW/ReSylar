#ifndef __SYLAR_NONCOPYABLE_H__
#define __SYLAR_NONCOPYABLE_H__

namespace sylar
{

    class Noncopyable
    {
    public:
        Noncopyable() = default;
        ~Noncopyable() = default;
        /// @brief 禁止拷贝构造函数
        Noncopyable(const Noncopyable &) = delete;
        /// @brief 禁止赋值构造函数
        Noncopyable &operator=(const Noncopyable &) = delete;
    };

}

#endif