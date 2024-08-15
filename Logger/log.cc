#include "log.hpp"

namespace sylar
{

    const char *LogLevel::ToString(LogLevel::Level level)
    {
        switch (level)
        {
#define XX(name)         \
    case LogLevel::name: \
        return #name;

            XX(FATAL);
            XX(ALERT);
            XX(CRIT);
            XX(ERROR);
            XX(WARN);
            XX(NOTICE);
            XX(INFO);
            XX(DEBUG);
#undef XX
        default:
            return "NOTSET";
        }
    }

    LogLevel::Level sylar::LogLevel::FromString(const std::string &str)
    {
#define XX(Level, v) \
    if (str == #v)   \
        return LogLevel::Level;
        XX(FATAL, fatal);
        XX(ALERT, alert);
        XX(CRIT, crit);
        XX(ERROR, error);
        XX(WARN, warn);
        XX(NOTICE, notice);
        XX(INFO, info);
        XX(DEBUG, debug);
        XX(FATAL, FATAL);
        XX(ALERT, ALERT);
        XX(CRIT, CRIT);
        XX(ERROR, ERROR);
        XX(WARN, WARN);
        XX(NOTICE, NOTICE);
#undef XX
        return LogLevel::NOTSET;
    }

    LogEvent::LogEvent(const std::string &loggerName, LogLevel::Level level,
                       const char *file, int32_t line,
                       uint64_t elapse, uint64_t threadId,
                       uint32_t fiberId, const std::string &threadName,
                       time_t time) : m_loggerName(loggerName), m_level(level),
                                      m_file(file), m_line(line), m_elapse(elapse),
                                      m_threadId(threadId), m_fiberId(fiberId),
                                      m_threadName(threadName), m_time(time)

    {
    }

    void LogEvent::printf(const char *fmt, ...)
    {
        va_list ap;
        va_start(ap, fmt);
        vprintf(fmt, ap);
        va_end(ap);
    }

    void LogEvent::vprintf(const char *fmt, va_list va)
    {
        char *buf = nullptr;
        int len = vasprintf(&buf, fmt, va);
        if (len != -1)
        {
            m_ss << std::string(buf, len);
            free(buf);
        }
    }

    class MessageFormatItem : public LogFormatter::FormatterItem
    {
    public:
        MessageFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogEvent::ptr event) override
        {
            os << event->getContent();
        }
    };

    class LevelFormatItem : public LogFormatter::FormatterItem
    {
    public:
        LevelFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogEvent::ptr event) override
        {
            os << LogLevel::ToString(event->getLevel());
        }
    };

    class ElapseFormatItem : public LogFormatter::FormatterItem
    {
    public:
        ElapseFormatItem(const std::string &str) {}
        void format(std::ostream &os, LogEvent::ptr event) override
        {
            os << event->getElapse();
        }
    };

    class LoggerNameFormatItem : public LogFormatter::FormatterItem
    {
    public:
        LoggerNameFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogEvent::ptr event) override
        {
            os << event->getLoggerName();
        }
    };

    class DateTimeFormatItem : public LogFormatter::FormatterItem
    {
    public:
        DateTimeFormatItem(const std::string &format = "%Y-%m-%d %H:%M:%S")
            : m_format(format)
        {
            if (m_format.empty())
            {
                m_format = "%Y-%m-%d %H:%M:%S";
            }
        }
        void format(std::ostream &os, LogEvent::ptr even) override
        {
            struct tm tm;
            time_t time = even->getTime();
            localtime_r(&time, &tm);
            char buf[64];
            strftime(buf, sizeof(buf), m_format.c_str(), &tm);
            os << buf;
        }
    private:
        std::string m_format;
    };


}