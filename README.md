# logger

描述：一个简单的日志库
API:
#define LOG_DEBUG   1
#define LOG_INFO    2
#define LOG_WARN    3
#define LOG_ERROR   4

int logger_init();

void logger_release();

void log_error(const char *format, ...);

void log_warn(const char *format, ...);

void log_info(const char *format, ...);

void log_debug(const char *format, ...);

配置文件：
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

