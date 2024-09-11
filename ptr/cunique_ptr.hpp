#ifndef __CUNIQUE_PTR_H__
#define __CUNIQUE_PTR_H__

#include <type_traits>
#include <utility>

/// 默认删除器
template <class T>
struct DefaultDeleter
{
    void operator()(T *ptr) const
    {
        delete ptr;
    }
};

/// 偏特化
template <class T>
struct DefaultDeleter<T[]>
{
    void operator()(T *ptr) const
    {
        delete[] ptr;
    }
};

template <class T, class Deleter = DefaultDeleter<T>>
struct CUniquePtr
{
private:
    T *m_ptr;
    Deleter m_deleter;
    template <class U, class UDeleter>
    friend struct CUniquePtr;

public:
    using element_type = T;
    using deleter_type = Deleter;
    using pointer = T *;

    /// 默认构造函数
    CUniquePtr(std::nullptr_t = nullptr) noexcept : m_ptr(nullptr)
    {
    }

    /// 自定义构造函数
    explicit CUniquePtr(T *ptr = nullptr) noexcept : m_ptr(ptr) {}

    /// 从子类型U转换为T类型的智能指针
    template <class U, class UD, class = std::enable_if_t<std::is_convertible_v<U *, T *>>>
    CUniquePtr(CUniquePtr<U, UD> &&other) noexcept : m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr;
    }
    // 析构
    ~CUniquePtr()
    {
        if (m_ptr)
        {
            m_deleter(m_ptr);
        }
    }

    // 删除拷贝与赋值构造函数
    CUniquePtr(const CUniquePtr &_that) = delete;
    CUniquePtr &operator=(const CUniquePtr &_that) = delete;

    // 移动构造
    CUniquePtr(CUniquePtr &&_that) : m_ptr(_that.m_ptr)
    {
        _that.m_ptr = nullptr;
    }

    // 移动赋值构造
    CUniquePtr &operator=(CUniquePtr &&_that) 
    {
        if (this != &_that)
        {
            if (m_ptr)
            {
                m_deleter(m_ptr);
            }

            m_ptr = _that.m_ptr;
            _that.m_ptr = nullptr;
        }
        return *this;
    }

    void swap(CUniquePtr &_that) noexcept
    {
        std::swap(m_ptr, _that.m_ptr);
    }

    T *get() const noexcept
    {
        return m_ptr;
    }

    T *operator->() const noexcept
    {
        return m_ptr;
    }

    T *release() noexcept
    {
        T *tmp = m_ptr;
        m_ptr = nullptr;
        return tmp;
    }

    void reset(T *ptr = nullptr) noexcept
    {
        if (m_ptr)
        {
            m_deleter(m_ptr);
        }
        m_ptr = ptr;
    }
};

#endif