#include "../Logger/log.hpp"

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT(); // 默认INFO级别
int main()
{
    SYLAR_LOG_FATAL(g_logger) << "fatal msg";
    SYLAR_LOG_ERROR(g_logger) << "err msg";
    SYLAR_LOG_INFO(g_logger) << "info msg";
    SYLAR_LOG_DEBUG(g_logger) << "debug msg";
    sleep(1);
    sylar::SetThreadName("brand_new_thread");

    g_logger->setLevel(sylar::LogLevel::WARN);
    sylar::FileLogAppender::ptr fileAppender(new sylar::FileLogAppender("./log.txt"));
    g_logger->addAppender(fileAppender);
    SYLAR_LOG_FATAL(g_logger) << "fatal msg";
    SYLAR_LOG_ERROR(g_logger) << "err msg";
    SYLAR_LOG_INFO(g_logger) << "info msg";
    SYLAR_LOG_DEBUG(g_logger) << "debug msg";
    return 0;
}