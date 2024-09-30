#ifndef _SHARED_PTR_HPP_
#define _SHARED_PTR_HPP_

#include <utility>
#include <type_traits>
#include <atomic>
template <typename T>
struct SpControlBlock
{
    T *ptr;
    std::atomic<int> refCount;

    explicit SpControlBlock(T *ptr) : ptr(ptr), refCount(1) {};
    ~SpControlBlock()
    {
        delete ptr;
    }

    void incRefCount()
    {
        refCount.fetch_add(1,std::memory_order_relaxed);
    }

    void decRefCount()
    {
        if (refCount.fetch_sub(1,std::memory_order_relaxed) == 1)
        {
            delete this;
        }
    }
};

template <typename T>
struct shared_ptr
{
    SpControlBlock<T> *m_cb;

    explicit shared_ptr(T *ptr) : m_cb(new SpControlBlock<T>(ptr)) {}

    shared_ptr(const shared_ptr &other) : m_cb(other.m_cb)
    {
        m_cb->incRefCount();
    }

    ~shared_ptr()
    {
        m_cb->decRefCount();
    }

    T *get() const
    {
        return m_cb->ptr;
    }

    T &operator*() const
    {
        return *m_cb->ptr;
    }

    T *operator->() const
    {
        return m_cb->ptr;
    }
};

template <class T, class... Args>
shared_ptr<T> make_shared(Args &&...args)
{
    return shared_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif