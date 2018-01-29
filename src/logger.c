/**************************************
 * Filename: logger.c
 * Author: zhangwj
 * description: Simple logging Library
 * Date: 2017-05-12
 *************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <pthread.h>
#include "logger.h"

#define RESERVED	64

typedef struct __logger_t
{
	char ifname[NAMESIZE];
	FILE *lfp;
	int enable;
	int level;
	unsigned int size;
	pthread_mutex_t mutex;	
} logger_t;

static logger_t * g_lptr = NULL;

int logger_load_conf(logger_t *l)
{
	FILE * fp = NULL;
	char line[CONFIG_MAX_LINE + 1];

	fp = fopen(LOG_CONF, "r");
	if (fp == NULL) {
		goto DEFAULT;
	}
	
	while (fgets(line, CONFIG_MAX_LINE + 1, fp)) {

		if (line[0] == '#'  || 
			line[0] == '\0' || 
			line[0] == '\r') {
			continue;
		}

		if(strstr(line, "filename")) {
			sscanf(line, "filename=%s", l->ifname);	
		} 
		else if (strstr(line, "log_enable")) {
			sscanf(line, "log_enable=%d", &l->enable);
		}	
		else if (strstr(line, "log_devel")) {	
			sscanf(line, "log_devel=%d", &l->level);	

		} 
		else if (strstr(line, "log_size")) {
			sscanf(line, "log_size=%u", &l->size);	
		}
	}
	fclose(fp);

	return 0;
	
DEFAULT:
	l->enable = 1;
	l->level = LOG_DEBUG;
	l->size =  LOG_DEFAULT_FILESIZE;
	strncpy(l->ifname, LOG_DEFAULT_FILENAME, NAMESIZE);
	return 0;
}

int logger_init()
{	
	FILE *lfp = NULL;

	g_lptr = (logger_t *)malloc(sizeof(logger_t));
	if (NULL == g_lptr) {
		printf("logger malloc memroy failure\n");
		return -1;
	}
	logger_load_conf(g_lptr);

	lfp = fopen(g_lptr->ifname, "w");
	if (NULL == lfp) {
		printf("fopen %s, Error[%d:%s]\n", g_lptr->ifname, errno, strerror(errno));
		return -1;
	}
	g_lptr->lfp = lfp;
	pthread_mutex_init(&g_lptr->mutex, NULL);

	return 0;
}

void logger_release()
{
	if (g_lptr) {
		pthread_mutex_destroy(&g_lptr->mutex);
		if (g_lptr->lfp) {
			fclose(g_lptr->lfp);
		}
		free(g_lptr);
	}
}

static int log_check_reset_file()
{
	long fileSize = 0;
	
	if ((fileSize = ftell(g_lptr->lfp)) == -1) {
		printf("An error with log file occurred: %s\n", strerror(errno));
		return 0;
	}   
	
	if (fileSize >= g_lptr->size) {
	
		fclose(g_lptr->lfp);
		if((g_lptr->lfp = fopen(g_lptr->ifname, "w")) == NULL) {
			printf("Can't open log file: %s", strerror(errno));
			return 0;
		}   
	}
	
	return 1;
}

static void log_vdebug(const char *format, const char *func, unsigned int line_id, int level, va_list args)
{
	char line[512];
	char timestr[128];
	char *logstr = NULL;
	time_t now;

	if (!g_lptr->enable) {
		return;
	}
	
	if (level <  g_lptr->level) {
		return;
	}

	if (NULL == format) {
		return;
	}
	
	time(&now);
	strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", localtime(&now));

	logstr = (char *)malloc(strlen(format) + strlen(timestr) + strlen(func) + RESERVED);
	if (NULL == logstr) {
		return;
	}
	
	switch(level) {
	case LOG_DEBUG:
		sprintf(logstr, "%s [DEBUG] %s():%d %s\n", timestr, func, line_id, format);
		break;
	case LOG_INFO:
		sprintf(logstr, "%s [INFO]: %s():%d %s\n", timestr, func, line_id, format);
		break;
	case LOG_WARN:
		sprintf(logstr, "%s [WARN]: %s():%d %s\n", timestr, func, line_id, format);
		break;
	case LOG_ERROR:
		sprintf(logstr, "%s [ERROR]: %s():%d %s\n", timestr, func, line_id, format);
		break;
	}

	if (g_lptr->lfp) {
		pthread_mutex_lock(&g_lptr->mutex);
		fseek(g_lptr->lfp, 0L, SEEK_END);
		vsnprintf(line, 511, line, args);
		if (!log_check_reset_file()) {
			pthread_mutex_unlock(&g_lptr->mutex);
			exit(1);
		}
		fwrite(logstr, strlen(logstr), 1, g_lptr->lfp);
		fflush(g_lptr->lfp);
		pthread_mutex_unlock(&g_lptr->mutex);
	}

	if (g_lptr->level == 1) {
		 vprintf(logstr, args);
    }

	if (logstr) {
		free(logstr);
		logstr = NULL;
	}
}

void logger_print(const char *format, const char *func, unsigned int line, int level, ...)
{
	va_list args;
	
	va_start(args, level);
	log_vdebug(format, func, line, level, args);
	va_end(args);
}

