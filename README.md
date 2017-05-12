# logger

# 描述
一个简单的日志库
# API:
    int logger_init();

    void logger_release();

    void log_error(const char *format, ...);

    void log_warn(const char *format, ...);

    void log_info(const char *format, ...);

    void log_debug(const char *format, ...);

# 配置文件（conf/logger.conf）：
    #configure file of logger 
    #eg: filename=/var/log/logger
    filename=/var/log/logger

    #log_enable disable 0 | enable 1
    log_enable=1
    #log_level (LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG)
    #LOG_ERROR 1
    #LOG_WARN  2
    #LOG_INFO  3
    #LOG_DEBUG 4
    log_level=1

    #default 1M
    log_size=1048576

# 简单例子
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include "logger.h"

    int main()
    {
	    if (logger_init()) {
		      return -1;	
	    }

	    log_debug("This is debug, %s %d", "hello", 123);
	    log_info(" This is info");
	    log_warn(" This is warnning");
	    log_error("This is error");
	
	    logger_release();

	    return 0;
    }
    /*DEBUG: 2017-05-12 06:45:25 This is debug, hello 123
    INFO: 2017-05-12 06:45:25  This is info
    WARN: 2017-05-12 06:45:25  This is warnning
    ERROR: 2017-05-12 06:45:25 This is error
    */
