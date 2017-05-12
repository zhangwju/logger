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
filename=/var/log/logger

log_enable=1

log_level=1

log_size=1048576

# 简单例子

