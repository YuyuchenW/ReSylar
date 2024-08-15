#include "cmutex.hpp"

sylar::Semaphore::Semaphore(uint32_t count)
{
    sem_init(&m_semaphore, 0, count);
}

sylar::Semaphore::~Semaphore()
{
    sem_destroy(&m_semaphore);
}

void sylar::Semaphore::wait()
{
    sem_wait(&m_semaphore);
}

void sylar::Semaphore::notify()
{
    sem_post(&m_semaphore);
}
