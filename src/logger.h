#ifndef __LOGGER_H__
#define __LOGGER_H__

#define LOG_CONF					"../conf/logger.conf"

#define LOG_DEFAULT_FILENAME		"/var/log/logger"
#define LOG_DEFAULT_FILESIZE		(1024 * 1024)

#define CONFIG_MAX_LINE				128

#ifndef NAMESIZE
#define NAMESIZE 					32
#endif

#define LOG_DEBUG 	1
#define LOG_INFO	2
#define LOG_WARN	3
#define LOG_ERROR	4

int logger_init();

void logger_release();

void log_error(const char *format, ...);

void log_warn(const char *format, ...);

void log_info(const char *format, ...);

void log_debug(const char *format, ...);

#endif //__LOGGER_H_
