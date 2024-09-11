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

    class ElapseFormatItem : public LogFormatter::FormatterItem
    {
    public:
        ElapseFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogEvent::ptr event) override
        {
            os << event->getElapse();
        }
    };

    class FileNameFormatItem : public LogFormatter::FormatterItem
    {
    public:
        FileNameFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogEvent::ptr event) override
        {
            os << event->getFile();
        }
    };

    class LineFormatItem : public LogFormatter::FormatterItem
    {
    public:
        LineFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogEvent::ptr event) override
        {
            os << event->getLine();
        }
    };

    class ThreadIdFormatItem : public LogFormatter::FormatterItem
    {
    public:
        ThreadIdFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogEvent::ptr event) override
        {
            os << event->getThreadId();
        }
    };

    class FiberIdFormatItem : public LogFormatter::FormatterItem
    {
    public:
        FiberIdFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogEvent::ptr event) override
        {
            os << event->getFiberId();
        }
    };

    class ThreadNameFormatItem : public LogFormatter::FormatterItem
    {
    public:
        ThreadNameFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogEvent::ptr event) override
        {
            os << event->getThreadName();
        }
    };

    class PercentSignFormatItem : public LogFormatter::FormatterItem
    {
    public:
        PercentSignFormatItem(const std::string &str) {}
        void format(std::ostream &os, LogEvent::ptr event) override
        {
            os << "%";
        }
    };

    class StringFormatItem : public LogFormatter::FormatterItem
    {
    public:
        StringFormatItem(const std::string &str) : m_string(str) {}
        void format(std::ostream &os, LogEvent::ptr event) override
        {
            os << m_string;
        }

    private:
        std::string m_string;
    };

    class TabFormatItem : public LogFormatter::FormatterItem
    {
    public:
        TabFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogEvent::ptr event) override
        {
            os << "\t";
        }
    };

    class NewLineFormatItem : public LogFormatter::FormatterItem
    {
    public:
        NewLineFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogEvent::ptr event) override
        {
            os << std::endl;
        }
    };

    LogFormatter::LogFormatter(const std::string &pattern) : m_pattern(pattern)
    {
        init();
    }

    /// 解析日志格式
    /// eg : pattern = "%d{%Y-%m-%d %H:%M:%S} [%rms]%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
    /// 年-月-日 时:分:秒 [累计运行毫秒数] \\t 线程id \\t 线程名称 \\t 协程id \\t [日志级别] \\t [日志器名称] \\t 文件名:行号 \\t 日志消息 换行符
    /// 简单的状态机判断，提取pattern中的常规字符和模式字符
    /// 一共有两种状态，一种是正在解析常规字符，一种是正在解析模式字符
    /// 其中特殊情况%d{%Y-%m-%d %H:%M:%S}，%%d，后面可以接一对大括号指定时间格式
    /// 一旦状态出错就停止解析，并设置错误标志，未识别的pattern转义字符也算出错
    void LogFormatter::init()
    {
        // 按字符串顺序解析pattern
        // 状态机，0表示正在解析常规字符，1表示正在解析模式字符，
        std::vector<std::pair<int, std::string>> patterns;
        // 临时变量，存储常规字符串
        std::string tmp;
        // 临时变量，存储时间格式，日期格式字符串，默认把位于%d后面的大括号对里的全部字符都当作格式字符，不校验格式是否合法
        std::string dateformat;

        bool isError = false;
        // 是否正在解析常规字符
        bool parsingString = true;

        size_t i = 0;
        while (i < m_pattern.size())
        {
            std::string c = std::string(1, m_pattern[i]);
            if (c == "%")
            {
                if (parsingString)
                {
                    if (!tmp.empty())
                    {
                        patterns.push_back(std::make_pair(0, tmp));
                    }
                    tmp.clear();
                    parsingString = false; // 进入模式字符状态
                    ++i;
                    continue;
                }
                else
                {
                    patterns.push_back(std::make_pair(1, c));
                    parsingString = true;
                    ++i;
                    continue;
                }
            }
            else
            {
                if (parsingString)
                {
                    tmp += c;
                    ++i;
                    continue;
                }
                else
                {
                    patterns.push_back(std::make_pair(1, c));
                    parsingString = true;

                    if (c != "d")
                    {
                        ++i;
                        continue;
                    }

                    ++i;
                    if (i < m_pattern.size() && m_pattern[i] != '{')
                    {
                        continue;
                    }
                    ++i;
                    while (i < m_pattern.size() && m_pattern[i] != '}')
                    {
                        dateformat.push_back(m_pattern[i]);
                        ++i;
                    }
                    if (m_pattern[i] != '}')
                    {
                        // %d后面没有大括号没有闭合，直接报错
                        std::cerr << "[ERROR] LogFormatter::init() " << "pattern: [" << m_pattern << "] '{' not closed" << std::endl;
                        isError = true;
                        break;
                    }
                    ++i;
                    continue;
                }
            }
        }
        if (isError)
        {
            m_error = true;
            return;
        }
        // 解析完毕，将临时变量tmp添加到patterns中
        if (!tmp.empty())
        {
            patterns.push_back(std::make_pair(0, tmp));
            tmp.clear();
        }

        static std::map<std::string, std::function<FormatterItem::ptr(const std::string &str)>> s_format_items = {
#define XX(str, C) {#str, [](const std::string &fmt) { return FormatterItem::ptr(new C(fmt)); }}
            XX(m, MessageFormatItem),     // m:消息
            XX(p, LevelFormatItem),       // p:日志级别
            XX(c, LoggerNameFormatItem),  // c:日志器名称
            XX(r, ElapseFormatItem),      // r:累计毫秒数
            XX(f, FileNameFormatItem),    // f:文件名
            XX(l, LineFormatItem),        // l:行号
            XX(t, ThreadIdFormatItem),    // t:编程号
            XX(F, FiberIdFormatItem),     // F:协程号
            XX(N, ThreadNameFormatItem),  // N:线程名称
            XX(%, PercentSignFormatItem), // %:百分号
            XX(T, TabFormatItem),         // T:制表符
            XX(n, NewLineFormatItem),     // n:换行符
#undef XX
        };

        for (auto &v : patterns)
        {
            if (v.first == 0)
            {
                m_items.push_back(FormatterItem::ptr(new StringFormatItem(v.second)));
            }
            else if (v.second == "d")
            {
                m_items.push_back(FormatterItem::ptr(new DateTimeFormatItem(dateformat)));
            }
            else
            {
                auto it = s_format_items.find(v.second);
                if (it == s_format_items.end())
                {
                    std::cout << "[ERROR] LogFormatter::init() " << "pattern: [" << m_pattern << "] " << "unknown format item: " << v.second << std::endl;
                    isError = true;
                    break;
                }
                m_items.push_back(it->second(v.second));
            }

            if (isError)
            {
                m_error = true;
                return;
            }
        }
    }

    std::string LogFormatter::format(LogEvent::ptr event)
    {
        std::stringstream ss;
        for (auto &i : m_items)
        {
            i->format(ss, event);
        }
        return ss.str();
    }

    std::ostream &LogFormatter::format(std::ostream &os, LogEvent::ptr event)
    {
        for (auto &i : m_items)
        {
            i->format(os, event);
        }
        return os;
    }

    LogAppender::LogAppender(LogFormatter::ptr default_formatter)
        : m_defaultFormatter(default_formatter)
    {
    }

    void LogAppender::setFormatter(LogFormatter::ptr val)
    {
        MutexType::Lock lock(m_mutex);
        m_formatter = val;
    }

    LogFormatter::ptr LogAppender::getFormatter()
    {
        MutexType::Lock lock(m_mutex);
        return m_formatter ? m_formatter : m_defaultFormatter;
    }

    StdoutLogAppender::StdoutLogAppender()
        : LogAppender(LogFormatter::ptr(new LogFormatter))
    {
    }
    void StdoutLogAppender::log(LogEvent::ptr event)
    {
        if (m_formatter)
        {
            m_formatter->format(std::cout, event);
        }
        else
        {
            m_defaultFormatter->format(std::cout, event);
        }
    }
    std::string StdoutLogAppender::toYamlString()
    {
        // TODO 后续加入配置模块
        return std::string();
    }

    FileLogAppender::FileLogAppender(const std::string &path) : LogAppender(LogFormatter::ptr(new LogFormatter))
    {
        m_path = path;
        reOpen();
        if (m_reopenError)
        {
            std::cout << "reopen file " << m_path << " error" << std::endl;
        }
    }

    void FileLogAppender::log(LogEvent::ptr event)
    {
        uint64_t now = event->getTime();
        if (now >= (m_lastTime + 3))
        {
            reOpen();
            if (m_reopenError)
            {
                std::cout << "reopen file " << m_path << " error" << std::endl;
            }
            m_lastTime = now;
        }
        if (m_reopenError)
        {
            return;
        }

        MutexType::Lock lock(m_mutex);
        if (m_formatter)
        {
            if (!m_formatter->format(m_filestream, event))
            {
                std::cout << "[ERROR] FileLogAppender::log() format error" << std::endl;
            }
        }
        else
        {
            if (!m_defaultFormatter->format(m_filestream, event))
            {
                std::cout << "[ERROR] FileLogAppender::log() format error" << std::endl;
            }
        }
    }

    std::string FileLogAppender::toYamlString()
    {
        // TODO 后续加入配置模块
        return std::string();
    }

    bool FileLogAppender::reOpen()
    {
        MutexType::Lock lock(m_mutex);
        if (m_filestream)
        {
            m_filestream.close();
        }
        m_filestream.open(m_path, std::ios::app);
        m_reopenError = !m_filestream;
        return !m_reopenError;
    }

    
}