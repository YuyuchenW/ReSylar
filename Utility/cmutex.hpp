#ifndef __SYLAR_CMUTEX_H__
#define __SYLAR_CMUTEX_H__

#include <thread>
#include <functional>
#include <memory>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <atomic>
#include <list>
#include "noncopyable.h"
namespace sylar
{
    /// @brief 信号量
    class Semaphore
    {
    public:
        Semaphore(uint32_t count = 0);
        ~Semaphore();
        /// @brief  获取信号量
        void wait();

        /// @brief 释放信号量
        void notify();

    private:
        sem_t m_semaphore;
    };
    /// @brief 局部锁模板实现，使用RAII思想
    template <class T>
    struct ScopedLockImpl
    {
    public:
        /// @brief 构造函数
        /// @param mutex mutex
        ScopedLockImpl(T &mutex)
            : m_mutex(mutex)
        {
            m_mutex.lock();
            b_locked = true;
        }
        ScopedLockImpl(const ScopedLockImpl &) = delete;            // 禁止复制构造
        ScopedLockImpl &operator=(const ScopedLockImpl &) = delete; // 禁止赋值
        ~ScopedLockImpl()
        {
            unlock();
        }
        void lock()
        {
            if (b_locked)
            {
                return;
            }
            m_mutex.lock();
            b_locked = true;
        }
        void unlock()
        {
            if (!b_locked)
            {
                return;
            }
            m_mutex.unlock();
            b_locked = false;
        }

    private:
        T &m_mutex;
        bool b_locked;
    };

    /// @brief  局部读锁模板实现
    template <class T>
    struct ReadScopedLockImpl
    {
    public:
        /// @brief 构造函数
        /// @param mutex  读写锁
        ReadScopedLockImpl(T &mutex)
            : m_mutex(mutex)
        {
            m_mutex.rdlock();
            b_locked = true;
        }
        ReadScopedLockImpl(const ReadScopedLockImpl &) = delete;            // 禁止复制构造
        ReadScopedLockImpl &operator=(const ReadScopedLockImpl &) = delete; // 禁止赋值

        ~ReadScopedLockImpl()
        {
            unlock();
        }

        void lock()
        {
            if (b_locked)
            {
                return;
            }
            m_mutex.rdlock();
            b_locked = true;
        }

        void unlock()
        {
            if (!b_locked)
            {
                return;
            }
            m_mutex.unlock();
            b_locked = false;
        }

    private:
        T &m_mutex;
        bool b_locked;
    };

    /// @brief  局部写锁模板实现
    /// @tparam T
    template <class T>
    struct WriteScopedLockImpl
    {
    public:
        /// @brief 构造函数
        /// @param mutex 读锁
        WriteScopedLockImpl(T &mutex)
            : m_mutex(mutex)
        {
            m_mutex.wrlock();
            b_locked = true;
        }

        ~WriteScopedLockImpl()
        {
            unlock();
        }

        WriteScopedLockImpl(const WriteScopedLockImpl &) = delete;            // 禁止复制构造
        WriteScopedLockImpl &operator=(const WriteScopedLockImpl &) = delete; // 禁止赋值

        void unlock()
        {
            if (!b_locked)
            {
                return;
            }
            m_mutex.unlock();
            b_locked = false;
        }

        void lock()
        {
            if (b_locked)
            {
                return;
            }
            m_mutex.wrlock();
            b_locked = true;
        }

    private:
        T &m_mutex;
        bool b_locked;
    };

    /// @brief  互斥量
    class Mutex : Noncopyable
    {
    public:
        Mutex()
        {
            pthread_mutex_init(&m_mutex, nullptr);
        }
        ~Mutex()
        {
            pthread_mutex_destroy(&m_mutex);
        }
        void lock()
        {
            pthread_mutex_lock(&m_mutex);
        }
        void unlock()
        {
            pthread_mutex_unlock(&m_mutex);
        }

    private:
        pthread_mutex_t m_mutex;
    };

    /// @brief  读写锁
    class RWMutex : Noncopyable
    {
    public:
        /// @brief 局部读锁
        typedef ReadScopedLockImpl<RWMutex> ReadLock;

        /// @brief 局部写锁
        typedef WriteScopedLockImpl<RWMutex> WriteLock;

        RWMutex()
        {
            pthread_rwlock_init(&m_rwlock, nullptr);
        }

        ~RWMutex()
        {
            pthread_rwlock_destroy(&m_rwlock);
        }

        void rdlock()
        {
            pthread_rwlock_rdlock(&m_rwlock);
        }

        void wrlock()
        {
            pthread_rwlock_wrlock(&m_rwlock);
        }

        void unlock()
        {
            pthread_rwlock_unlock(&m_rwlock);
        }

    private:
        pthread_rwlock_t m_rwlock;
    };

    /// @brief  自旋锁
    class Spinlock : Noncopyable
    {
    public:
        typedef ScopedLockImpl<Spinlock> Lock;
        /// @brief 构造函数
        Spinlock()
        {
            pthread_spin_init(&m_spin, 0);
        }

        /// @brief  构造
        ~Spinlock()
        {
            pthread_spin_destroy(&m_spin);
        }

        /// @brief  获取锁
        void lock()
        {
            pthread_spin_lock(&m_spin);
        }

        void unlock()
        {
            pthread_spin_unlock(&m_spin);
        }

    private:
        pthread_spinlock_t m_spin;
    };

    /// @brief  CAS锁
    /// @details  CAS锁，基于原子变量，性能比互斥量高
    class CASLock : Noncopyable
    {
    public:
        typedef ScopedLockImpl<CASLock> Lock;
        /// @brief 构造函数
        CASLock()
        {
            m_mutex.clear();
        }

        ~CASLock() {};

        void lock()
        {
            while (std::atomic_flag_test_and_set_explicit(&m_mutex, std::memory_order_acquire))
                ;
        }

        void unlock()
        {
            std::atomic_flag_clear_explicit(&m_mutex, std::memory_order_release);
        }

    private:
        volatile std::atomic_flag m_mutex;
    };
}

#endif