# 日志模块
## 日志模块概述
用于格式化输出程序日志，方便从日志中定位程序运行过程中出现的问题。这里的日志除了日志内容本身之外，还应该包括文件名/行号，时间戳，线程/协程号，模块名称，日志级别等额外信息，甚至在打印致命的日志时，还应该附加程序的栈回溯信息，以便于分析和排查问题。

## 主要功能
### 区分不同级别
DEBUG/INFO/WARN/ERROR等级别。日志模块可以通过指定级别实现只输出某个级别以上的日志，这样可以灵活开关一些不重要的日志输出，比如程序在调试阶段可以设置一个较低的级别，以便看到更多的调度日志信息，程序发布之后可以设置一个较高的级别，以减少日志输出，提高性能。
### 输出地点
不同的日志可以输出到不同的位置，比如可以输出到标准输出，输出到文件，输出到syslog，输出到网络上的日志服务器等，甚至同一条日志可以同时输出到多个输出地。
###  区分不同的类别
日志可以分类并命名，一个程序的各个模块可以使用不同的名称来输出日志，这样可以很方便地判断出当前日志是哪个程序模块输出的。
### 可配置
可以按需指定每条日志是否包含文件名/行号、时间戳、线程/协程号、日志级别、启动时间等内容。


## 以log4cpp为例子
~~~cpp
#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/PropertyConfigurator.hh>
 
int main() {
    // 日志类别
    log4cpp::Category &root_logger = log4cpp::Category::getRoot(); // 获取root日志器，root日志器名称默认为空
    log4cpp::Category &file_logger = log4cpp::Category::getInstance("file_logger"); // 获取指定名称为file_logger的日志器
 
    // 日志输出地
    log4cpp::Appender *coutAppender = new log4cpp::OstreamAppender("cout", &std::cout); // 输出到终端
    log4cpp::FileAppender *fileAppender = new log4cpp::FileAppender("file", "./log.txt"); // 输出到文件
 
    // 日志格式
    log4cpp::PatternLayout *coutPattern = new log4cpp::PatternLayout();
    coutPattern->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S:%l}] %p %m%n"); // 格式：[年:月:日 时:分:秒:毫秒] 日志级别 日志内容   
    log4cpp::PatternLayout *filePattern = new log4cpp::PatternLayout();
    filePattern->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S:%l}] %R %c %p %m%n"); // 格式：[年:月:日 时:分:秒:毫秒] UTC秒数 日志器名称 日志级别 日志内容  
 
    // 绑定日志器和appender，设置日志器的输出级别
    coutAppender->setLayout(coutPattern);
    root_logger.addAppender(coutAppender);
    root_logger.setPriority(log4cpp::Priority::INFO);
    fileAppender->setLayout(filePattern);
    file_logger.addAppender(fileAppender);
    file_logger.setPriority(log4cpp::Priority::ERROR);
 
    // 支持c风格的日志打印和流式的日志打印
    root_logger.debug("debug msg");
    root_logger.debugStream() << "debug msg by stream";
    root_logger.info("info msg");
    root_logger.infoStream() << "info msg by stream";
    root_logger.error("error msg");
    root_logger.errorStream() << "err msg by stream";
 
    file_logger.debug("debug msg");
    file_logger.debugStream() << "debug msg by stream";
    file_logger.info("info msg");
    file_logger.infoStream() << "info msg by stream";
    file_logger.error("error msg");
    file_logger.errorStream() << "err msg by stream";
 
    return 0;
}
~~~

输出内容如下
~~~
[2021-06-25 09:33:59:734] INFO info msg
[2021-06-25 09:33:59:734] INFO info msg by stream
[2021-06-25 09:33:59:734] ERROR error msg
[2021-06-25 09:33:59:734] ERROR err msg by stream
[2021-06-25 09:33:59:734] ERROR error msg
[2021-06-25 09:33:59:734] ERROR err msg by stream
~~~

对于log4cpp总结如下：

1. Category对应日志类别，使用不同的名称来区别日志器，使用 log4cpp::Category::getInstance() 获取指定名称的日志器实例，如果两个日志器名称相同，那么对应同一个日志器实例。

2. 使用Category的的setPriority()方法设置日志器的日志级别，日志级别使用log4cpp::Priority枚举值来表示，一共有FATAL/ALERT/CRIT/ERROR/WARN/NOTICE/INFO/DEBUG/NOTSET几个等级。

3. 使用Appender来表示日志输出地，Appender可以细分为OstreamAppender和FileAppender等不同类型。一个Category可以有多个Appender，这样一条日志就可以输出到多个地方，通过addAppender()方法为Category新增Appender。

4. 使用PatternLayout来表示日志的格式，格式通过模板字符串来指定，比如%d表示时间（后面可用{}指定具体的时间格式），%R表示UTC秒数，%c表示日志器名称，%p表示日志级别，%m表示日志消息等。

5. PatternLayout和Appender绑定，Priority和Category绑定，一条日志经过Category判断级别通过后由Appender输出，Appender输出的格式由PatternLayout指定。

## 日志模块设计

### 日志级别
~~~
enum Level {
    /// 致命情况，系统不可用
    FATAL  = 0,
    /// 高优先级情况，例如数据库系统崩溃
    ALERT  = 100,
    /// 严重错误，例如硬盘错误
    CRIT   = 200,
    /// 错误
    ERROR  = 300,
    /// 警告
    WARN   = 400,
    /// 正常但值得注意
    NOTICE = 500,
    /// 一般信息
    INFO   = 600,
    /// 调试信息
    DEBUG  = 700,
    /// 未设置
    NOTSET = 800,
};
~~~


### 关键类
~~~cpp
class LogFormatter;
class LogAppender;
class Logger;
class LogEvent;
class LogEventWrap;
class LogManager;
~~~

关于这几个类的设计如下：

LogFormatter: 日志格式器，与log4cpp的PatternLayout对应，用于格式化一个日志事件。该类构建时可以指定pattern，表示如何进行格式化。提供format方法，用于将日志事件格式化成字符串。

LogAppender: 日志输出器，用于将一个日志事件输出到对应的输出地。该类内部包含一个LogFormatter成员和一个log方法，日志事件先经过LogFormatter格式化后再输出到对应的输出地。从这个类可以派生出不同的Appender类型，比如StdoutLogAppender和FileLogAppender，分别表示输出到终端和文件。

Logger: 日志器，负责进行日志输出。一个Logger包含多个LogAppender和一个日志级别，提供log方法，传入日志事件，判断该日志事件的级别高于日志器本身的级别之后调用LogAppender将日志进行输出，否则该日志被抛弃。

LogEvent: 日志事件，用于记录日志现场，比如该日志的级别，文件名/行号，日志消息，线程/协程号，所属日志器名称等。

LogEventWrap: 日志事件包装类，其实就是将日志事件和日志器包装到一起，因为一条日志只会在一个日志器上进行输出。将日志事件和日志器包装到一起后，方便通过宏定义来简化日志模块的使用。另外，LogEventWrap还负责在构建时指定日志事件和日志器，在析构时调用日志器的log方法将日志事件进行输出。

LogManager: 日志器管理类，单例模式，用于统一管理所有的日志器，提供日志器的创建与获取方法。LogManager自带一个root Logger，用于为日志模块提供一个初始可用的日志器。


