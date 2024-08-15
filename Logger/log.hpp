#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__
#include <iostream>
#include <memory>
#include <sstream>
#include <fstream>
#include <vector>
#include "../Utility/cmutex.hpp"

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
                 const char *file, int32_t line, uint64_t elapse, uint64_t threadId, uint32_t fiberId, uint64_t threadName, time_t time);

        const std::string &getLoggerName() const { return m_loggerName; }
        const LogLevel::Level &getLoggerLevel() const { return m_level; }
        const char *getFile() const { return m_file; }
        const int32_t &geteLine() const { return m_line; }
        const uint64_t &getElapse() const { return m_elapse; }
        const uint64_t &getThreadId() const { return m_threadId; }

        const uint32_t &getFiberId() const { return m_fiberId; }
        const std::string &getThreadName() const { return m_threadName; }
        const time_t &getTime() const { return m_time; }
        const std::stringstream &getStringStream() { return m_ss; };

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

            virtual std::string format(std::ostream &os, LogEvent::ptr) = 0;
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
        // TODO 加锁

        /// @brief 构造函数
        /// @param formatter 默认格式器
        LogAppender(LogFormatter::ptr defaultFormatter);

        /// @brief 析构函数
        virtual ~LogAppender() {};
        void setFormatter(LogFormatter::ptr formatter);
        LogFormatter::ptr getFormatter() { return m_formatter; }

        virtual void log(LogEvent::ptr event) = 0;

        virtual std::string toYamlString() = 0;

    protected:
        LogFormatter::ptr m_defaultFormatter;
        LogFormatter::ptr m_formatter;
    };
};

#endif