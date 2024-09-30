#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__
#include <iostream>
#include <memory>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include "../Utility/cmutex.hpp"
#include "../Utility/singleton.h"
#include "../Utility/util.h"
// 获取root日志器
#define SYLAR_LOG_ROOT() sylar::LoggerMgr::GetInstance()->getRoot()

// 获取指定日志器
#define SYLAR_LOG(name) sylar::LoggerMgr::GetInstance()->getLogger(name)
/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 * @details 构造一个LoggerWrap对象，包裹包含日志器和日志事件，在对象析构时调用日志器写日志事件
 * @todo 协程id未实现，暂时写0
 */
#define SYLAR_LOG_LEVEL(logger, level)                                                                                                               \
    if (level <= logger->getLevel())                                                                                                                 \
    sylar::LoggerWrap(logger, sylar::LogEvent::ptr(new sylar::LogEvent(logger->getName(),                                                            \
                                                                       level, __FILE__, __LINE__, sylar::GetElapsedMS() - logger->getCreateTime(),   \
                                                                       sylar::GetThreadId(), sylar::GetFiberId(), sylar::GetThreadName(), time(0)))) \
        .getLogEvent()                                                                                                                               \
        ->getSS()

#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)

#define SYLAR_LOG_ALERT(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ALERT)

#define SYLAR_LOG_CRIT(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::CRIT)

#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)

#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)

#define SYLAR_LOG_NOTICE(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::NOTICE)

#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)

#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)

namespace sylar
{
    class LogLevel
    {
    public:
        enum Level
        {
            /// 致命情况，系统不可用
            FATAL = 0,
            /// 高优先级情况，例如数据库系统崩溃
            ALERT = 100,
            /// 严重错误，例如硬盘错误
            CRIT = 200,
            /// 错误
            ERROR = 300,
            /// 警告
            WARN = 400,
            /// 正常但值得注意
            NOTICE = 500,
            /// 一般信息
            INFO = 600,
            /// 调试信息
            DEBUG = 700,
            /// 未设置
            NOTSET = 800,
        };
        /**
         * @brief 从日志级别转为字符串
         * @param 日志级别
         * @return 日志级别的字符串形式
         * */
        static const char *ToString(LogLevel::Level level);

        /// @brief 从字符串转为
        /// @param 日志级别的字符串(std)
        /// @return 日志级别
        static LogLevel::Level FromString(const std::string &str);
    };

    /**
     * @brief 日志事件
     */
    class LogEvent
    {
    public:
        typedef std::shared_ptr<LogEvent> ptr;
        /// @brief 构造函数
        /// @param loggerName 日志名称
        /// @param level 日志级别
        /// @param file 文件名
        /// @param line 行号
        /// @param elapse 从日志器创建到当前的累计时间（ms）
        /// @param threadId 线程id
        /// @param fiberId 协程id
        /// @param threadName 线程名称
        /// @param time UTC时间
        LogEvent(const std::string &loggerName, LogLevel::Level level,
                 const char *file, int32_t line, uint64_t elapse,
                 uint64_t threadId, uint32_t fiberId,
                 const std::string &threadName, time_t time);

        const std::string &getLoggerName() const { return m_loggerName; }
        const LogLevel::Level &getLoggerLevel() const { return m_level; }
        const char *getFile() const { return m_file; }
        const int32_t &geteLine() const { return m_line; }
        const uint64_t &getElapse() const { return m_elapse; }
        const uint64_t &getThreadId() const { return m_threadId; }
        std::string getContent() const { return m_ss.str(); }
        const uint32_t &getFiberId() const { return m_fiberId; }
        const std::string &getThreadName() const { return m_threadName; }
        const time_t &getTime() const { return m_time; }
        const int32_t &getLine() const { return m_line; }
        const LogLevel::Level &getLevel() { return m_level; }
        const std::stringstream &getStringStream() { return m_ss; };

        std::stringstream &getSS() { return m_ss; }

        /// @brief 写入日志，使用printf风格格式
        /// @param fmt 格式模板
        void printf(const char *fmt, ...);
        /// @brief 以c vprintf风格写入日志
        /// @param fmt
        /// @param va
        void vprintf(const char *fmt, va_list va);

    private:
        // 日志名称
        std::string m_loggerName;
        // 日志级别
        LogLevel::Level m_level;
        // 文件名
        const char *m_file;
        // 行号
        int32_t m_line;
        // 从日志器创建到当前的累计时间（ms）
        uint64_t m_elapse;
        // 线程id
        uint64_t m_threadId;
        // 协程id
        uint32_t m_fiberId;
        // 线程名称
        std::string m_threadName;
        // UTC时间
        time_t m_time;
        // 日志内容，使用stringstream存储，便于流式写入日志
        std::stringstream m_ss;
    };

    /// @brief 日志格式化
    class LogFormatter
    {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        /**
         * @brief 构造函数
         * @param[in] pattern 格式模板，参考sylar与log4cpp
         * @details 模板参数说明：
         * - %%m 消息
         * - %%p 日志级别
         * - %%c 日志器名称
         * - %%d 日期时间，后面可跟一对括号指定时间格式，比如%%d{%%Y-%%m-%%d %%H:%%M:%%S}，这里的格式字符与C语言strftime一致
         * - %%r 该日志器创建后的累计运行毫秒数
         * - %%f 文件名
         * - %%l 行号
         * - %%t 线程id
         * - %%F 协程id
         * - %%N 线程名称
         * - %%% 百分号
         * - %%T 制表符
         * - %%n 换行
         *
         * 默认格式：%%d{%%Y-%%m-%%d %%H:%%M:%%S}%%T%%t%%T%%N%%T%%F%%T[%%p]%%T[%%c]%%T%%f:%%l%%T%%m%%n
         *
         * 默认格式描述：年-月-日 时:分:秒 [累计运行毫秒数] \\t 线程id \\t 线程名称 \\t 协程id \\t [日志级别] \\t [日志器名称] \\t 文件名:行号 \\t 日志消息 换行符
         */
        LogFormatter(const std::string &pattern = "%d{%Y-%m-%d %H:%M:%S} [%rms]%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");

        /// @brief 初始化，解析格式模板，提取模板项
        void init();

        /// @brief 解析格式是否出错
        /// @return 有错误返回true
        bool isError() const { return m_error; }

        /// @brief 对日志事件进行格式化，返回字符串
        /// @param  event 日志事件
        /// @return 格式化日志字符串
        std::string format(LogEvent::ptr even);

        /// @brief 对日志事件进行格式化，返回格式化日志流
        /// @param os 日志输出流
        /// @param event 日志事件
        /// @return 格式化日志流
        std::ostream &format(std::ostream &os, LogEvent::ptr event);

        /// @brief 获取格式模板
        /// @return 格式模板字符串
        std::string getPattern() const { return m_pattern; }

    public:
        /// @brief 日志内容格式化项， 虚基类，用于派生不同的格式项
        class FormatterItem
        {
        public:
            typedef std::shared_ptr<FormatterItem> ptr;

            /// @brief 析构函数
            virtual ~FormatterItem() {};

            virtual void format(std::ostream &os, LogEvent::ptr) = 0;
        };

    private:
        std::string m_pattern;
        /// 解析后的格式模板数据
        std::vector<FormatterItem::ptr> m_items;

        bool m_error;
    };

    /// @brief 日志输出目标，虚基类，用于派生不同的日志输出目标
    class LogAppender
    {
    public:
        typedef std::shared_ptr<LogAppender> ptr;
        typedef Spinlock MutexType;

        /// @brief 构造函数
        /// @param formatter 默认格式器
        LogAppender(LogFormatter::ptr defaultFormatter);

        /// @brief 析构函数
        virtual ~LogAppender() {};
        void setFormatter(LogFormatter::ptr formatter);
        LogFormatter::ptr getFormatter();

        virtual void log(LogEvent::ptr event) = 0;

        virtual std::string toYamlString() = 0;

    protected:
        MutexType m_mutex;
        LogFormatter::ptr m_defaultFormatter;
        LogFormatter::ptr m_formatter;
    };

    /// @brief 输出到控制台
    class StdoutLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;

        /// @brief 构造函数
        StdoutLogAppender();

        /// @brief 析构函数
        virtual ~StdoutLogAppender() {};

        void log(LogEvent::ptr event) override;

        std::string toYamlString() override;
    };

    /// @brief 输出到文件
    class FileLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;

        /// @brief 构造函数
        /// @param path 文件路径
        FileLogAppender(const std::string &path);

        void log(LogEvent::ptr event) override;

        std::string toYamlString() override;

        bool reOpen();

    private:
        /// 文件路径
        std::string m_path;
        /// 文件流
        std::ofstream m_filestream;
        /// 上次重打打开时间
        uint64_t m_lastTime = 0;
        /// 文件打开错误标识
        bool m_reopenError = false;
    };

    /// @brief 日志器
    class Logger
    {
    public:
        typedef std::shared_ptr<Logger> ptr;
        typedef Spinlock MutexType;

        /// @brief 构造函数
        /// @param name 日志名称
        Logger(const std::string &name = "root");

        /// @brief 获取日志器创建时间
        /// @return 时间戳
        const uint64_t &getCreateTime() const { return m_createTime; }

        /// @brief 获取日志器名
        /// @return 日志器名称
        const std::string &getName() const { return m_name; }

        /// @brief 获取日志器等级
        /// @return 日志器等级
        const LogLevel::Level &getLevel() const { return m_level; }
        /// @brief 设置日志器等级
        /// @param level 日志等级
        void setLevel(LogLevel::Level level) { m_level = level; }

        /// @brief 添加日志输出地
        /// @param appender 日志输出目标
        void addAppender(LogAppender::ptr appender);

        /// @brief 删除目的输出目标
        /// @param appender 日志输出目标
        void delAppender(LogAppender::ptr appender);

        /// @brief 清空日志输出目标
        void clearAppenders();
        /// @brief 写日志
        /// @param event 事件
        void log(LogEvent::ptr event);

        std::string toYamlString();

    private:
        /// mutex
        MutexType m_mutex;
        /// 日志器名字
        std::string m_name;
        /// 日志级别
        LogLevel::Level m_level;
        /// 日志输出列表
        std::list<LogAppender::ptr> m_appenders;
        /// 日志创建时间
        uint64_t m_createTime;
    };

    /// @brief 日志事件包装器，方便宏定义，内部包含日志事件和日志器
    class LoggerWrap
    {
    public:
        /// @brief 构造函数
        /// @param logger 日志器
        /// @param event 日志事件
        LoggerWrap(Logger::ptr logger, LogEvent::ptr event);

        /// @brief 析构函数
        /// @details 日志器在析构时进行输出
        ~LoggerWrap();

        LogEvent::ptr getLogEvent() const { return m_event; }

    private:
        Logger::ptr m_logger;
        LogEvent::ptr m_event;
    };

    /// @brief 日志管理器
    class LogManager
    {
    public:
        typedef Spinlock MutexType;
        /// @brief 构造函数
        LogManager();
        Logger::ptr getLogger(const std::string &name);

        /// @brief 初始化，从配置文件中加载日志配置
        void init();
        std::string toYamlString();
        Logger::ptr getRoot() { return m_root; }

    private:
        MutexType m_mutex;
        /// 日志器集合 name - logger
        std::unordered_map<std::string, Logger::ptr> m_loggers;
        /// 根日志器
        Logger::ptr m_root;
    };

    /// 日志器管理类单例
    typedef sylar::Singleton<LogManager> LoggerMgr;

};

#endif